#include <d2tweaks/ui/controls/image.h>

#include <cstring>

#include <diablo2/d2gfx.h>

#include <d2tweaks/common/asset.h>
#include <d2tweaks/common/asset_manager.h>
#include <spdlog/spdlog.h>
#include <diablo2/structures/cellfile.h>
#include <diablo2/structures/gfxcell.h>
#include <diablo2/d2client.h>

d2_tweaks::ui::controls::image::image(common::asset* image, int32_t x, int32_t y, int32_t frame) : control(x, y, 0, 0) {
	control::set_enabled(true);
	control::set_visible(true);

	m_image = image;
	m_frame = frame;
	auto cellFile = static_cast<diablo2::structures::cellfile*>(m_image->get());
	const auto cell = cellFile->cells[m_frame];
	m_rect = rect(x, y, cell->width + cell->offset_x, cell->height - cell->offset_y);
	m_block_click = false;

	control::set_width(m_rect.get_width());
	control::set_height(m_rect.get_height());
}

d2_tweaks::ui::controls::image::image(const pugi::xml_node& node) : control(0, 0, 0, 0) {
	const auto cname = node.attribute("name").as_string();
	const auto cx = node.attribute("x").as_int(0);
	const auto cy = node.attribute("y").as_int(0);

	const auto cImgPath = node.attribute("image").as_string(nullptr);
	const auto cimg = singleton<common::asset_manager>::instance().get_mpq_file(
		const_cast<char*>(cImgPath), common::MPQ_FILE_TYPE_DC6);

	if (!cimg) {
		spdlog::critical("Cannot load {0} image for {1} control!", cImgPath, cname);
		exit(-1);
	}

	const auto frame = node.attribute("frame").as_int();
	const auto blockClick = node.attribute("blockClick").as_bool(false);

	control::set_enabled(true);
	control::set_visible(true);

	m_image = cimg;
	m_frame = frame;
	auto cellFile = static_cast<diablo2::structures::cellfile*>(m_image->get());
	const auto cell = cellFile->cells[m_frame];
	m_rect = rect(cx, cy, cell->width + cell->offset_x, cell->height - cell->offset_y);
	m_block_click = blockClick;

	control::set_x(cx);
	control::set_y(cy);
	control::set_width(m_rect.get_width());
	control::set_height(m_rect.get_height());

	set_name(cname);
}

void d2_tweaks::ui::controls::image::draw() {
	draw(0, 0);
}

void d2_tweaks::ui::controls::image::draw(int32_t offsetX, int32_t offsetY) {
	if (m_image == nullptr || !m_image->get())
		return;

	//I don't like to call memset for each draw_info every frame, but if we wouldn't clear it - we'll get access violation inside draw_image
	memset(&m_draw_info, 0x00, sizeof m_draw_info);
	m_draw_info.cell_file = static_cast<diablo2::structures::cellfile*>(m_image->get());
	m_draw_info.frame = m_frame;

	diablo2::d2_gfx::draw_image(&m_draw_info, get_x() + offsetX, get_y() + offsetY, -1, 5, nullptr);
}

void d2_tweaks::ui::controls::image::left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
	if (!m_block_click)
		return;

	block = m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY);
}

void d2_tweaks::ui::controls::image::right_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
	if (!m_block_click)
		return;

	block = m_rect.contains(diablo2::d2_client::get_mouse_x(), diablo2::d2_client::get_mouse_y(), offsetX, offsetY);
}

void d2_tweaks::ui::controls::image::key_event(int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) {}
