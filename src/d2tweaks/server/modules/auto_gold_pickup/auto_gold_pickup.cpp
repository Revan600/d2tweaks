#include <d2tweaks/server/modules/auto_gold_pickup/auto_gold_pickup.h>
#include <d2tweaks/server/server.h>

#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/room.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/player_data.h>

MODULE_INIT(auto_gold_pickup)

void d2_tweaks::server::modules::auto_gold_pickup::init() {
	singleton<server>::instance().register_tick_handler(this);
}

void d2_tweaks::server::modules::auto_gold_pickup::tick(diablo2::structures::game* game,
														diablo2::structures::unit* unit) {
	static common::gold_pickup_info_sc packet;

	if (!game || !unit)
		return;

	if (unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return;

	const auto room = diablo2::d2_common::get_room_from_unit(unit);

	if (!room)
		return;

	for (auto item = room->unit; item; item = item->prev_unit_in_room) {
		if (!item)
			continue;

		if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
			continue;

		auto record = diablo2::d2_common::get_item_record(item->data_record_index);

		if (!record)
			continue;

		if (record->string_code[0] != 'g' ||
			record->string_code[1] != 'l' ||
			record->string_code[2] != 'd') { //Gold pile
			continue;
		}

		const auto distance = diablo2::d2_common::get_distance_between_units(unit, item);

		if (distance > 4)
			continue;

		const auto currentGold = diablo2::d2_common::get_stat(unit, diablo2::UNIT_STAT_GOLD, 0);
		const auto goldToPickup = diablo2::d2_common::get_stat(item, diablo2::UNIT_STAT_GOLD, 0);
		const auto maxGold = diablo2::d2_common::get_maximum_character_gold(unit);

		if (currentGold + goldToPickup > maxGold)
			continue;

		diablo2::d2_game::pickup_gold_pile(game, unit, item);
		packet.gold = goldToPickup;

		singleton<server>::instance().send_packet(unit->player_data->net_client, &packet, sizeof packet);
	}
}
