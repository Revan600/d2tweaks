#include <d2tweaks/ui/controls/checkbox.h>
#include <d2tweaks/ui/controls/image.h>
#include <d2tweaks/ui/controls/label.h>
#include <common/string_utils.h>
#include <d2tweaks/common/asset_manager.h>
#include <spdlog/spdlog.h>
#include <diablo2/d2client.h>

const int32_t PADDING = 3;

d2_tweaks::ui::controls::checkbox::checkbox(const std::wstring& text, const rect& rect,
											const std::function<void()>& onClick,
											common::asset* image, int32_t frameChecked, int32_t frameUnchecked, int32_t clickSound)
	: control(rect.get_x(),
			  rect.get_y(),
			  rect.get_width(),
			  rect.get_height()) {
	control::set_enabled(true);
	control::set_visible(true);

	checkbox::set_x(rect.get_x());
	checkbox::set_y(rect.get_y());

	m_rect = rect;

	m_image = new controls::image(image, rect.get_x(), rect.get_y(), frameUnchecked);
	m_label = new label(text, rect.get_x() + m_image->get_width() + PADDING, m_rect.get_y());

	m_frame_checked = frameChecked;
	m_frame_unchecked = frameUnchecked;

	m_click_sound = clickSound;

	m_is_down = false;
	m_state = false;
}

d2_tweaks::ui::controls::checkbox::checkbox(const pugi::xml_node& node) : control(0, 0, 0, 0) {
	control::set_enabled(true);
	control::set_visible(true);

	const auto cname = node.attribute("name").as_string();
	const auto cx = node.attribute("x").as_int(0);
	const auto cy = node.attribute("y").as_int(0);

	const auto frameChecked = node.attribute("frameChecked").as_int();
	const auto frameUnchecked = node.attribute("frameUnchecked").as_int();
	const auto clickSound = node.attribute("clickSound").as_int(-1);
	const auto popup = node.attribute("popup").as_string();

	const auto ctext = node.attribute("text").as_string();
	const auto ccolor = node.attribute("color").as_int();
	const auto cfont = node.attribute("font").as_int(1);

	const auto cImgPath = node.attribute("image").as_string(nullptr);
	const auto cimg = singleton<common::asset_manager>::instance().get_mpq_file(
		const_cast<char*>(cImgPath), common::MPQ_FILE_TYPE_DC6);

	if (!cimg) {
		spdlog::critical("Cannot load {0} image for {1} control!", cImgPath, cname);
		exit(-1);
	}

	m_frame_checked = frameChecked;
	m_frame_unchecked = frameUnchecked;
	m_click_sound = clickSound;
	m_popup = string_utils::string_to_wstring(popup);

	m_is_down = false;
	m_state = false;

	m_image = new image(cimg, cx, cy, m_frame_unchecked);
	m_label = new label(string_utils::string_to_wstring(ctext), cx + m_image->get_width() + PADDING, cy,
						static_cast<diablo2::ui_color_t>(ccolor), static_cast<diablo2::ui_font_t>(cfont));

	checkbox::set_x(cx);
	checkbox::set_y(cy);

	set_name(cname);
}

void d2_tweaks::ui::controls::checkbox::set_x(int32_t value) {
	m_rect.set_x(value);
	m_image->set_x(value);
	m_label->set_x(value + m_image->get_width() + PADDING);
	control::set_x(value);
}

void d2_tweaks::ui::controls::checkbox::set_y(int32_t value) {
	m_rect.set_y(value);
	m_image->set_y(value);
	m_label->set_y(value);
	control::set_y(value);
}

void d2_tweaks::ui::controls::checkbox::draw() {
	draw(0, 0);
}

void d2_tweaks::ui::controls::checkbox::draw(int32_t offsetX, int32_t offsetY) {
	if (m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY) &&
		!m_popup.empty()) {
		diablo2::d2_win::set_current_font(diablo2::UI_FONT_16);
		diablo2::d2_win::set_popup_properties(const_cast<wchar_t*>(m_popup.c_str()),
											  get_x() + offsetX + m_rect.get_width() / 2,
											  get_y() + offsetY - m_rect.get_height(),
											  diablo2::UI_COLOR_WHITE, TRUE);
		diablo2::d2_win::draw_popup();
	}

	m_image->set_frame(m_state ? m_frame_checked : m_frame_unchecked);
	m_image->draw(offsetX, offsetY);
	m_label->draw(offsetX, offsetY);
}

void d2_tweaks::ui::controls::checkbox::left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
	if (m_rect.get_width() == 0)
		m_rect.set_width(m_image->get_width() + m_label->get_width() + PADDING);

	if (m_rect.get_height() == 0)
		m_rect.set_height(m_image->get_height());

	if (up && m_is_down) {
		block = true;

		m_is_down = false;

		if (!m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY))
			return;

		m_state = !m_state;

		if (m_on_click)
			m_on_click(m_state);

		return;
	}

	if (!m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY))
		return;

	block = true;

	m_is_down = true;

	if (m_click_sound < 0)
		return;

	diablo2::d2_client::play_sound(m_click_sound, nullptr, 0, FALSE, 0);
}

void d2_tweaks::ui::controls::checkbox::right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {}

void d2_tweaks::ui::controls::checkbox::key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) {}
