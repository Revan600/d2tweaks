#include <common/hooking.h>

#include <MinHook.h>

hooking::mh_status_t hooking::hook(void* target, void* detour, void** original) {
	return static_cast<mh_status_t>(MH_CreateHook(target, detour, original));
}
