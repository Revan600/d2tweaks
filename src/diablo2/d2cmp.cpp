#include <diablo2/d2cmp.h>

#include <common/ptr_wrapper.h>

#include <Windows.h>

char* diablo2::structures::d2_cmp::get_base() {
	static const auto base = reinterpret_cast<char*>(GetModuleHandle("d2cmp.dll"));
	return base;
}

bool diablo2::structures::d2_cmp::init_gfx_data(gfxdata* gfxdata) {
	static wrap_func_std_import<BOOL(structures::gfxdata*, int32_t, int32_t)> init_gfx_data(10055, get_base());
	return init_gfx_data(gfxdata, 0, 1) > 0;
}
