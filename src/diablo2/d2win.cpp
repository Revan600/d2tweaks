#include <diablo2/d2win.h>
#include <common/ptr_wrapper.h>

char* diablo2::d2_win::get_base() {
	static const auto base = reinterpret_cast<char*>(GetModuleHandle("d2win.dll"));
	return base;
}

int32_t diablo2::d2_win::get_text_pixel_width(wchar_t* str) {
	static wrap_func_fast_import<int32_t(wchar_t*)> get_text_pixel_width(10121, get_base());
	return get_text_pixel_width(str);
}

void diablo2::d2_win::draw_text(wchar_t* str, uint32_t x, uint32_t y, ui_color_t color, int32_t transTbl) {
	static wrap_func_fast_import<void(wchar_t*, uint32_t, uint32_t, int32_t, int32_t)> draw_text(10117, get_base());
	draw_text(str, x, y, color, transTbl);
}

void diablo2::d2_win::draw_boxed_text(wchar_t* str, uint32_t x, uint32_t y, int32_t paletteIndex, int32_t transTbl,
									  ui_color_t color) {
	static wrap_func_fast_import<void(wchar_t*, uint32_t, uint32_t, int32_t, int32_t, int32_t)> draw_boxed_text(10132, get_base());
	draw_boxed_text(str, x, y, paletteIndex, transTbl, color);
}

void diablo2::d2_win::set_popup_properties(wchar_t* str, uint32_t x, uint32_t y, ui_color_t color, int32_t align) {
	static wrap_func_fast_import<void(wchar_t*, uint32_t, uint32_t, int32_t, int32_t)> hover_text(10129, get_base());
	hover_text(str, x, y, color, align);
}

void diablo2::d2_win::draw_popup() {
	static wrap_func_cdecl_import<void()> draw_popup(10124, get_base());
	draw_popup();
}

diablo2::ui_font_t diablo2::d2_win::get_current_font() {
	return static_cast<ui_font_t>(*reinterpret_cast<int32_t*>(get_base() + 0x1DB24));
}

int32_t diablo2::d2_win::get_current_font_height() {
	static wrap_func_cdecl_import<int32_t()> get_current_font_height(10125, get_base());
	return get_current_font_height();
}

int32_t diablo2::d2_win::set_current_font(ui_font_t font) {
	static wrap_func_fast_import<int32_t(int32_t)> set_current_font(10127, get_base());
	return set_current_font(font);
}

void* diablo2::d2_win::load_mpq(char* dllName, char* mpqName, char* mpqTitle, int32_t overrideFlags) {
	static wrap_func_fast<void* (char*, char*, char*, int32_t, int32_t, int32_t, int32_t)> load_mpq(0x12399, get_base());
	return load_mpq(dllName, mpqName, mpqTitle, 0, 0, 0, overrideFlags);
}

bool diablo2::d2_win::unload_mpq(void* mpq) {
	static wrap_func_fast<BOOL(void*)> unload_mpq(0x012548, get_base());
	return unload_mpq(mpq);
}
