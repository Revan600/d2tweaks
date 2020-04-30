#pragma once

template<typename TEnum>
class enum_helper {
public:
	static constexpr const char* to_string(TEnum value) {
		return __FUNCSIG__;
	}
};