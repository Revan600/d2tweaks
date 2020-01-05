#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct room;

		struct act_map //ptGame+BC size=0x60
		{
			uint32_t is_not_managed;
			uint32_t uk4;
			uint32_t uk8; //size = 0x488
			room* pt_first_room;
		};
	}
}
