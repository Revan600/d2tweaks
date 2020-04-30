#pragma once

#include <d2tweaks/ui/controls/control.h>
#include <pugixml.hpp>
#include <vector>

namespace d2_tweaks {
	namespace ui {
		class menu;

		namespace controls {
			class group : public control {
				std::vector<control*> m_controls;
			public:
				explicit group(menu* menu, int32_t x, int32_t y);
				explicit group(menu* menu, const pugi::xml_node& node);

				void draw() override;
				void draw(int32_t offsetX, int32_t offsetY) override;

				void left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) override;
				void right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) override;

				void key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) override;

			private:
				void add_control(control* control);
			};
		}
	}
}
