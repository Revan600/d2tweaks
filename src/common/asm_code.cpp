#include <common/asm_code.h>
#include <common/hooking.h>

#include <Windows.h>

void asm_code::build() {
	m_code = reinterpret_cast<decltype(m_code)>(hooking::get_executable_memory(GetModuleHandle(nullptr), m_offset));
	memcpy(m_code, m_buffer, m_offset);

	for (auto addr : m_addresses) {
		addr->build(m_code);
	}
}
