#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct unit;
		
		struct room//size=0x80
		{
			//ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
			uint32_t	seed1;						//+00
			uint32_t	seed2;						//+04
			uint8_t	uk8[0x1C];					//+08
			room* pt_near_rooms;				//+24
			uint32_t	nb_near_rooms;				//+28
			unit* unit;						//+2C
			uint8_t	uk5[0x44];					//+30
			room* pt_next_room;					//+74
			union {
				uint8_t	flags;					//+78
				struct {
					uint8_t is_generated : 1;
					uint8_t is_generated2 : 1;//???
				};
			};
		};
	}
}