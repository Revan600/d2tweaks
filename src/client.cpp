#include <client.h>

#include <Windows.h>
#include <MinHook.h>

#include <spdlog/spdlog.h>

#include <common/d2protocol.h>
#include <common/d2common.h>
#include <common/d2function.h>

static char* d2_client = reinterpret_cast<char*>(GetModuleHandle("d2client.dll"));
static char* d2_common = reinterpret_cast<char*>(GetModuleHandle("d2common.dll"));
static char* d2_net = reinterpret_cast<char*>(GetModuleHandle("d2net.dll"));

static item_mover::client* g_client;

static d2_func_std<unit * ()> get_local_player(reinterpret_cast<void*>(0x883D0), d2_client);

static d2_func_fast<unit * (int32_t type, int32_t guid)> get_unit_by_guid(reinterpret_cast<void*>(0x869F0), d2_client);

static d2_func_std_import<int32_t(unit * item, char page, BOOL lod)> get_inventory_index(10409, d2_common);
static d2_func_std_import<BOOL(inventory * inv, unit * item, BOOL isClient)> inv_update_item(10242, d2_common);

static d2_func_std_import<BOOL(inventory * inv, unit * item, uint32_t x, uint32_t y,
	uint32_t invIndex, BOOL isClient, uint8_t page)> inv_add_item(
		10249, d2_common);

static d2_func_std_import<items_line * (uint32_t id)> get_item_record(10600, d2_common);

static d2_func_fast<void(uint32_t soundId, unit * u, uint32_t ticks, BOOL prePick, uint32_t cache)>
play_sound(reinterpret_cast<void*>(0xB5820), d2_client);

//8901CB0
//void __fastcall sub_8901CB0(int a1, int a2)
static void(__fastcall* g_handle_packet)(d2_protocol::packet_header* packet, size_t size);

static void __fastcall handle_packet(d2_protocol::packet_header* packet, size_t size) {
	static d2_protocol::packet_header dummy;

	if (size == -1)
		return;

	if (packet->d2_packet_type != dummy.d2_packet_type) {
		g_handle_packet(packet, size);
		return;
	}

	g_client->handle_packet(packet);
}

item_mover::client::client() {
	g_client = this;
}

void item_mover::client::init() {
	MH_CreateHook(reinterpret_cast<void*>(d2_client + 0x11CB0), ::handle_packet, reinterpret_cast<void**>(&g_handle_packet));
}

void item_mover::client::handle_packet(d2_protocol::packet_header* packet) const {
	switch (packet->message_type) {
		case d2_protocol::MESSAGE_TYPE_ITEM_MOVE:
			return handle_item_move(packet);
		default:;
	}
}

unit* item_mover::client::get_client_unit(uint32_t type, uint32_t guid) {
	static auto units = reinterpret_cast<client_unit_list*>(d2_client + 0x11AA00);

	const auto index = guid & 127;

	auto result = units->unit_list[type][index];

	while (result != nullptr && result->item_num != guid) {
		result = result->unit1;
	}

	return result;
}

void item_mover::client::handle_item_move(d2_protocol::packet_header* packet) const {
	const auto itemMove = static_cast<d2_protocol::item_move_sc*>(packet);
	const auto item = g_client->get_client_unit(0x04, itemMove->item_guid); //0x03 -> 0x04 - item

	if (item == nullptr)
		return;

	spdlog::debug("[CLIENT] Got {0} packet!", reinterpret_cast<void*>(packet->d2_packet_type));

	const auto player = get_local_player();

	//Last parameter is some boolean
	const auto inventoryIndex = get_inventory_index(player, itemMove->target_page, *reinterpret_cast<int*>(d2_client + 0x1077C4));

	item->item_data->page = itemMove->target_page;

	inv_add_item(player->inventory, item, itemMove->tx, itemMove->ty, inventoryIndex, true, item->item_data->page);
	inv_update_item(player->inventory, item, true);

	const auto itemRecord = get_item_record(item->txt_file_no);

	if (itemRecord != nullptr)
		play_sound(itemRecord->wdropsound, nullptr, 0, 0, 0);
	else
		play_sound(4, nullptr, 0, 0, 0);
}
