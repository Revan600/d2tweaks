#pragma once

#include <cstdint>
#include <Windows.h>

namespace details {
	template<typename TRet, typename... Args>
	class wrap_func_cdecl_ {
		void* m_function_ptr;
	public:
		explicit wrap_func_cdecl_(uintptr_t ptr, void* base) {
			m_function_ptr = static_cast<char*>(base) + ptr;
		}

		TRet operator()(Args... args) {
			return reinterpret_cast<TRet(__cdecl*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class wrap_func_std_ {
		void* m_function_ptr;
	public:
		explicit wrap_func_std_(uintptr_t ptr, void* base) {
			m_function_ptr = static_cast<char*>(base) + ptr;
		}

		TRet operator()(Args... args) {
			return reinterpret_cast<TRet(__stdcall*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class wrap_func_cdecl_import_ {
		void* m_base;
		void* m_function_ptr;
		uint32_t m_ordinal;
	public:
		explicit wrap_func_cdecl_import_(uint32_t ordinal, void* base) {
			m_base = base;
			m_function_ptr = nullptr;
			m_ordinal = ordinal;
		}

		TRet operator()(Args... args) {
			if (!m_function_ptr) {
				m_function_ptr = reinterpret_cast<decltype(m_function_ptr)>(GetProcAddress(reinterpret_cast<HMODULE>(m_base),
																						   reinterpret_cast<LPCSTR>(m_ordinal)));
			}

			return reinterpret_cast<TRet(__cdecl*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class wrap_func_std_import_ {
		void* m_base;
		void* m_function_ptr;
		uint32_t m_ordinal;
	public:
		explicit wrap_func_std_import_(uint32_t ordinal, void* base) {
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
	class wrap_func_fast_import_ {
		void* m_base;
		void* m_function_ptr;
		uint32_t m_ordinal;
	public:
		explicit wrap_func_fast_import_(uint32_t ordinal, void* base) {
			m_base = base;
			m_function_ptr = nullptr;
			m_ordinal = ordinal;
		}

		TRet operator()(Args... args) {
			if (!m_function_ptr) {
				m_function_ptr = reinterpret_cast<decltype(m_function_ptr)>(GetProcAddress(reinterpret_cast<HMODULE>(m_base),
																						   reinterpret_cast<LPCSTR>(m_ordinal)));
			}

			return reinterpret_cast<TRet(__fastcall*)(Args...)>(m_function_ptr)(args...);
		}
	};

	template<typename TRet, typename... Args>
	class wrap_func_fast_ {
		void* m_function_ptr;
	public:
		explicit wrap_func_fast_(uintptr_t ptr, void* base) {
			m_function_ptr = static_cast<char*>(base) + ptr;
		}

		TRet operator()(Args... args) {
			return reinterpret_cast<TRet(__fastcall*)(Args...)>(m_function_ptr)(args...);
		}
	};
}

template<typename TRet>
class wrap_func_cdecl {};

template<typename TRet, typename... Args>
class wrap_func_cdecl<TRet(Args...)> : public details::wrap_func_cdecl_<TRet, Args...> {
public:
	wrap_func_cdecl(uintptr_t ptr, void* base) : wrap_func_cdecl_(ptr, base) {};
};

template<typename TRet>
class wrap_func_cdecl_import {};

template<typename TRet, typename... Args>
class wrap_func_cdecl_import<TRet(Args...)> : public details::wrap_func_cdecl_import_<TRet, Args...> {
public:
	wrap_func_cdecl_import(uint32_t ordinal, void* base) : wrap_func_cdecl_import_(ordinal, base) {};
};

template<typename TRet>
class wrap_func_std {};

template<typename TRet, typename... Args>
class wrap_func_std<TRet(Args...)> : public details::wrap_func_std_<TRet, Args...> {
public:
	wrap_func_std(uintptr_t ptr, void* base) : wrap_func_std_(ptr, base) {};
};

template<typename TRet>
class wrap_func_std_import {};

template<typename TRet, typename... Args>
class wrap_func_std_import<TRet(Args...)> : public details::wrap_func_std_import_<TRet, Args...> {
public:
	wrap_func_std_import(uint32_t ordinal, void* base) : wrap_func_std_import_(ordinal, base) {};
};

template<typename TRet>
class wrap_func_fast_import {};

template<typename TRet, typename... Args>
class wrap_func_fast_import<TRet(Args...)> : public details::wrap_func_fast_import_<TRet, Args...> {
public:
	wrap_func_fast_import(uint32_t ordinal, void* base) : wrap_func_fast_import_(ordinal, base) {};
};

template<typename TRet>
class wrap_func_fast {};

template<typename TRet, typename... Args>
class wrap_func_fast<TRet(Args...)> : public details::wrap_func_fast_<TRet, Args...> {
public:
	wrap_func_fast(uintptr_t ptr, void* base) : wrap_func_fast_(ptr, base) {};
};

template<typename TType>
class wrap_value {
	TType* m_value;
public:
	wrap_value(uintptr_t ptr, void* base) {
		// Fuck off, I'm gonna cast it in C-style because of templates
		// ReSharper disable once CppCStyleCast
		m_value = (TType*)(static_cast<char*>(base) + ptr);
	}

	operator void* () const {
		return m_value;
	}

	operator TType* () const {
		return m_value;
	}
};