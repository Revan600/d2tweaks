#pragma once

#include <Windows.h>

#include <fw/singleton.h>

#include <vector>
#include <string>

namespace d2_tweaks {
	namespace ui {
		class menu;

		class ui_manager final : public singleton<ui_manager> {
			std::vector<menu*> m_menus;

			bool m_was_down_before_left = false;
			bool m_was_down_before_right = false;
			bool m_mouse_state_left = false;
			bool m_mouse_state_right = false;
		public:
			explicit ui_manager(token);

			void add_menu(menu* m);
			menu* get_menu(const std::string& name);
			void remove_menu(menu* m);

			void draw();
		private:
			static LRESULT __stdcall wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			void process_inputs();

			bool process_left_mouse(bool up);
			bool process_right_mouse(bool up);

			bool process_key_event(uint32_t key, bool up);
		};
	}
}
