#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct gfxcell;

		struct cellfile {
			int32_t version;		//0x00
			struct {
				int16_t flags;
				int8_t mylastcol;
				int8_t mytabno : 1;
			};				//0x04
			int32_t format;			//0x08
			int32_t termination;		//0x0C
			int32_t numdirs;			//0x10
			int32_t numcells;			//0x14
			gfxcell* cells[255];		//0x18
		};
	}
}