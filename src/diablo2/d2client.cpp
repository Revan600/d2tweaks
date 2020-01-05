#include <diablo2/d2client.h>

#include <common/ptr_wrapper.h>

char* diablo2::d2_client::get_base() {
	static auto base = reinterpret_cast<char*>(GetModuleHandle("d2client.dll"));
	return base;
}

bool diablo2::d2_client::is_lod() {
	return *reinterpret_cast<int*>(get_base() + 0x1077C4) > 0;
}

diablo2::structures::unit* diablo2::d2_client::get_local_player() {
	static wrap_func_std<structures::unit * ()> get_local_player(0x883D0, get_base());
	return get_local_player();
}

diablo2::structures::client_unit_list* diablo2::d2_client::get_client_unit_list() {
	static auto unit_list = reinterpret_cast<structures::client_unit_list*>(get_base() + 0x11AA00);
	return unit_list;
}

int32_t diablo2::d2_client::get_view_offset_x() {
	static wrap_func_std<int32_t()> get_view_offset_x(0x15890, get_base());
	return get_view_offset_x();
}

int32_t diablo2::d2_client::get_view_offset_y() {
	static wrap_func_std<int32_t()> get_view_offset_y(0x158A0, get_base());
	return get_view_offset_y();
}

uint32_t diablo2::d2_client::get_mouse_x() {
	static wrap_func_std<uint32_t()> get_mouse_x(0xB7BC0, get_base());
	return get_mouse_x();
}

uint32_t diablo2::d2_client::get_mouse_y() {
	static wrap_func_std<uint32_t()> get_mouse_y(0xB7BD0, get_base());
	return get_mouse_y();
}

bool diablo2::d2_client::get_ui_window_state(const ui_window_t window) {
	static auto ui_states = reinterpret_cast<BOOL*>(get_base() + 0x11A6A8);
	return ui_states[window];
}

void* diablo2::d2_client::get_buysellbtn() {
	static wrap_func_cdecl<void* ()> get_buysellbtn(0x84110, get_base());
	return get_buysellbtn();
}

void diablo2::d2_client::play_sound(const uint32_t soundId, structures::unit* u, const uint32_t ticks, const BOOL prePick, const uint32_t cache) {
	static wrap_func_fast<void(uint32_t, structures::unit*, uint32_t, BOOL, uint32_t)>
		play_sound(0xB5820, get_base());

	play_sound(soundId, u, ticks, prePick, cache);
}

diablo2::structures::unit* diablo2::d2_client::get_unit_by_guid(const int32_t type, const int32_t guid) {
	static wrap_func_fast<structures::unit * (int32_t, int32_t)> get_unit_by_guid(0x869F0, get_base());
	return get_unit_by_guid(type, guid);
}

void diablo2::d2_client::send_to_server(void* data, const size_t size) {
	static wrap_func_fast<void(void*, size_t)> send_to_server(0xD850, get_base());
	send_to_server(data, size);
}

bool diablo2::d2_client::cache_gfx_data(structures::gfxdata* gfxData, structures::unit* unit,
										structures::cellfile* cellfFile, int32_t direction, int32_t frame, int32_t* outIndex, int8_t flags,
										int32_t colorTint) {
	static wrap_func_fast<int32_t(structures::gfxdata*,
								  structures::unit*,
								  structures::cellfile*,
								  int32_t, int32_t, int32_t*, int8_t, int32_t)> cache_gfx_data(0xBEC80, get_base());
	return cache_gfx_data(gfxData, unit, cellfFile, direction, frame, outIndex, flags, colorTint);
}

diablo2::structures::cellfile* diablo2::d2_client::load_gfx_resource(char* path) {
	static wrap_func_fast<structures::cellfile * (char*, int32_t)> load_gfx_resource(0x1000, get_base());
	return load_gfx_resource(path, 0);
}

int32_t diablo2::d2_client::unload_gfx_resource(structures::cellfile* handle) {
	static wrap_func_fast<int32_t(structures::cellfile*)> unload_gfx_resource(0x1140, get_base());
	return unload_gfx_resource(handle);
}
