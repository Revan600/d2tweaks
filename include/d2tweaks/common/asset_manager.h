#pragma once

#include <fw/singleton.h>

#include <functional>
#include <string>
#include <unordered_map>

namespace d2_tweaks {
	namespace common {
		class asset;

		enum mpq_file_type_t {
			MPQ_FILE_TYPE_UNKNOWN = -1,

			MPQ_FILE_TYPE_DC6 = 0,
			MPQ_FILE_TYPE_DCC = 1,

			MPQ_FILE_TYPE_COUNT
		};

		class asset_manager : public singleton<asset_manager> {
			std::unordered_map<std::string, asset*> m_assets;
		public:
			explicit asset_manager(token);

			void init();

			asset* get_mpq_file(const std::string& path, mpq_file_type_t type);
		private:
			void* load_asset_data(const std::string& path, mpq_file_type_t type);
			static int32_t __stdcall reload();
		};
	}
}
