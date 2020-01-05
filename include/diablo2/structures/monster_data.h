#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct monstats_line;

		struct monster_data {    // sizeof(MonsterData)=0x60
			monstats_line* monstats;
			uint8_t components[16];     //+04
			union {
				uint16_t flags;     //+16
				struct {
					uint16_t fuk1 : 1;
					uint16_t is_super_unique : 1;
					uint16_t is_champion : 1;
					uint16_t is_unique : 1;
					uint16_t fuk2 : 13;
				};
			};
			uint8_t  uk1[0x0E];     //+18
			uint16_t  super_unique_id;    //+26
			void* unknow1;     //+28
			uint8_t  uk2[0x38];     //+28
		};
	}
}