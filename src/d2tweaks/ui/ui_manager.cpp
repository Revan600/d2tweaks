#include <d2tweaks/ui/ui_manager.h>

#include <Windows.h>
#include <algorithm>

#include <common/hooking.h>

#include <d2tweaks/ui/menu.h>
#include <diablo2/d2win.h>
#include <spdlog/spdlog.h>

static LRESULT(__stdcall* g_wnd_proc_original)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

d2_tweaks::ui::ui_manager::ui_manager(token) {
	hooking::hook(diablo2::d2_win::get_base() + 0xD9B0, wnd_proc, reinterpret_cast<void**>(&g_wnd_proc_original));
}

void d2_tweaks::ui::ui_manager::add_menu(menu* m) {
	if (m == nullptr)
		return;

	const auto it = std::find(m_menus.begin(), m_menus.end(), m);

	if (it != m_menus.end())
		return;

	m_menus.push_back(m);
}

d2_tweaks::ui::menu* d2_tweaks::ui::ui_manager::get_menu(const std::string& name) {
	if (name.empty())
		return nullptr;

	//TODO: optimize somehow
	for (auto menu : m_menus) {
		if (menu->get_name() == name)
			return menu;
	}

	return nullptr;
}

void d2_tweaks::ui::ui_manager::remove_menu(menu* m) {
	if (m == nullptr)
		return;

	m_menus.erase(std::remove(m_menus.begin(), m_menus.end(), m), m_menus.end());
}

void d2_tweaks::ui::ui_manager::draw() {
	//process_inputs();

	for (auto menu : m_menus) {
		if (!menu->get_visible())
			continue;

		menu->draw();
	}
}

LRESULT d2_tweaks::ui::ui_manager::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static auto& instance = singleton<ui_manager>::instance();

	bool block;

	switch (msg) {
		case WM_LBUTTONDOWN:
		{
			block = instance.process_left_mouse(false);
			break;
		}

		case WM_LBUTTONUP:
		{
			block = instance.process_left_mouse(true);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			block = instance.process_right_mouse(false);
			break;
		}

		case WM_RBUTTONUP:
		{
			block = instance.process_right_mouse(true);
			break;
		}

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			block = instance.process_key_event(wParam, false);
			break;
		}

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			block = instance.process_key_event(wParam, true);
			break;
		}

		default: return g_wnd_proc_original(hWnd, msg, wParam, lParam);
	}

	if (block)
		return 0;

	return g_wnd_proc_original(hWnd, msg, wParam, lParam);
}

void d2_tweaks::ui::ui_manager::process_inputs() {
	if (GetAsyncKeyState(VK_LBUTTON)) {
		if (!m_was_down_before_left) {
			m_was_down_before_left = true;
			m_mouse_state_left = true;

			process_left_mouse(false);
		}
	} else if (m_was_down_before_left) {
		m_was_down_before_left = false;
		m_mouse_state_left = false;

		process_left_mouse(true);
	}

	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (!m_was_down_before_right) {
			m_was_down_before_right = true;
			m_mouse_state_right = true;

			process_right_mouse(false);
		}
	} else if (m_was_down_before_right) {
		m_was_down_before_right = false;
		m_mouse_state_right = false;

		process_right_mouse(true);
	}
}

bool d2_tweaks::ui::ui_manager::process_left_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->left_mouse(up);
	}

	return block;
}

bool d2_tweaks::ui::ui_manager::process_right_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->right_mouse(up);
	}

	return block;
}

bool d2_tweaks::ui::ui_manager::process_key_event(uint32_t key, bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->key_event(key, up);
	}

	return block;
}
