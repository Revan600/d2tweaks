#include <d2tweaks/server/modules/identify_on_pickup/identify_on_pickup.h>
#include <d2tweaks/server/server.h>

#include <common/hooking.h>

#include <diablo2/d2game.h>
#include <diablo2/structures/unit.h>

MODULE_INIT(identify_on_pickup)

static uint32_t(__fastcall* g_pickup_item_original)(diablo2::structures::game*,
													diablo2::structures::unit*,
													uint32_t, uint32_t);
static uint32_t __fastcall pickup_item(diablo2::structures::game* game,
									   diablo2::structures::unit* player,
									   uint32_t guid,
									   uint32_t a4) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	if (!game || !player || player->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return g_pickup_item_original(game, player, guid, a4);

	const auto item = instance.get_server_unit(game, guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM);

	if (!item)
		return g_pickup_item_original(game, player, guid, a4);

	diablo2::d2_game::identify_item(game, player, item);

	return g_pickup_item_original(game, player, guid, a4);
}

static uint32_t(__fastcall* g_pickup_item_cursor_original)(diablo2::structures::game*,
													diablo2::structures::unit*,
													uint32_t, uint32_t);
static uint32_t __fastcall pickup_item_cursor(diablo2::structures::game* game,
									   diablo2::structures::unit* player,
									   uint32_t guid,
									   uint32_t a4) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	if (!game || !player || player->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return g_pickup_item_cursor_original(game, player, guid, a4);

	const auto item = instance.get_server_unit(game, guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM);

	if (!item)
		return g_pickup_item_cursor_original(game, player, guid, a4);

	diablo2::d2_game::identify_item(game, player, item);

	return g_pickup_item_cursor_original(game, player, guid, a4);
}

void d2_tweaks::server::modules::identify_on_pickup::init() {
	hooking::hook(diablo2::d2_game::get_base() + 0x13340, pickup_item, &g_pickup_item_original);
	hooking::hook(diablo2::d2_game::get_base() + 0x12B80, pickup_item_cursor, &g_pickup_item_cursor_original);
}
