#pragma once

#include <fw/singleton.h>

#include <vector>

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
			void remove_menu(menu* m);

			void draw();
		private:
			void process_inputs();
		};
	}
}
