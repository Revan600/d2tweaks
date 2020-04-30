#include <d2tweaks/ui/controls/group.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/ui/controls/label.h>
#include <d2tweaks/ui/controls/checkbox.h>
#include <d2tweaks/ui/controls/image.h>
#include <d2tweaks/ui/menu.h>

d2_tweaks::ui::controls::group::group(menu* menu, int32_t x, int32_t y) : control(menu, x, y, 0, 0) {
	control::set_enabled(true);
	control::set_visible(true);
}

d2_tweaks::ui::controls::group::group(menu* menu, const pugi::xml_node& node) : control(menu, 0, 0, 0, 0) {
	control::set_enabled(true);
	control::set_visible(true);

	const auto cname = node.attribute("name").as_string();
	const auto cx = node.attribute("x").as_int(0);
	const auto cy = node.attribute("y").as_int(0);
	const auto cw = node.attribute("width").as_int(0);
	const auto ch = node.attribute("height").as_int(0);

	set_name(cname);

	control::set_x(cx);
	control::set_y(cy);
	control::set_width(cw);
	control::set_height(ch);

	for (auto child : node.children()) {
		const auto name = child.name();

		if (strcmp(name, "button") == 0) {
			add_control(new button(menu, child));
			continue;
		}

		if (strcmp(name, "checkbox") == 0) {
			add_control(new checkbox(menu, child));
			continue;
		}

		if (strcmp(name, "group") == 0) {
			add_control(new group(menu, child));
			continue;
		}

		if (strcmp(name, "image") == 0) {
			add_control(new image(menu, child));
			continue;
		}

		if (strcmp(name, "label") == 0) {
			add_control(new label(menu, child));
		}
	}
}

void d2_tweaks::ui::controls::group::draw() {
	draw(0, 0);
}

void d2_tweaks::ui::controls::group::draw(int32_t offsetX, int32_t offsetY) {
	for (auto control : m_controls) {
		if (!control->get_visible())
			continue;

		control->draw(get_x() + offsetX, get_y() + offsetY);
	}
}

void d2_tweaks::ui::controls::group::left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
	for (auto control : m_controls) {
		if (!control->get_enabled())
			continue;

		control->left_mouse(get_x() + offsetX, get_y() + offsetY, up, block);
	}
}

void d2_tweaks::ui::controls::group::right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
	for (auto control : m_controls) {
		if (!control->get_enabled())
			continue;

		control->right_mouse(get_x() + offsetX, get_y() + offsetY, up, block);
	}
}

void d2_tweaks::ui::controls::group::key_event(int32_t offsetX, int32_t offsetY,
											   uint32_t key, bool up, bool& block) {
	for (auto control : m_controls) {
		if (!control->get_enabled())
			continue;

		control->key_event(get_x() + offsetX, get_y() + offsetY, key, up, block);
	}
}

void d2_tweaks::ui::controls::group::add_control(control* control) {
	control->set_parent(this);
	m_controls.push_back(control);

	if (get_menu() == nullptr)
		return;

	get_menu()->add_control(control);
}
