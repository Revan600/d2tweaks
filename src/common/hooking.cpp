#include <common/hooking.h>

#include <MinHook.h>
#include <cstdint>
#include <spdlog/spdlog.h>

// Size of each memory block. (= page size of VirtualAlloc)
const size_t MEMORY_BLOCK_SIZE = 0x1000;

static void* allocate_function_stub(void* origin, void* ptr, size_t size) {
	static void* current_stub = nullptr;

	if (!current_stub) {
		current_stub =
			VirtualAlloc(nullptr, MEMORY_BLOCK_SIZE, MEM_COMMIT | MEM_RESERVE,
						 PAGE_EXECUTE_READWRITE);
	}

	if (!current_stub)
		return nullptr;

	const auto code = static_cast<char*>(current_stub);

	DWORD old_protect;
	VirtualProtect(code, size, PAGE_EXECUTE_READWRITE, &old_protect);

	*reinterpret_cast<uint8_t*>(code) = 0xB8;
	*reinterpret_cast<uintptr_t*>(code + 1) = reinterpret_cast<uintptr_t>(ptr);

	*reinterpret_cast<uint16_t*>(code + 5) = 0xE0FF;

	spdlog::debug("Created function stub at {0}", current_stub);

	current_stub = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(current_stub) + size);

	return code;
}

hooking::mh_status_t hooking::details::hook(void* target, void* detour, void** original) {
	return static_cast<mh_status_t>(MH_CreateHook(target, detour, original));
}

intptr_t hooking::get_executable_memory(void* origin, size_t size) {
	const auto stub = reinterpret_cast<intptr_t>(allocate_function_stub(origin, nullptr, size));
	memset(reinterpret_cast<void*>(stub), 0, size);
	return stub;
}

void* hooking::set_call(void* address, void* function, size_t stubSize) {
	const auto stub = reinterpret_cast<int32_t>(allocate_function_stub(address, function, stubSize));

	DWORD old_protect;
	VirtualProtect(address, 5, PAGE_EXECUTE_READWRITE, &old_protect);

	*static_cast<unsigned char*>(address) = 0xE8;
	*reinterpret_cast<int32_t*>(static_cast<char*>(address) + 1) = stub - reinterpret_cast<int32_t>(address) - 5;

	VirtualProtect(address, 5, old_protect, &old_protect);

	assert(get_call(address) == reinterpret_cast<void*>(stub));

	return reinterpret_cast<void*>(stub);
}

void* hooking::set_jmp(void* address, void* function, size_t stubSize) {
	const auto stub = reinterpret_cast<int32_t>(allocate_function_stub(address, function, stubSize));

	DWORD old_protect;
	VirtualProtect(address, 5, PAGE_EXECUTE_READWRITE, &old_protect);

	*static_cast<unsigned char*>(address) = 0xE9;
	*reinterpret_cast<int32_t*>(static_cast<char*>(address) + 1) = stub - reinterpret_cast<int32_t>(address) - 5;

	VirtualProtect(address, 5, old_protect, &old_protect);

	assert(get_call(address) == reinterpret_cast<void*>(stub));

	return reinterpret_cast<void*>(stub);
}

void* hooking::get_call(void* address) {
	auto target = *reinterpret_cast<int32_t*>(static_cast<char*>(address) + 1);
	target += reinterpret_cast<intptr_t>(address) + 5;

	return reinterpret_cast<void*>(target);
}
