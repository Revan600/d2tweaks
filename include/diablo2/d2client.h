#pragma once

#include <Windows.h>
#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct gfxdata;
		struct unit;
		struct client_unit_list;
		struct cellfile;
	}

	enum ui_window_t {
		UI_WINDOW_INVENTORY = 0x01,
		UI_WINDOW_CHARACTER = 0x02,
		UI_WINDOW_MINISKILL = 0x03,
		UI_WINDOW_SKILL = 0x04,
		UI_WINDOW_CHAT = 0x05,
		UI_WINDOW_NPCMENU = 0x08,
		UI_WINDOW_MAINMENU = 0x09,
		UI_WINDOW_AUTOMAP = 0x0a,
		UI_WINDOW_CONFIG = 0x0b,
		UI_WINDOW_NPCSHOP = 0x0c,
		UI_WINDOW_ALTDOWN = 0x0d,
		UI_WINDOW_ANVIL = 0x0e,
		UI_WINDOW_QUEST = 0x0f,
		UI_WINDOW_QUESTLOG = 0x11,
		UI_WINDOW_STATUSAREA = 0x12,
		UI_WINDOW_WPMENU = 0x14,
		UI_WINDOW_MINIPANEL = 0x15,
		UI_WINDOW_PARTY = 0x16,
		UI_WINDOW_TRADE = 0x17,
		UI_WINDOW_MSGS = 0x18,
		UI_WINDOW_STASH = 0x19,
		UI_WINDOW_CUBE = 0x1a,
		UI_WINDOW_BELT = 0x1f,
		UI_WINDOW_HELP = 0x21,
		UI_WINDOW_MERC = 0x24,
		UI_WINDOW_SCROLL = 0x25
	};

	class d2_client {
	public:
		static char* get_base();

		static bool is_lod();

		static structures::unit* get_local_player();
		static const char* get_local_player_name();
		static structures::client_unit_list* get_client_unit_list();

		static int32_t get_view_offset_x();
		static int32_t get_view_offset_y();
		
		static uint32_t get_mouse_x();
		static uint32_t get_mouse_y();
		static bool get_ui_window_state(ui_window_t window);
		static void* get_buysellbtn();

		static void play_sound(uint32_t soundId, structures::unit* u, uint32_t ticks, BOOL prePick, uint32_t cache);

		static structures::unit* get_unit_by_guid(int32_t type, int32_t guid);

		static void send_to_server(void* data, size_t size);

		static bool cache_gfx_data(structures::gfxdata* gfxData,
								   structures::unit* unit,
								   structures::cellfile* cellfFile,
								   int32_t direction,
								   int32_t frame,
								   int32_t* outIndex,
								   int8_t flags,
								   int32_t colorTint);
		
		static structures::cellfile* load_gfx_resource(char* path);
		static int32_t unload_gfx_resource(structures::cellfile* handle);
	};
}