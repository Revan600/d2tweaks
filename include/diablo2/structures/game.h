#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct unit;

		struct game {
			char pad_0000[24]; //0x0000
			void* critical_section; //0x0018
			void* memory_pool; //0x001C
			char pad_0020[74]; //0x0020
			int8_t game_type; //0x006A
			char pad_006_b[2]; //0x006B
			int8_t difficulty_level; //0x006D
			char pad_006_e[2]; //0x006E
			int32_t expansion; //0x0070
			int32_t game_type2; //0x0074
			int32_t item_format; //0x0078
			int32_t init_seed; //0x007C
			int32_t object_seed; //0x0080
			int32_t init_seed2; //0x0084
			void* last_client; //0x0088 structure of last player that entered the game
			int32_t clients_count; //0x008C
			int32_t unit_counts[6]; //0x0090
			int32_t game_frame; //0x00A8
			char pad_00_ac[12]; //0x00AC
			void* timer_queue; //0x00B8
			void* drlg_act[5]; //0x00BC
			int32_t lo_seed; //0x00D0
			int32_t hi_seed; //0x00D4
			char pad_00d8[20]; //0x00D8
			int32_t monster_seed; //0x00EC
			void* monster_region[1024]; //0x00F0
			void* object_controller; //0x10F0
			void* quest_controller; //0x10F4
			void* unit_nodes[10]; //0x10F8
			unit* unit_list[5][128]; //0x1120
			void* tile_list; //0x1B20
			int32_t unique_flags[128]; //0x1B24
			void* npc_control; //0x1D24
			void* arena_control; //0x1D28
			void* party_control; //0x1D2C
			int8_t boss_flags[64]; //0x1D30
			int32_t monster_mode_data[17]; //0x1D70
			int32_t monster_mode_data_count; //0x1DB4
			char pad_1db8[12]; //0x1DB8
			int32_t sync_timer; //0x1DC4
			char pad_1dc8[32]; //0x1DC8
			int32_t uber_baal; //0x1DE8
			int32_t uber_diablo; //0x1DEC
			int32_t uber_mephisto; //0x1DF0
		}; //Size: 0x1DF4
		static_assert(sizeof(game) == 0x1DF4);
	}
}
