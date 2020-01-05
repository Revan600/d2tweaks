#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct room;

		struct path //(8 dword)
		{                                 //Offset from Code.  Size: 20
			uint16_t uk1;      //+00
			uint16_t mapx;      //+02
			uint16_t uk2;      //+04
			uint16_t mapy;      //+06
			uint32_t uk3;      //+08
			uint32_t x;       //+0C
			uint32_t y;       //+10
			uint32_t uk6;      //+14
			uint32_t uk7;      //+18
			room* pt_room;      //+1C
		};
	}
}