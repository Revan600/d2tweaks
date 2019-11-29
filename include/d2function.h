#pragma once

#include <cstdint>
#include <Windows.h>

namespace details {
	template<typename TRet, typename... Args>
	class d2_func_std_ {
		void* m_function_ptr;
	public:
		explicit d2_func_std_(void* ptr, void* base) {
			m_function_ptr = static_cast<char*>(base) + reinterpret_cast<uintptr_t>(ptr);
		}

		TRet operator()(Args... args) {
			return reinterpret_cast<TRet(__stdcall*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class d2_func_std_import_ {
		void* m_base;
		void* m_function_ptr;
		uint32_t m_ordinal;
	public:
		explicit d2_func_std_import_(uint32_t ordinal, void* base) {
			m_base = base;
			m_function_ptr = nullptr;
			m_ordinal = ordinal;
		}

		TRet operator()(Args... args) {
			if (!m_function_ptr) {
				m_function_ptr = reinterpret_cast<decltype(m_function_ptr)>(GetProcAddress(reinterpret_cast<HMODULE>(m_base),
																						   reinterpret_cast<LPCSTR>(m_ordinal)));
			}

			return reinterpret_cast<TRet(__stdcall*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class d2_func_fast_ {
		void* m_function_ptr;
	public:
		explicit d2_func_fast_(void* ptr, void* base) {
			m_function_ptr = static_cast<char*>(base) + reinterpret_cast<uintptr_t>(ptr);
		}

		TRet operator()(Args... args) {
			return reinterpret_cast<TRet(__fastcall*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class d2_func_this_ {
		void* m_function_ptr;
	public:
		explicit d2_func_this_(void* ptr, void* base) {
			m_function_ptr = static_cast<char*>(base) + reinterpret_cast<uintptr_t>(ptr);
		}

		TRet operator()(Args... args) {
			return reinterpret_cast<TRet(__thiscall*)(Args...)>(m_function_ptr)(args...);
		}
	};
}

template<typename TRet>
class d2_func_std {};

template<typename TRet, typename... Args>
class d2_func_std<TRet(Args...)> : public details::d2_func_std_<TRet, Args...> {
public:
	d2_func_std(void* ptr, void* base) : d2_func_std_(ptr, base) {};
};

template<typename TRet>
class d2_func_std_import {};

template<typename TRet, typename... Args>
class d2_func_std_import<TRet(Args...)> : public details::d2_func_std_import_<TRet, Args...> {
public:
	d2_func_std_import(uint32_t ordinal, void* base) : d2_func_std_import_(ordinal, base) {};
};

template<typename TRet>
class d2_func_fast {};

template<typename TRet, typename... Args>
class d2_func_fast<TRet(Args...)> : public details::d2_func_fast_<TRet, Args...> {
public:
	d2_func_fast(void* ptr, void* base) : d2_func_fast_(ptr, base) {};
};

template<typename TRet>
class d2_func_this {};

template<typename TRet, typename... Args>
class d2_func_this<TRet(Args...)> : public details::d2_func_this_<TRet, Args...> {
public:
	d2_func_this(void* ptr, void* base) : d2_func_this_(ptr, base) {};
};