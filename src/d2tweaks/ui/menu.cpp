#include <d2tweaks/ui/menu.h>

#include <algorithm>
#include <pugixml.hpp>

#include <d2tweaks/ui/controls/control.h>

#include <diablo2/utils/mpq_ifstream.h>
#include <spdlog/spdlog.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/common/asset_manager.h>
#include <codecvt>

static std::wstring string_to_wstring(const std::string& str) {
	if (str.empty())
		return std::wstring();

	const auto sizeNeeded = MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
	std::wstring wstrTo(sizeNeeded, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &wstrTo[0], sizeNeeded);

	return wstrTo;
}

d2_tweaks::ui::menu::menu() {}

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

	auto x = menuNode.node().attribute("x").as_int();
	auto y = menuNode.node().attribute("y").as_int();
	auto width = menuNode.node().attribute("width").as_int();
	auto height = menuNode.node().attribute("height").as_int();

	for (auto node : menuNode.node().children("button")) {
		const auto cname = node.attribute("name").as_string();
		const auto cx = node.attribute("x").as_int();
		const auto cy = node.attribute("y").as_int();
		const auto cw = node.attribute("width").as_int();
		const auto ch = node.attribute("height").as_int();

		const auto cImgPath = node.attribute("image").as_string();
		const auto cimg = singleton<common::asset_manager>::instance().get_mpq_file(const_cast<char*>(cImgPath), common::MPQ_FILE_TYPE_DC6);

		if (!cimg) {
			spdlog::critical("Cannot load {0} image for {1} control!", cImgPath, cname);
			exit(-1);
		}

		const auto frameDown = node.attribute("frameDown").as_int();
		const auto frameUp = node.attribute("frameUp").as_int();
		const auto clickSound = node.attribute("clickSound").as_int(-1);
		const auto popup = node.attribute("popup").as_string();

		auto btn = new controls::button(rect(cx, cy, cw, ch), {}, cimg, frameDown, frameUp, clickSound);
		btn->set_name(cname);
		btn->set_popup(string_to_wstring(popup));
		add_control(btn);
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

d2_tweaks::ui::controls::control* d2_tweaks::ui::menu::get_control(const std::string& name) {
	const auto it = m_named_controls.find(name);

	if (it == m_named_controls.end())
		return nullptr;

	return it->second;
}

void d2_tweaks::ui::menu::remove_control(controls::control* control) {
	if (control == nullptr)
		return;

	m_controls.erase(std::remove(m_controls.begin(), m_controls.end(), control), m_controls.end());
}

void d2_tweaks::ui::menu::draw() {
	for (auto control : m_controls) {
		if (!control->get_visible())
			continue;

		control->draw();
	}
}

void d2_tweaks::ui::menu::left_mouse(bool up) {
	for (auto control : m_controls) {
		if (!control->get_enabled())
			continue;

		control->left_mouse(up);
	}
}

void d2_tweaks::ui::menu::right_mouse(bool up) {
	for (auto control : m_controls) {
		if (!control->get_enabled())
			continue;

		control->right_mouse(up);
	}
}
