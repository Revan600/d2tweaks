#include <d2tweaks/client/modules/loot_filter/loot_filter.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_menu.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_toggle_menu.h>

#include <common/hooking.h>

#include <d2tweaks/ui/ui_manager.h>

#include <diablo2/d2client.h>
#include <diablo2/d2launch.h>

MODULE_INIT(loot_filter)

static char* (__fastcall* g_set_player_name_original)(void*, void*);
static char* __fastcall set_player_name(void* player, void* edx) {
	const auto result = g_set_player_name_original(player, edx);

	d2_tweaks::client::modules::loot_filter_settings::load(diablo2::d2_client::get_local_player_name());
	singleton<d2_tweaks::client::modules::loot_filter_settings_menu>::instance().reload_settings();

	return result;
}

static HANDLE(__fastcall* g_delete_save_file_original)(char*, char*);
static HANDLE __fastcall delete_save_file(char* name, char* a2) {
	d2_tweaks::client::modules::loot_filter_settings::remove(name);
	return g_delete_save_file_original(name, a2);
}

void d2_tweaks::client::modules::loot_filter::init() {
	hooking::hook(diablo2::d2_client::get_base() + 0xBDE0, set_player_name, &g_set_player_name_original);
	hooking::hook(diablo2::d2_launch::get_base() + 0x17C00, delete_save_file, &g_delete_save_file_original);

	singleton<ui::ui_manager>::instance().add_menu(&singleton<loot_filter_settings_menu>::instance());
	singleton<ui::ui_manager>::instance().add_menu(&singleton<loot_filter_settings_toggle_menu>::instance());
}
