#include <diablo2/d2net.h>
#include <common/ptr_wrapper.h>

char* diablo2::d2_net::get_base() {
	char* base = reinterpret_cast<char*>(GetModuleHandle("d2net.dll"));
	return base;
}

int32_t diablo2::d2_net::send_to_server(int32_t queue, void* data, size_t size) {
	static wrap_func_std_import<int32_t(int32_t, void*, size_t)> send_to_server(10005, get_base());
	return send_to_server(queue, data, size);
}

int32_t diablo2::d2_net::send_to_client(int32_t queue, int32_t clientId, void* packet, size_t size) {
	static wrap_func_std_import<int32_t(int32_t, int32_t, void*, size_t)> send_to_client(10006, get_base());
	return send_to_client(queue, clientId, packet, size);
}
