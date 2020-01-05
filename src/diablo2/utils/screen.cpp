#include <diablo2/utils/screen.h>

#include <diablo2/d2client.h>
#include <diablo2/d2common.h>

void diablo2::utils::screen::screen_to_world(int32_t sx, int32_t sy, int32_t& wx, int32_t& wy) {
	const auto x = sx + d2_client::get_view_offset_x();
	const auto y = sy + d2_client::get_view_offset_y();

	auto tx = 0, ty = 0;

	d2_common::_10116(x, y, &tx, &ty);
	d2_common::_10116(x, y, &tx, &ty);

	wx = tx;
	wy = ty;
}

void diablo2::utils::screen::world_to_screen(int32_t wx, int32_t wy, int32_t& sx, int32_t& sy) {
	auto x = wx;
	auto y = wy;

	d2_common::_10111(&x, &y);

	sx = x - d2_client::get_view_offset_x();
	sy = y - d2_client::get_view_offset_y();
}
