#pragma once

#include <d2tweaks/ui/controls/control.h>
#include <diablo2/structures/gfxdata.h>

#include <pugixml.hpp>
#include <d2tweaks/ui/rect.h>

namespace d2_tweaks {
	namespace common {
		class asset;
	}

	namespace ui {
		namespace controls {
			class image : public control {
				common::asset* m_image;
				int32_t m_frame;

				rect m_rect;
				bool m_block_click;
				diablo2::structures::gfxdata m_draw_info;
			public:
				explicit image(menu* menu, common::asset* image, int32_t x = 0, int32_t y = 0, int32_t frame = 0);
				explicit image(menu* menu, const pugi::xml_node& node);

				void set_frame(int32_t frame) {
					m_frame = static_cast<uint32_t>(frame);
				}

				int32_t get_frame() const {
					return static_cast<int32_t>(m_frame);
				}

				void draw() override;
				void draw(int32_t offsetX, int32_t offsetY) override;

				void left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) override;
				void right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) override;

				void key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) override;
			};
		}
	}
}
