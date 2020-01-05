#include <d2tweaks/ui/ui_manager.h>
#ifndef NDEBUG

#include <d2tweaks/client/modules/test/test.h>

#include <d2tweaks/common/asset_manager.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/ui/menu.h>

#include <diablo2/structures/unit.h>
#include <diablo2/d2client.h>

MODULE_INIT(test)

static d2_tweaks::common::asset* g_buttons_img;

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

		menu::draw();
	}
};

static test_menu* g_test_menu;

void d2_tweaks::client::modules::test::init() {
	g_test_menu = new test_menu();
	singleton<ui::ui_manager>::instance().add_menu(g_test_menu);
}

void d2_tweaks::client::modules::test::handle_packet(common::packet_header* packet) {}

#endif