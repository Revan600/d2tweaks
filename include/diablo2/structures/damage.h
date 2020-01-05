#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct damage {
			int32_t hit_flags;			//0x00
			int16_t result_flags;			//0x04
			int16_t extra;				//0x06
			int32_t phys_damage;			//0x08
			int32_t en_dmg_pct;			//0x0C
			int32_t fire_damage;			//0x10
			int32_t burn_damage;			//0x14
			int32_t burn_len;			//0x18
			int32_t ltng_damage;			//0x1C
			int32_t mag_damage;			//0x20
			int32_t cold_damage;			//0x24
			int32_t pois_damage;			//0x28
			int32_t pois_len;			//0x2C
			int32_t cold_len;			//0x30
			int32_t frz_len;				//0x34
			int32_t life_leech;			//0x38
			int32_t mana_leech;			//0x3C
			int32_t stam_leech;			//0x40
			int32_t stun_len;			//0x44
			int32_t abs_life;			//0x48
			int32_t dmg_total;			//0x4C
			int32_t unk0_x50;				//0x50
			int32_t pierce_pct;			//0x54
			int32_t damage_rate;			//0x58
			int32_t unk0_x5_c;				//0x5C
			int32_t hit_class;			//0x60
			int8_t hit_class_active_set;		//0x64
			char conv_type;				//0x65
			int8_t unk0_x66[2];			//0x66
			int32_t conv_pct;			//0x68
			int8_t unk0_x6_c[4];			//0x6C
		};
	}
}
