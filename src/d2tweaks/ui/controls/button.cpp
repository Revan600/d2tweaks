#include <d2tweaks/ui/controls/button.h>

#include <d2tweaks/ui/controls/image.h>

#include <diablo2/d2client.h>
#include <diablo2/d2win.h>

d2_tweaks::ui::controls::button::button(const rect& rect, const std::function<void()>& onClick,
                                        common::asset* image,
										int32_t frameDown, int32_t frameUp, int32_t clickSound) {
	control::set_enabled(true);
	control::set_visible(true);

	m_rect = rect;
	m_image = new controls::image(image, m_rect.get_x(), m_rect.get_y(), frameUp);
	m_frame_down = frameDown;
	m_frame_up = frameUp;
	m_click_sound = clickSound;

	m_current_frame = m_frame_up;

	m_on_click = onClick;
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
	if (m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y()) && !m_popup.empty()) {
		diablo2::d2_win::set_current_font(diablo2::UI_FONT_16);
		diablo2::d2_win::set_popup_properties(const_cast<wchar_t*>(m_popup.c_str()),
											  get_x() + m_rect.get_width() / 2, get_y() - m_rect.get_height(),
											  diablo2::UI_COLOR_WHITE, TRUE);
		diablo2::d2_win::draw_popup();
	}

	m_image->set_frame(m_current_frame);
	m_image->draw();
}

void d2_tweaks::ui::controls::button::draw(int32_t offsetX, int32_t offsetY) {
	m_image->set_frame(m_current_frame);
	m_image->draw(offsetX, offsetY);
}

void d2_tweaks::ui::controls::button::left_mouse(bool up) {
	if (!m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y()))
		return;

	if (up) {
		m_current_frame = m_frame_up;

		if (m_on_click)
			m_on_click();

		return;
	}

	m_current_frame = m_frame_down;

	if (m_click_sound < 0)
		return;

	diablo2::d2_client::play_sound(m_click_sound, nullptr, 0, FALSE, 0);
}

void d2_tweaks::ui::controls::button::right_mouse(bool up) {}
