#include <d2tweaks/client/modules/auto_gold_pickup/auto_gold_pickup.h>
#include <d2tweaks/client/client.h>

#include <d2tweaks/common/protocol.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>

#include <diablo2/d2client.h>
#include <diablo2/d2win.h>

MODULE_INIT(auto_gold_pickup)

static const uint32_t DISPLAY_TIME = 2500;
static uint32_t g_last_update = 0;
static uint32_t g_value = 0;

class draw_gold_menu final : public d2_tweaks::ui::menu {
public:
	draw_gold_menu() {
		menu::set_visible(true);
		menu::set_enabled(true);
	}

	void draw() override {
		static wchar_t buffer[1024];

		if (g_value == 0) {
			return;
		}

		if (GetTickCount() - g_last_update >= DISPLAY_TIME) {
			g_value = 0;
			return;
		}

		swprintf_s(buffer, L"+%i", g_value);

		const auto font = diablo2::d2_win::get_current_font();

		diablo2::d2_win::set_current_font(diablo2::UI_FONT_16);

		diablo2::d2_win::draw_text(buffer,
								   550,
								   540,
								   diablo2::UI_COLOR_GOLD, 0);

		diablo2::d2_win::set_current_font(font);
	}
};

void d2_tweaks::client::modules::auto_gold_pickup::init() {
	singleton<client>::instance().register_packet_handler(common::message_types_t::MESSAGE_TYPE_GOLD_PICKUP_INFO, this);
	singleton<ui::ui_manager>::instance().add_menu(new draw_gold_menu());
}

void d2_tweaks::client::modules::auto_gold_pickup::handle_packet(common::packet_header* packet) {
	const auto info = static_cast<common::gold_pickup_info_sc*>(packet);

	g_last_update = GetTickCount();
	g_value += info->gold;
}
