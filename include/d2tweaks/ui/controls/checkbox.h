#pragma once

#include <d2tweaks/ui/controls/control.h>

#include <pugixml.hpp>
#include <functional>
#include <d2tweaks/ui/rect.h>

namespace d2_tweaks {
	namespace common {
		class asset;
	}
}

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class image;
			class label;

			class checkbox : public control {
				rect m_rect;
				image* m_image;
				label* m_label;

				std::wstring m_popup;

				int32_t m_frame_checked;
				int32_t m_frame_unchecked;
				int32_t m_click_sound;

				bool m_is_down;
				bool m_state;

				std::function<void(bool)> m_on_click;
			public:
				checkbox(const std::wstring& text, const rect& rect, const std::function<void()>& onClick,
						 common::asset* image, int32_t frameChecked, int32_t frameUnchecked, int32_t clickSound = -1);
				explicit checkbox(const pugi::xml_node& node);

				void set_x(int32_t value) override;
				int32_t get_x() const override {
					return m_rect.get_x();
				}

				void set_y(int32_t value) override;
				int32_t get_y() const override {
					return m_rect.get_y();
				}

				std::wstring popup() const {
					return m_popup;
				}

				void set_popup(const std::wstring& popup) {
					m_popup = popup;
				}

				std::function<void(bool)> get_on_click() const {
					return m_on_click;
				}

				void set_on_click(const std::function<void(bool)>& on_click) {
					m_on_click = on_click;
				}

				bool get_state() const {
					return m_state;
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
