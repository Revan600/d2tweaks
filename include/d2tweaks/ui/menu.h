#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class control;
		}

		class menu {
			bool m_enabled = false;
			bool m_visible = false;

			std::vector<controls::control*> m_controls;
			std::unordered_map<std::string, controls::control*> m_named_controls;
		public:
			virtual ~menu() = default;

			menu();

			const std::vector<controls::control*>& get_controls() const {
				return m_controls;
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

			bool load_xml(const std::string& path);

			virtual void add_control(controls::control* control);
			virtual controls::control* get_control(const std::string& name);
			virtual void remove_control(controls::control* control);

			virtual void draw();

			virtual void left_mouse(bool up);
			virtual void right_mouse(bool up);
		};
	}
}
