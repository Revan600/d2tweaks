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

			std::string m_name;

			int32_t m_x;
			int32_t m_y;
			int32_t m_width;
			int32_t m_height;

			std::vector<controls::control*> m_controls;
			std::unordered_map<std::string, controls::control*> m_named_controls;
		public:
			virtual ~menu() = default;

			menu();

			const std::vector<controls::control*>& get_controls() const {
				return m_controls;
			}

			const std::string& get_name() const {
				return m_name;
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

			template<typename TControl = controls::control>
			TControl* get_control(const std::string& name) {
				const auto it = m_named_controls.find(name);

				if (it == m_named_controls.end())
					return nullptr;

				return static_cast<TControl*>(it->second);
			}

			virtual void add_control(controls::control* control);
			//virtual controls::control* get_control(const std::string& name);
			virtual void remove_control(controls::control* control);

			virtual void draw();

			virtual bool left_mouse(bool up);
			virtual bool right_mouse(bool up);

			virtual bool key_event(uint32_t key, bool up);
		};
	}
}
