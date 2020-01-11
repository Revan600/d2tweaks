#pragma once

#include <d2tweaks/ui/controls/control.h>

#include <pugixml.hpp>

#include <diablo2/d2win.h>

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class label : public control {
				std::wstring m_text;
				bool m_text_owned;
				diablo2::ui_color_t m_color;
				diablo2::ui_font_t m_font;
			public:
				explicit label(const std::wstring& text, int32_t x = 0, int32_t y = 0,
							   diablo2::ui_color_t color = diablo2::UI_COLOR_WHITE,
							   diablo2::ui_font_t font = diablo2::UI_FONT_16);
				explicit label(const pugi::xml_node& node);

				void set_text(const std::wstring& text) {
					m_text = text;
				}

				const std::wstring& get_text() const {
					return m_text;
				}


				diablo2::ui_color_t get_color() const {
					return m_color;
				}

				void set_color(const diablo2::ui_color_t color) {
					m_color = color;
				}

				diablo2::ui_font_t get_font() const {
					return m_font;
				}

				void set_font(const diablo2::ui_font_t font) {
					m_font = font;
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
