#include <d2tweaks/ui/menu.h>

#include <algorithm>
#include <pugixml.hpp>

#include <diablo2/utils/mpq_ifstream.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/ui/controls/image.h>
#include <d2tweaks/ui/controls/label.h>
#include <d2tweaks/ui/controls/checkbox.h>
#include <d2tweaks/ui/controls/group.h>

d2_tweaks::ui::menu::menu() : m_x(0), m_y(0), m_width(0), m_height(0) {}

bool d2_tweaks::ui::menu::load_xml(const std::string& path) {
	diablo2::utils::mpq_ifstream file(path);

	if (!file.good())
		return false;

	pugi::xml_document doc;
	const auto result = doc.load(file);

	if (!result)
		return false;

	const auto root = doc.select_nodes("/interface/menu");

	if (root.size() == 0)
		return false;

	const auto menuNode = root.first();

	m_name = menuNode.node().attribute("name").as_string();
	m_x = menuNode.node().attribute("x").as_int(0);
	m_y = menuNode.node().attribute("y").as_int(0);
	m_width = menuNode.node().attribute("width").as_int(-1);
	m_height = menuNode.node().attribute("height").as_int(-1);

	for (auto child : menuNode.node().children()) {
		const auto name = child.name();

		if (strcmp(name, "button") == 0) {
			add_control(new controls::button(this, child));
			continue;
		}

		if (strcmp(name, "checkbox") == 0) {
			add_control(new controls::checkbox(this, child));
			continue;
		}

		if (strcmp(name, "group") == 0) {
			add_control(new controls::group(this, child));
			continue;
		}

		if (strcmp(name, "image") == 0) {
			add_control(new controls::image(this, child));
			continue;
		}

		if (strcmp(name, "label") == 0) {
			add_control(new controls::label(this, child));
		}
	}

	if (m_width == -1) {
		for (auto control : m_controls)
			if (control->get_width() > m_width)
				m_width = control->get_width();
	}

	if (m_height == -1) {
		for (auto control : m_controls)
			if (control->get_height() > m_height)
				m_height = control->get_height();
	}

	return true;
}

void d2_tweaks::ui::menu::add_control(controls::control* control) {
	if (control == nullptr)
		return;

	if (control->get_name().empty()) {
		spdlog::error("Cannot add control: empty name!");
		return;
	}

	const auto it = m_named_controls.find(control->get_name());

	if (it != m_named_controls.end())
		return;

	m_named_controls[control->get_name()] = control;
	m_controls.push_back(control);
}

void d2_tweaks::ui::menu::remove_control(controls::control* control) {
	if (control == nullptr)
		return;

	m_controls.erase(std::remove(m_controls.begin(), m_controls.end(), control), m_controls.end());
}

void d2_tweaks::ui::menu::draw() {
	for (auto control : m_controls) {
		if (!control->get_visible() || control->get_parent() != nullptr)
			continue;

		control->draw(m_x, m_y);
	}
}

bool d2_tweaks::ui::menu::left_mouse(bool up) {
	auto block = false;

	for (auto control : m_controls) {
		if (!control->get_enabled() || control->get_parent() != nullptr)
			continue;

		auto tblock = false;
		control->left_mouse(m_x, m_y, up, tblock);
		block |= tblock;
	}

	return block;
}

bool d2_tweaks::ui::menu::right_mouse(bool up) {
	auto block = false;

	for (auto control : m_controls) {
		if (!control->get_enabled() || control->get_parent() != nullptr)
			continue;

		auto tblock = false;
		control->right_mouse(m_x, m_y, up, tblock);
		block |= tblock;
	}

	return block;
}

bool d2_tweaks::ui::menu::key_event(uint32_t key, bool up) {
	auto block = false;

	for (auto control : m_controls) {
		if (!control->get_enabled() || control->get_parent() != nullptr)
			continue;

		auto tblock = false;
		control->key_event(m_x, m_y, key, up, tblock);
		block |= tblock;
	}

	return block;
}
