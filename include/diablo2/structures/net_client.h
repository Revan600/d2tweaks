#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct game;
		
		struct net_client {
			uint32_t	client_id;					//+00
			uint8_t	uk1[0x06];					//+04
			union {								//+0A
				uint16_t	flag;
				struct {
					uint16_t f1 : 1;
					uint16_t f2 : 1;
					uint16_t is_hard_core_game : 1;
				};
			};
			uint8_t	uk2;						//+0C
			char	name[0x10];					//+0D
			uint8_t	uk3[0x15F];					//+1D
			uint8_t* savefile;					//+17C
			uint32_t	final_size;					//+180
			uint32_t	counter;					//+184
			uint32_t	current_size;				//+188
			uint8_t	uk4[0x1C];					//+18C
			game* game;						//+1A8
		};
	}
}