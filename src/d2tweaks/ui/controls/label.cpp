#include <d2tweaks/ui/controls/label.h>
#include <common/string_utils.h>

d2_tweaks::ui::controls::label::label(menu* menu, const std::wstring& text, int32_t x, int32_t y, diablo2::ui_color_t color,
									  diablo2::ui_font_t font) : control(menu, x, y, 0, 0) {
	control::set_enabled(true);
	control::set_visible(true);

	m_text = text;
	m_text_owned = false;
	m_color = color;
	m_font = font;

	control::set_height(0);
	control::set_width(0);
}

d2_tweaks::ui::controls::label::label(menu* menu, const pugi::xml_node& node) : control(menu, 0, 0, 0, 0) {
	const auto cname = node.attribute("name").as_string();
	const auto cx = node.attribute("x").as_int(0);
	const auto cy = node.attribute("y").as_int(0);
	const auto ctext = node.attribute("text").as_string();
	const auto ccolor = node.attribute("color").as_int();
	const auto cfont = node.attribute("font").as_int(1);

	control::set_enabled(true);
	control::set_visible(true);

	m_text_owned = true;
	m_text = string_utils::string_to_wstring(ctext);
	m_color = static_cast<diablo2::ui_color_t>(ccolor);
	m_font = static_cast<diablo2::ui_font_t>(cfont);

	control::set_x(cx);
	control::set_y(cy);

	control::set_height(0);
	control::set_width(0);

	set_name(cname);
}

void d2_tweaks::ui::controls::label::draw() {
	draw(0, 0);
}

void d2_tweaks::ui::controls::label::draw(int32_t offsetX, int32_t offsetY) {
	const auto font = diablo2::d2_win::get_current_font();
	diablo2::d2_win::set_current_font(m_font);

	if (get_width() == 0) {
		set_width(diablo2::d2_win::get_text_pixel_width(const_cast<wchar_t*>(m_text.c_str())));
	}

	if (get_height() == 0) {
		set_height(diablo2::d2_win::get_current_font_height());
	}

	diablo2::d2_win::draw_text(const_cast<wchar_t*>(m_text.c_str()), get_x() + offsetX, get_y() + offsetY, m_color, 0);
	diablo2::d2_win::set_current_font(font);
}

void d2_tweaks::ui::controls::label::left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {}

void d2_tweaks::ui::controls::label::right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {}

void d2_tweaks::ui::controls::label::key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) {}
