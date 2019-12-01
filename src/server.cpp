#include <server.h>

#include <Windows.h>
#include <MinHook.h>

#include <spdlog/spdlog.h>

#include <common/d2protocol.h>
#include <common/d2common.h>
#include <common/d2function.h>

#include <common.h>

static char* d2_common = reinterpret_cast<char*>(GetModuleHandle("d2common.dll"));
static char* d2_game = reinterpret_cast<char*>(GetModuleHandle("d2game.dll"));
static char* d2_net = reinterpret_cast<char*>(GetModuleHandle("d2net.dll"));

static item_mover::server* g_server;
static item_mover::common* g_common;

static d2_func_std_import<int32_t(int32_t queue, int32_t clientId, void* packet, size_t size)> send_to_client(10006, d2_net);

static d2_func_std_import< uint32_t(inventory * inv, unit * item, uint32_t x, uint32_t y,
	uint32_t invIndex, unit * *lastBlockingUnit, uint32_t * lastBlockingUnitIndex, uint8_t page)> can_put_into_slot(
		10247, d2_common);
static d2_func_std_import<int32_t(unit * item, char page, BOOL lod)> get_inventory_index(10409, d2_common);
static d2_func_std_import<BOOL(inventory * inv, unit * item, BOOL isClient)> inv_update_item(10242, d2_common);
static d2_func_std_import<BOOL(inventory * inv, unit * item, uint32_t x, uint32_t y,
	uint32_t invIndex, BOOL isClient, uint8_t page)> inv_add_item(
		10249, d2_common);

static d2_func_fast<void(game*, unit*, uint32_t)>update_inventory_items(reinterpret_cast<void*>(0x14A90), d2_game);

static int32_t(__fastcall* g_get_incoming_packet_info)(d2_protocol::packet_header* data, unsigned int dataSize, size_t* packetSizeOut, size_t* someOffset, int* packetGroup, int32_t* a6, int a7, int a8);

static int32_t(__fastcall* g_handle_packet)(game* game, unit* player, d2_protocol::packet_header* data, size_t size);

//returns some kind of processing type (i.e. resultGroup == 0x04 means drop packet)
static int32_t __fastcall get_incoming_packet_info(d2_protocol::packet_header* data, unsigned int dataSize, size_t* packetSizeOut, size_t* someOffset, int* packetGroup, int32_t* a6, int a7, int a8) {
	static d2_protocol::packet_header dummy;

	const auto resultGroup = g_get_incoming_packet_info(data, dataSize, packetSizeOut, someOffset, packetGroup, a6, a7, a8);

	if (data->d2_packet_type == dummy.d2_packet_type && resultGroup == 0x04) { //0x04 - drop packet
		size_t size;

		if (!g_common->get_packet_size_cs(data, size))
			return resultGroup;

		*packetSizeOut = size;
		*packetGroup = 1;
		*a6 = 100;
		return 2;
	}

	return resultGroup;
}

static int32_t __fastcall handle_packet(game* game, unit* player, d2_protocol::packet_header* data, size_t size) {
	static d2_protocol::packet_header dummy;

	if (data->d2_packet_type == dummy.d2_packet_type) {
		if (!g_server->handle_packet(game, player, data))
			return g_handle_packet(game, player, data, size);

		return 1;
	}

	return g_handle_packet(game, player, data, size);
}

item_mover::server::server() {
	g_server = this;
}

void item_mover::server::init(common* common) {
	g_common = common;

	MH_CreateHook(reinterpret_cast<void*>(d2_game + 0x59320), ::handle_packet, reinterpret_cast<void**>(&g_handle_packet));
	MH_CreateHook(reinterpret_cast<void*>(d2_net + 0x1FE0), get_incoming_packet_info, reinterpret_cast<void**>(&g_get_incoming_packet_info));

	//disable outgoing packet type checks
	DWORD oldProtect;
	if (VirtualProtect(d2_net + 0x18B2, 0x02, PAGE_EXECUTE_READWRITE, &oldProtect))
		*reinterpret_cast<uint16_t*>(d2_net + 0x18B2) = 0xBC3C;

	if (VirtualProtect(d2_net + 0x18D0, 0x02, PAGE_EXECUTE_READWRITE, &oldProtect))
		*reinterpret_cast<uint16_t*>(d2_net + 0x18D0) = 0xBC3C;
}

bool item_mover::server::handle_packet(game* game, unit* player, d2_protocol::packet_header* packet) {
	switch (packet->message_type) {
		case d2_protocol::MESSAGE_TYPE_ITEM_MOVE:
			return handle_item_move(game, player, packet);
		default:
			return false;
	}
}

unit* item_mover::server::get_server_unit(game* game, uint32_t guid, uint32_t type) {
	if (game == nullptr)
		return nullptr;

	auto typeIndex = type;

	if (type == 3)
		typeIndex = 4;

	if (type == 4)
		typeIndex = 3;

	const auto index = guid & 127;

	auto result = game->unit_list[typeIndex][index];

	while (result != nullptr && result->item_num != guid) {
		result = result->unit1;
	}

	return result;
}

bool item_mover::server::find_free_space(inventory* inv, unit* item, int32_t inventoryIndex, char page, uint32_t& x,
										 uint32_t& y) {
	//15x15 max page size because I'm too lazy to implement proper page size fetching
	const auto mx = 15;
	const auto my = 15;

	for (x = 0; x < mx; x++) {
		for (y = 0; y < my; y++) {
			unit* blockingUnit = nullptr;
			uint32_t blockingUnitIndex = 0;

			if (can_put_into_slot(inv, item, x, y, inventoryIndex, &blockingUnit, &blockingUnitIndex, page))
				return true;
		}
	}

	return false;
}

bool item_mover::server::handle_item_move(game* game, unit* player, d2_protocol::packet_header* packet) {
	static d2_protocol::item_move_sc resp;

	const auto itemMove = static_cast<d2_protocol::item_move_cs*>(packet);
	const auto item = get_server_unit(game, itemMove->item_guid, 0x4); //0x4 = item

	if (item == nullptr)
		return true; //block further packet processing

	spdlog::debug("Got d2_protocol::item_move_cs");

	const auto inventoryIndex = get_inventory_index(player, itemMove->target_page, game->item_format == 101);

	uint32_t tx, ty;

	if (!find_free_space(player->inventory, item, inventoryIndex, itemMove->target_page, tx, ty))
		return true; //block further packet processing

	item->item_data->page = itemMove->target_page;

	inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, item->item_data->page);
	inv_update_item(player->inventory, item, false);

	update_inventory_items(game, player, 0);

	//send update packet
	resp.item_guid = itemMove->item_guid;
	resp.target_page = itemMove->target_page;
	resp.tx = tx;
	resp.ty = ty;

	const auto client = player->player_data->net_client;

	send_to_client(1, client->client_id, &resp, sizeof resp);

	return true;
}
