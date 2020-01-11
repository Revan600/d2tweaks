#pragma once

#include <cstdint>

namespace d2_tweaks {
	namespace ui {
		//Special ui rectangle with starting point in the left bottom corner
		class rect {
			int32_t m_x, m_y, m_w, m_h;
		public:
			rect() : m_x(0), m_y(0), m_w(0), m_h(0) {}
			rect(int32_t x, int32_t y, int32_t width, int32_t height) : m_x(x), m_y(y), m_w(width), m_h(height) {}

			int32_t get_x() const {
				return m_x;
			}

			void set_x(int32_t value) {
				m_x = value;
			}

			int32_t get_y() const {
				return m_y;
			}

			void set_y(int32_t value) {
				m_y = value;
			}

			void set_width(int32_t value) {
				m_w = value;
			}

			int32_t get_width() const {
				return m_w;
			}

			void set_height(int32_t value) {
				m_h = value;
			}

			int32_t get_height() const {
				return m_h;
			}

			bool contains(int32_t x, int32_t y) const {
				return x >= m_x && x < m_x + m_w &&
					y > m_y - m_h && y <= m_y;
			}

			bool contains(int32_t x, int32_t y, int32_t offsetX, int32_t offsetY) const {
				return x >= m_x + offsetX && x < m_x + offsetX + m_w &&
					y > m_y + offsetY - m_h && y <= m_y + offsetY;
			}
		};
	}
}