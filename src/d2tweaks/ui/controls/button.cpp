#include <d2tweaks/ui/controls/button.h>

#include <d2tweaks/ui/controls/image.h>

#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <spdlog/spdlog.h>
#include <d2tweaks/common/asset_manager.h>
#include <common/string_utils.h>

d2_tweaks::ui::controls::button::button(const rect& rect, const std::function<void()>& onClick,
										common::asset* image,
										int32_t frameDown, int32_t frameUp, int32_t clickSound) : control(rect.get_x(),
																										  rect.get_y(),
																										  rect.get_width(),
																										  rect.get_height()) {
	control::set_enabled(true);
	control::set_visible(true);

	set_x(rect.get_x());
	set_y(rect.get_y());

	m_rect = rect;
	m_image = new controls::image(image, m_rect.get_x(), m_rect.get_y(), frameUp);
	m_frame_down = frameDown;
	m_frame_up = frameUp;
	m_click_sound = clickSound;

	m_is_down = false;
	m_current_frame = m_frame_up;

	m_on_click = onClick;
}

d2_tweaks::ui::controls::button::button(const pugi::xml_node& node) : control(0, 0, 0, 0) {
	const auto cname = node.attribute("name").as_string();
	const auto cx = node.attribute("x").as_int(0);
	const auto cy = node.attribute("y").as_int(0);
	const auto cw = node.attribute("width").as_int(0);
	const auto ch = node.attribute("height").as_int(0);

	const auto cImgPath = node.attribute("image").as_string(nullptr);
	const auto cimg = singleton<common::asset_manager>::instance().get_mpq_file(
		const_cast<char*>(cImgPath), common::MPQ_FILE_TYPE_DC6);

	if (!cimg) {
		spdlog::critical("Cannot load {0} image for {1} control!", cImgPath, cname);
		exit(-1);
	}

	const auto frameDown = node.attribute("frameDown").as_int();
	const auto frameUp = node.attribute("frameUp").as_int();
	const auto clickSound = node.attribute("clickSound").as_int(-1);
	const auto popup = node.attribute("popup").as_string();

	control::set_enabled(true);
	control::set_visible(true);

	m_rect = rect(cx, cy, cw, ch);
	m_image = new image(cimg, m_rect.get_x(), m_rect.get_y(), frameUp);
	m_frame_down = frameDown;
	m_frame_up = frameUp;
	m_click_sound = clickSound;

	m_is_down = false;
	m_current_frame = m_frame_up;

	set_x(cx);
	set_y(cy);
	control::set_width(cw);
	control::set_height(ch);

	set_name(cname);
	set_popup(string_utils::string_to_wstring(popup));
}

d2_tweaks::ui::controls::button::~button() {
	delete m_image;
}

void d2_tweaks::ui::controls::button::set_x(int32_t value) {
	m_rect.set_x(value);
	m_image->set_x(value);
	control::set_x(value);
}

void d2_tweaks::ui::controls::button::set_y(int32_t value) {
	m_rect.set_y(value);
	m_image->set_y(value);
	control::set_y(value);
}

void d2_tweaks::ui::controls::button::draw() {
	draw(0, 0);
}

void d2_tweaks::ui::controls::button::draw(int32_t offsetX, int32_t offsetY) {
	if (m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY) &&
		!m_popup.empty()) {
		diablo2::d2_win::set_current_font(diablo2::UI_FONT_16);
		diablo2::d2_win::set_popup_properties(const_cast<wchar_t*>(m_popup.c_str()),
											  get_x() + offsetX + m_rect.get_width() / 2,
											  get_y() + offsetY - m_rect.get_height(),
											  diablo2::UI_COLOR_WHITE, TRUE);
		diablo2::d2_win::draw_popup();
	}

	m_image->set_frame(m_current_frame);
	m_image->draw(offsetX, offsetY);
}

void d2_tweaks::ui::controls::button::left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
	if (up && m_is_down) {
		block = true;

		m_is_down = false;
		m_current_frame = m_frame_up;

		if (!m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY))
			return;

		if (m_on_click)
			m_on_click();

		return;
	}

	if (!m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY))
		return;

	block = true;

	m_is_down = true;
	m_current_frame = m_frame_down;

	if (m_click_sound < 0)
		return;

	diablo2::d2_client::play_sound(m_click_sound, nullptr, 0, FALSE, 0);
}

void d2_tweaks::ui::controls::button::right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {}

void d2_tweaks::ui::controls::button::key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) {}
