#include <diablo2/d2gfx.h>
#include <common/ptr_wrapper.h>

char* diablo2::d2_gfx::get_base() {
	static char* base = reinterpret_cast<char*>(GetModuleHandle("d2gfx.dll"));
	return base;
}

bool diablo2::d2_gfx::check_perspective_mode() {
	static wrap_func_std_import<bool()> check_perspective_mode(10010, get_base());
	return check_perspective_mode();
}

bool diablo2::d2_gfx::check_perspective_coords(int32_t x, int32_t y) {
	static wrap_func_std_import<bool(int32_t, int32_t)> check_perspective_coords(10065, get_base());
	return check_perspective_coords(x, y);
}

int32_t diablo2::d2_gfx::adjust_perspective_coords(int32_t x, int32_t y, int32_t* adjustX, int32_t* adjustY) {
	static wrap_func_std_import<int32_t(int32_t, int32_t, int32_t, int32_t*, int32_t*)> adjust_perspective_coords(10066, get_base());
	return adjust_perspective_coords(x, y, 0, adjustX, adjustY);
}

void diablo2::d2_gfx::draw_image(structures::gfxdata* data, uint32_t x, uint32_t y, int32_t gamma,
								 int32_t drawType, void* palette) {
	static wrap_func_std_import<void(structures::gfxdata*, uint32_t, uint32_t,
								   int32_t, int32_t, void*)> draw_image(10072, get_base());
	draw_image(data, x, y, gamma, drawType, palette);
}
