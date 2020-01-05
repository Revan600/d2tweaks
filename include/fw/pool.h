#pragma once

#include <cstdint>
#include <queue>

template<typename T>
class growing_object_pool {
	std::queue<T*> m_objects;
	uint32_t m_objects_count;
	T* (*m_factory)();
public:
	explicit growing_object_pool(T* (*factory)(), uint32_t initialSize = 0) {
		m_factory = factory;

		m_objects_count = initialSize;
		for (size_t i = 0; i < initialSize; i++) {
			m_objects.push(m_factory());
		}
	}

	uint32_t get_count() const {
		return m_objects_count;
	}

	T* get() {
		if (m_objects.empty()) {
			m_objects.push(m_factory());
			m_objects_count++;
		}

		auto result = m_objects.front();

		m_objects.pop();

		return result;
	}

	void put(T* obj) {
		m_objects.push(obj);
	}
};