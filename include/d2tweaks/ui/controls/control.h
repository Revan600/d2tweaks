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
				int32_t m_x;
				int32_t m_y;
				int32_t m_width;
				int32_t m_height;
			public:
				control(int32_t x, int32_t y, int32_t w, int32_t h) : m_x(x), m_y(y), m_width(w), m_height(h) {}

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

				virtual int32_t get_width() const {
					return m_width;
				}

				virtual void set_width(const int32_t width) {
					m_width = width;
				}

				virtual int32_t get_height() const {
					return m_height;
				}

				virtual void set_height(const int32_t height) {
					m_height = height;
				}

				virtual void draw() = 0;
				virtual void draw(int32_t offsetX, int32_t offsetY) = 0;

				virtual void left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) = 0;
				virtual void right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) = 0;

				virtual void key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) = 0;
			};
		}
	}
}
