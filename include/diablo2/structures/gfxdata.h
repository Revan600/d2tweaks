#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct cellfile;
		struct gfxcell;

		struct gfxdata {
			gfxcell* cell_init;    //+00
			cellfile* cell_file;   //+04
			int32_t frame;                //+08
			int32_t direction;            //+0C
			int max_directions;            //+10
			int max_frames;                //+14
			int32_t flags;                //+18
			int8_t state;                //+1C
			union {
				int8_t component;            //+1D
				int8_t item_flags;            //+1D
			};
			int8_t unk1_e;                //+1E - padding no doubt
			int8_t unk1_f;                //+1F
			int unit_type;                //+20
			int unit_index;                //+24
			int mode;                    //+28
			int overlay;                //+2C
			union {
				// [token][component][type][mode][hitclass]
				struct {
					int32_t token;                //+30
					int32_t component;            //+34
					int32_t armor_type;            //+38 - lit, med , hvy
					int32_t mode;                //+3C
					int32_t hit_class;            //+40
				} details;
				char unk_name[5][4];                //+30
			};
			const char* name;                //+44
		};

		static_assert(sizeof(gfxdata) == 0x48);
	}
}
