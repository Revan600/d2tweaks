#pragma once

#include <diablo2/structures/item_data.h>

#include <cstring>

namespace d2_tweaks {
	namespace client {
		namespace modules {
			struct loot_filter_settings {
				size_t size; //struct size

				bool alt_only;
				bool show_gold;
				bool show_runes;
				bool show_gems;

				bool quality_settings[static_cast<size_t>(diablo2::structures::item_quality_t::ITEM_QUALITY_COUNT)];

				char reserved[1004];

				static loot_filter_settings& get();

				static void save(const char* name);
				static void load(const char* name);
				static void remove(const char* name);
				
			private:
				loot_filter_settings() : size(sizeof(loot_filter_settings)),
					alt_only(false), show_gold(true), show_runes(true), show_gems(true), reserved{}
				{
					memset(quality_settings, 0x1, sizeof quality_settings);
				}
			};
		}
	}
}
