#pragma once

#include <string>

namespace d2_tweaks {
	namespace common {
		enum mpq_file_type_t;

		class asset final {
			std::string m_path;
			void* m_asset;
			mpq_file_type_t m_type;
		public:
			explicit asset(const std::string& path, void* asset, mpq_file_type_t type) : m_path(path), m_asset(asset), m_type(type) {}

			const std::string& get_path() const {
				return m_path;
			}

			void* get() const {
				return m_asset;
			}

			mpq_file_type_t get_type() const {
				return m_type;
			}

			void update(void* asset) {
				m_asset = asset;
			}
		};
	}
}