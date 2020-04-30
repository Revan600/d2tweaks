#pragma once

#include <cstdint>
#include <vector>
#include <optional>

namespace details {
	class asm_address {
		intptr_t m_full_offset = 0;
	public:
		virtual ~asm_address() = default;

		intptr_t get_full_offset() const {
			return m_full_offset;
		}

		void set_full_offset(const intptr_t full_offset) {
			m_full_offset = full_offset;
		}

		virtual void build(unsigned char* code) = 0;

		virtual intptr_t get_offset() const = 0;
		virtual intptr_t get() const = 0;
	};
}

class asm_address_static final : public details::asm_address {
	intptr_t m_offset;
	intptr_t m_address;
public:
	/**
	 * \brief
	 * \param offset relative to current instruction
	 * \param address
	 */
	asm_address_static(intptr_t offset, void* address) {
		m_offset = offset;
		m_address = reinterpret_cast<intptr_t>(address);
	}

	void build(unsigned char* code) override {
		*reinterpret_cast<intptr_t*>(code + get_full_offset() + m_offset) = m_address;
	}

	intptr_t get_offset() const override {
		return m_offset;
	}

	intptr_t get() const override {
		return m_address;
	}
};

class asm_address_relative : public details::asm_address {
	intptr_t m_offset;
	intptr_t m_instruction_size;
	intptr_t m_to_address;

	intptr_t m_address;
public:
	/**
	 * \brief
	 * \param offset relative to current instruction
	 * \param instructionSize full size of instruction
	 * \param toAddress
	 */
	asm_address_relative(intptr_t offset, size_t instructionSize, void* toAddress) {
		m_offset = offset;
		m_instruction_size = instructionSize;
		m_to_address = reinterpret_cast<decltype(m_to_address)>(toAddress);

		m_address = 0;
	}

	void build(unsigned char* code) override {
		const auto fromAddress = reinterpret_cast<intptr_t>(code + get_full_offset());
		*reinterpret_cast<intptr_t*>(code + get_full_offset() + m_offset) =
			m_to_address - fromAddress - m_instruction_size;
	}

	intptr_t get_offset() const override {
		return m_offset;
	}

	intptr_t get() const override {
		return m_address;
	}
};

class asm_code {
	const size_t m_growth_factor = 2;

	unsigned char* m_code;
	unsigned char* m_buffer;
	size_t m_buffer_size;
	intptr_t m_offset;

	std::vector<details::asm_address*> m_addresses;
public:
	explicit asm_code(const size_t initSize = 0) {
		m_code = nullptr;
		m_buffer = nullptr;
		m_buffer_size = initSize;
		m_offset = 0;

		if (initSize != 0)
			m_buffer = new unsigned char[initSize]();
	}

	~asm_code() {
		delete[] m_buffer;

		for (auto addr : m_addresses)
			delete addr;
	}

	template<size_t Len>
	void add(int const(&code)[Len], details::asm_address* address = nullptr) {
		if (m_code)
			return;

		if (Len == 0)
			return;

		ensure_buffer_size(Len);

		const auto startOffset = m_offset;

		for (size_t i = 0; i < Len; i++)
			m_buffer[m_offset++] = static_cast<unsigned char>(code[i]);

		if (!address)
			return;

		//scratch some space in buffer for address
		ensure_buffer_size(Len + sizeof(intptr_t));
		m_offset += sizeof(intptr_t);

		address->set_full_offset(startOffset);
		m_addresses.push_back(address);
	}

	size_t get_code_size() const {
		return m_offset;
	}

	unsigned char* get_code() {
		if (!m_code)
			build();

		return m_code;
	}

	void build();
private:
	void ensure_buffer_size(size_t size) {
		if (m_buffer_size - m_offset > size)
			return;

		auto newSize = m_buffer_size;

		if (newSize == 0)
			newSize = 1;

		do {
			newSize *= m_growth_factor;
		} while (size > newSize);

		const auto newBuffer = new unsigned char[newSize]();
		memcpy(newBuffer, m_buffer, m_buffer_size);

		delete[] m_buffer;

		m_buffer_size = newSize;
		m_buffer = newBuffer;
	}
};