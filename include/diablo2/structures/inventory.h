#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct unit;

		struct inventory {
			//Offset from Code.		Size: 30 or 40
			int32_t tag; //0x0000
			void* memory_pool; //0x0004
			unit* owner; //0x0008
			unit* first_item; //0x000C
			unit* last_item; //0x0010
			void* inventory_info; //0x0014
			int32_t inventory_info_count; //0x0018
			int32_t weapon_guid; //0x001C
			unit* inventory_owner_item; //0x0020
			int32_t inventory_owner_guid; //0x0024
			int32_t filled_sockets_count; //0x0028
			char pad_002_c[8]; //0x002C
			void* first_corpse; //0x0034
			char pad_0038[4]; //0x0038
			int32_t next_corpse_guid; //0x003C
		};
	}
}
