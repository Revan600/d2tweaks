#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_toggle_menu.h>

#include <functional>

#include <d2tweaks/ui/controls/button.h>

#include <diablo2/d2client.h>
#include <d2tweaks/ui/ui_manager.h>

d2_tweaks::client::modules::loot_filter_settings_toggle_menu::loot_filter_settings_toggle_menu(token) {
	m_show = false;

	menu::set_enabled(true);
	menu::set_visible(true);

	load_xml("d2tweaks\\interfaces\\loot_filter_settings_toggle_menu.xml");

	m_toggle_filter_settings_btn = static_cast<ui::controls::button*>(
		get_control("m_toggle_filter_settings_btn"));
	m_toggle_filter_settings_btn->set_enabled(false);
	m_toggle_filter_settings_btn->set_visible(false);
	m_toggle_filter_settings_btn->set_on_click(std::bind(&loot_filter_settings_toggle_menu::toggle_filter_settings_click, this));

	m_filter_settings_menu = singleton<ui::ui_manager>::instance().get_menu("loot_filter_settings_menu");
}

void d2_tweaks::client::modules::loot_filter_settings_toggle_menu::toggle_filter_settings_click() {
	m_show = !m_show;

	m_filter_settings_menu->set_enabled(m_show);
	m_filter_settings_menu->set_visible(m_show);
}

void d2_tweaks::client::modules::loot_filter_settings_toggle_menu::draw() {
	m_toggle_filter_settings_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MINIPANEL));
	m_toggle_filter_settings_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MINIPANEL));

	menu::draw();
}

bool d2_tweaks::client::modules::loot_filter_settings_toggle_menu::key_event(uint32_t key, bool up) {
	if (key == VK_ESCAPE && m_show) {
		m_show = false;

		m_filter_settings_menu->set_enabled(m_show);
		m_filter_settings_menu->set_visible(m_show);

		return true; //block escape key stroke
	}

	return menu::key_event(key, up);
}
