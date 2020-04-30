#include <d2tweaks/server/modules/item_move/item_move.h>

#include <d2tweaks/server/server.h>

#include <diablo2/d2net.h>
#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/net_client.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

MODULE_INIT(item_move)

void d2_tweaks::server::modules::item_move::init() {
	singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_ITEM_MOVE, this);
}

bool d2_tweaks::server::modules::item_move::handle_packet(diablo2::structures::game* game,
														  diablo2::structures::unit* player, common::packet_header* packet) {
	static common::item_move_sc resp;
	static auto& instance = singleton<server>::instance();

	const auto itemMove = static_cast<common::item_move_cs*>(packet);
	const auto item = instance.get_server_unit(game, itemMove->item_guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM); //0x4 = item

	if (item == nullptr)
		return true; //block further packet processing

	const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, itemMove->target_page, game->item_format == 101);

	uint32_t tx, ty;

	if (!find_free_space(player->inventory, item, inventoryIndex, itemMove->target_page, tx, ty))
		return true; //block further packet processing

	item->item_data->page = itemMove->target_page;

	diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, item->item_data->page);
	diablo2::d2_common::inv_update_item(player->inventory, item, false);

	diablo2::d2_game::update_inventory_items(game, player);

	//send update packet
	resp.item_guid = itemMove->item_guid;
	resp.target_page = itemMove->target_page;
	resp.tx = tx;
	resp.ty = ty;

	const auto client = player->player_data->net_client;

	diablo2::d2_net::send_to_client(1, client->client_id, &resp, sizeof resp);

	return true;
}

bool d2_tweaks::server::modules::item_move::find_free_space(diablo2::structures::inventory* inv,
															diablo2::structures::unit* item, int32_t inventoryIndex, char page, uint32_t& x, uint32_t& y) {
	char data[0x18];

	diablo2::d2_common::get_inventory_data(inventoryIndex, 0, data);

	const auto mx = static_cast<uint32_t>(data[0]);
	const auto my = static_cast<uint32_t>(data[1]);

	for (x = 0; x < mx; x++) {
		for (y = 0; y < my; y++) {
			diablo2::structures::unit* blockingUnit = nullptr;
			uint32_t blockingUnitIndex = 0;

			if (diablo2::d2_common::can_put_into_slot(inv, item, x, y, inventoryIndex, &blockingUnit, &blockingUnitIndex, page))
				return true;
		}
	}

	return false;
}
