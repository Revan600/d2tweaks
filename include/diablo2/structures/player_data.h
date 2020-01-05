#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct net_client;
		
		struct player_data {
			char		name[0x10];				//+00	Player Name  
			void* pt_quest[3];				//+10	Quest Pointers for each difficulty  
			uint8_t		uk1[0x18];				//+1C		//before : 0x14
			void* pt_arena_unit;			//+34	ptArena for the Unit  
			uint8_t		uk2[0x4];				//+38		//before : 0x7
			uint16_t		mp_source_portal_unique_id;	//+3C	Source Portal Unique_ID  
			uint8_t		uk3[0x2];				//+3E
			uint16_t		mp_dest_portal_unique_id;	//+40	Destination Portal Unique_ID  
			uint8_t		uk4[0x06];				//+42  
			uint8_t		pt_object_un_id;			//+48	Object UniqueID for TownPortals       
			uint8_t		uk5[0x53];				//+49  
			net_client* net_client;			//+9C	ptClient  
		};
	}
}