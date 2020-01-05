#include <d2tweaks/client/modules/small_patches/small_patches.h>

#include <Windows.h>

#include <diablo2/d2client.h>

MODULE_INIT(small_patches)

void d2_tweaks::client::modules::small_patches::init() {
	//FPS unlock
	DWORD oldProtect;
	if (VirtualProtect(diablo2::d2_client::get_base() + 0xA2C9, 0x04, PAGE_EXECUTE_READWRITE, &oldProtect))
		*reinterpret_cast<uint32_t*>(diablo2::d2_client::get_base() + 0xA2C9) = 0x90909090;
}
