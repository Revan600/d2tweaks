#pragma once

#include <fw/singleton.h>
#include <d2tweaks/ui/menu.h>

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class button;
		}
	}
}

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class loot_filter_settings_toggle_menu final : public ui::menu, singleton<loot_filter_settings_toggle_menu> {
				ui::controls::button* m_toggle_filter_settings_btn;
				menu* m_filter_settings_menu;
				bool m_show;
			public:
				explicit loot_filter_settings_toggle_menu(token);

				void toggle_filter_settings_click();

				void draw() override;

				bool key_event(uint32_t key, bool up) override;
			};
		}
	}
}
