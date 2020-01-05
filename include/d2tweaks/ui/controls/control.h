#pragma once

#include <string>
#include <cstdint>

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class control {
				std::string m_name;
				bool m_enabled = false;
				bool m_visible = false;
				int32_t m_x = 0;
				int32_t m_y = 0;
			public:
				virtual ~control() = default;

				std::string get_name() const {
					return m_name;
				}

				void set_name(const std::string& name) {
					m_name = name;
				}

				virtual bool get_enabled() const {
					return m_enabled;
				}

				virtual void set_enabled(bool value) {
					m_enabled = value;
				}

				virtual bool get_visible() const {
					return m_visible;
				}

				virtual void set_visible(bool value) {
					m_visible = value;
				}

				virtual int32_t get_x() const {
					return m_x;
				}

				virtual void set_x(int32_t value) {
					m_x = value;
				}

				virtual int32_t get_y() const {
					return m_y;
				}

				virtual void set_y(int32_t value) {
					m_y = value;
				}

				virtual void draw() = 0;
				virtual void draw(int32_t offsetX, int32_t offsetY) = 0;

				virtual void left_mouse(bool up) = 0;
				virtual void right_mouse(bool up) = 0;
			};
		}
	}
}
