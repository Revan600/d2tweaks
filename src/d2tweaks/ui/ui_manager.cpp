#include <d2tweaks/ui/ui_manager.h>

#include <Windows.h>
#include <algorithm>

#include <d2tweaks/ui/menu.h>

d2_tweaks::ui::ui_manager::ui_manager(token) {}

void d2_tweaks::ui::ui_manager::add_menu(menu* m) {
	if (m == nullptr)
		return;

	const auto it = std::find(m_menus.begin(), m_menus.end(), m);

	if (it != m_menus.end())
		return;

	m_menus.push_back(m);
}

void d2_tweaks::ui::ui_manager::remove_menu(menu* m) {
	if (m == nullptr)
		return;

	m_menus.erase(std::remove(m_menus.begin(), m_menus.end(), m), m_menus.end());
}

void d2_tweaks::ui::ui_manager::draw() {
	process_inputs();

	for (auto menu : m_menus) {
		if (!menu->get_visible())
			continue;

		menu->draw();
	}
}

void d2_tweaks::ui::ui_manager::process_inputs() {
	if (GetAsyncKeyState(VK_LBUTTON)) {
		if (!m_was_down_before_left) {
			m_was_down_before_left = true;
			m_mouse_state_left = true;

			for (auto menu : m_menus) {
				if (!menu->get_enabled())
					continue;

				menu->left_mouse(false);
			}
		}
	} else if (m_was_down_before_left) {
		m_was_down_before_left = false;
		m_mouse_state_left = false;

		for (auto menu : m_menus) {
			if (!menu->get_enabled())
				continue;

			menu->left_mouse(true);
		}
	}

	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (!m_was_down_before_right) {
			m_was_down_before_right = true;
			m_mouse_state_right = true;

			for (auto menu : m_menus) {
				if (!menu->get_enabled())
					continue;

				menu->right_mouse(false);
			}
		}
	} else if (m_was_down_before_right) {
		m_was_down_before_right = false;
		m_mouse_state_right = false;

		for (auto menu : m_menus) {
			if (!menu->get_enabled())
				continue;

			menu->right_mouse(true);
		}
	}
}
