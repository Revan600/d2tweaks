#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct item_types_line {
			char code[4];						//0x00
			int16_t equiv1;						//0x04
			int16_t equiv2;						//0x06
			int8_t repair;						//0x08
			int8_t body;							//0x09
			int8_t body_loc1;						//0x0A
			int8_t body_loc2;						//0x0B
			int16_t shoots;						//0x0C
			int16_t quiver;						//0x0E
			int8_t throwable;					//0x10
			int8_t reload;						//0x11
			int8_t re_equip;						//0x12
			int8_t auto_stack;					//0x13
			int8_t magic;						//0x14
			int8_t rare;							//0x15
			int8_t normal;						//0x16
			int8_t charm;						//0x17
			int8_t gem;							//0x18
			int8_t beltable;						//0x19
			int8_t max_sock1;						//0x1A
			int8_t max_sock25;					//0x1B
			int8_t max_sock40;					//0x1C
			int8_t treasure_class;				//0x1D
			int8_t rarity;						//0x1E
			int8_t staff_mods;					//0x1F
			int8_t cost_formula;					//0x20
			int8_t item_class;						//0x21
			int8_t store_page;					//0x22
			int8_t var_inv_gfx;					//0x23
			char inv_gfx1[32];					//0x24
			char inv_gfx2[32];					//0x44
			char inv_gfx3[32];					//0x64
			char inv_gfx4[32];					//0x84
			char inv_gfx5[32];					//0xA4
			char inv_gfx6[32];					//0xC4
		};
	}
}