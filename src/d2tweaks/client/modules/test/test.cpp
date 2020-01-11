#include <d2tweaks/client/modules/test/test.h>

#include <d2tweaks/common/asset_manager.h>

#include <spdlog/spdlog.h>
#include <magic_enum.hpp>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>

#include <diablo2/structures/unit.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>

MODULE_INIT(test)

static d2_tweaks::common::asset* g_buttons_img;
static constexpr auto ENUM_VALUES = magic_enum::enum_values<diablo2::ui_window_t>();

static bool string_to_wstring(const char* str, wchar_t* outStr, size_t outStrLength) {
	if (!str || !outStr)
		return false;

	const auto strLength = strlen(str);

	if (strLength == 0)
		return false;

	memset(outStr, 0, outStrLength / sizeof(wchar_t));

	MultiByteToWideChar(CP_ACP, 0, str, strLength, outStr, outStrLength / sizeof(wchar_t));

	return true;
}

class test_menu : public d2_tweaks::ui::menu {
public:
	test_menu() {
		menu::set_enabled(true);
		menu::set_visible(true);
	}

	void draw() override {
		const auto player = diablo2::d2_client::get_local_player();

		if (!player)
			return;

		draw_cursor_pos();

		menu::draw();
	}
private:
	void draw_cursor_pos() {
		static wchar_t buffer[512]{ 0 };

		auto mx = diablo2::d2_client::get_mouse_x();
		auto my = diablo2::d2_client::get_mouse_y();

		swprintf_s(buffer, L"%i, %i", mx, my);
		diablo2::d2_win::draw_text(buffer, mx, my, diablo2::UI_COLOR_WHITE, 0);
	}

	void draw_window_states() {
		auto i = 0;
		const auto height = diablo2::d2_win::get_current_font_height() + 5;

		for (const auto& value : ENUM_VALUES) {
			draw_window_state(value, 100 + (i++ * height));
		}
	}

	void draw_window_state(diablo2::ui_window_t window, uint32_t y) {
		static wchar_t buffer[512]{ 0 };
		static wchar_t name_buffer[512]{ 0 };
		memset(buffer, 0, sizeof buffer);

		auto nameView = magic_enum::enum_name(window);
		string_to_wstring(nameView.data(), name_buffer, sizeof name_buffer);
		const auto state = diablo2::d2_client::get_ui_window_state(window);
		swprintf_s(buffer, L"%s: %i", name_buffer, static_cast<int32_t>(state));
		diablo2::d2_win::draw_text(buffer, 40, y, diablo2::UI_COLOR_WHITE, 0);
	}
};

static test_menu* g_test_menu;

void d2_tweaks::client::modules::test::init() {
#ifndef NDEBUG
	g_test_menu = new test_menu();
	singleton<ui::ui_manager>::instance().add_menu(g_test_menu);
#endif
}

void d2_tweaks::client::modules::test::handle_packet(common::packet_header* packet) {}