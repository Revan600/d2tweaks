#include <d2tweaks/ui/controls/image.h>

#include <cstring>

#include <diablo2/d2gfx.h>
#include <diablo2/d2client.h>

#include <d2tweaks/common/asset.h>

d2_tweaks::ui::controls::image::image(common::asset* image, int32_t x, int32_t y, int32_t frame) : m_x(x), m_y(y) {
	control::set_enabled(true);
	control::set_visible(true);

	m_image = image;
	m_frame = frame;
}

void d2_tweaks::ui::controls::image::draw() {
	if (m_image == nullptr || !m_image->get())
		return;

	//I don't like to call memset for each draw_info every frame, but if we wouldn't clear it - we'll get access violation inside draw_image
	memset(&m_draw_info, 0x00, sizeof m_draw_info);
	m_draw_info.cell_file = static_cast<diablo2::structures::cellfile*>(m_image->get());
	m_draw_info.frame = m_frame;

	diablo2::d2_gfx::draw_image(&m_draw_info, m_x, m_y, -1, 5, nullptr);
}

void d2_tweaks::ui::controls::image::draw(int32_t offsetX, int32_t offsetY) {
	if (m_image == nullptr || !m_image->get())
		return;

	//I don't like to call memset for each draw_info every frame, but if we wouldn't clear it - we'll get access violation inside draw_image
	memset(&m_draw_info, 0x00, sizeof m_draw_info);
	m_draw_info.cell_file = static_cast<diablo2::structures::cellfile*>(m_image->get());
	m_draw_info.frame = m_frame;

	diablo2::d2_gfx::draw_image(&m_draw_info, m_x + offsetX, m_y + offsetY, -1, 5, nullptr);
}

void d2_tweaks::ui::controls::image::left_mouse(bool up) {}

void d2_tweaks::ui::controls::image::right_mouse(bool up) {}
