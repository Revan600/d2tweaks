#include <diablo2/fog.h>
#include <common/ptr_wrapper.h>

char* diablo2::fog::get_base() {
	static auto base = reinterpret_cast<char*>(GetModuleHandle("fog.dll"));
	return base;
}

void diablo2::fog::get_save_path(char* buffer, size_t bufferSize) {
	static wrap_func_fast_import<void(char*, size_t)> get_save_path(10115, get_base());
	return get_save_path(buffer, bufferSize);
}

bool diablo2::fog::mpq_open_file(char* path, structures::file_handle** outHandle) {
	static wrap_func_fast_import<BOOL(char*, structures::file_handle**)> mpq_open_file(10102, get_base());
	return mpq_open_file(path, outHandle);
}

bool diablo2::fog::mpq_close_file(structures::file_handle* handle) {
	static wrap_func_fast_import<BOOL(structures::file_handle*)> mpq_close_file(10103, get_base());
	return mpq_close_file(handle);
}

bool diablo2::fog::mpq_read_file(structures::file_handle* handle, void* buffer, size_t size, size_t* bytesToRead) {
	static wrap_func_fast_import<BOOL(structures::file_handle*, void*, size_t, size_t*, int32_t, int32_t, int32_t)> mpq_read_fiile(10104, get_base());
	return mpq_read_fiile(handle, buffer, size, bytesToRead, 0, 0, 0);
}

size_t diablo2::fog::mpq_get_file_size(structures::file_handle* handle, size_t* compressedSize) {
	static wrap_func_fast_import<size_t(structures::file_handle*, size_t*)> mpq_get_file_size(10105, get_base());
	return mpq_get_file_size(handle, compressedSize);
}
