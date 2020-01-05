#pragma once

template<typename T>
class singleton {
public:
	static T& instance();

	singleton(const singleton&) = delete;
	singleton& operator= (singleton) = delete;

protected:
	struct token {};
	singleton() = default;
};

#include <memory>
template<typename T>
T& singleton<T>::instance() {
	static const std::unique_ptr<T> instance{ new T{token{}} };
	return *instance;
}