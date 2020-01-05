#pragma once

#include <d2tweaks/ui/controls/control.h>
#include <diablo2/structures/gfxdata.h>

namespace d2_tweaks {
	namespace common {
		class asset;
	}
	
	namespace ui {
		namespace controls {
			class image : public control {
				int32_t m_x, m_y;
				common::asset* m_image;
				int32_t m_frame;
				diablo2::structures::gfxdata m_draw_info;
			public:
				explicit image(common::asset* image, int32_t x = 0, int32_t y = 0, int32_t frame = 0);

				void set_x(int32_t value) override {
					m_x = value;
					control::set_x(value);
				}

				int32_t get_x() const override {
					return m_x;
				}

				void set_y(int32_t value) override {
					m_y = value;
					control::set_y(value);
				}

				int32_t get_y() const override {
					return m_y;
				}

				void set_frame(int32_t frame) {
					m_frame = static_cast<uint32_t>(frame);
				}

				int32_t get_frame() const {
					return static_cast<int32_t>(m_frame);
				}

				void draw() override;
				void draw(int32_t offsetX, int32_t offsetY) override;

				void left_mouse(bool up) override;
				void right_mouse(bool up) override;
			};
		}
	}
}
