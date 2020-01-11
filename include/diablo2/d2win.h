#pragma once

#include <Windows.h>
#include <cstdint>

namespace diablo2 {
	enum ui_color_t {
		UI_COLOR_WHITE = 0x0,
		UI_COLOR_RED = 0x1,
		UI_COLOR_LIGHT_GREEN = 0x2,
		UI_COLOR_BLUE = 0x3,
		UI_COLOR_DARK_GOLD = 0x4,
		UI_COLOR_GREY = 0x5,
		UI_COLOR_BLACK = 0x6,
		UI_COLOR_GOLD = 0x7,
		UI_COLOR_ORANGE = 0x8,
		UI_COLOR_YELLOW = 0x9,
		UI_COLOR_DARK_GREEN = 0xA,
		UI_COLOR_PURPLE = 0xB,
		UI_COLOR_GREEN = 0xC,
		UI_COLOR_WHITE2 = 0xD,
		UI_COLOR_BLACK2 = 0xE,
		UI_COLOR_DARK_WHITE = 0xF,
		UI_COLOR_LIGHT_GREY = 0x10,
	};

	enum ui_font_t {
		UI_FONT_8 = 0,
		UI_FONT_16 = 1,
		UI_FONT_30 = 2,
		UI_FONT_42 = 3,
		UI_FONT_FORMAL10 = 4,
		UI_FONT_FORMAL12 = 5,
		UI_FONT_6 = 6,
		UI_FONT_24 = 7,
		UI_FONT_FORMAL11 = 8,
		UI_FONT_EXOCET10 = 9,
		UI_FONT_RIDICULOUS = 10,
		UI_FONT_EXOCET8 = 11,
		UI_FONT_REALLYTHELASTSUCKER = 12,
		UI_FONT_INGAMECHAT = 13
	};

	class d2_win {
	public:
		static char* get_base();

		static int32_t get_text_pixel_width(wchar_t* str);
		static void draw_text(wchar_t* str, uint32_t x, uint32_t y, ui_color_t color, int32_t transTbl);
		static void draw_boxed_text(wchar_t* str, uint32_t x, uint32_t y, int32_t paletteIndex, int32_t transTbl, ui_color_t color);
		static void set_popup_properties(wchar_t* str, uint32_t x, uint32_t y, ui_color_t color, int32_t align);
		static void draw_popup();

		static ui_font_t get_current_font();
		static int32_t get_current_font_height();
		static int32_t set_current_font(ui_font_t font);

		static void* load_mpq(char* dllName, char* mpqName, char* mpqTitle, int32_t overrideFlags);
		static bool unload_mpq(void* mpq);
	};
}