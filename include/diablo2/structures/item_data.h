#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct inventory;
		struct unit;

		enum class item_quality_t : uint32_t {
			ITEM_QUALITY_INFERIOR = 0x01,
			ITEM_QUALITY_NORMAL = 0x02,
			ITEM_QUALITY_SUPERIOR = 0x03,
			ITEM_QUALITY_MAGIC = 0x04,
			ITEM_QUALITY_SET = 0x05,
			ITEM_QUALITY_RARE = 0x06,
			ITEM_QUALITY_UNIQUE = 0x07,
			ITEM_QUALITY_CRAFTED = 0x08,
			ITEM_QUALITY_TEMPERED = 0x09,

			ITEM_QUALITY_COUNT
		};

		struct item_data {
			//Offset from Code.
			item_quality_t quality; //+00
			uint32_t seed_low; //+04
			uint32_t seed_hi; //+08
			uint32_t player_id; //+0C #10734 / #10735 (PCInventory->ptPlayer->0C)
			uint32_t seed_starting; //+10
			uint32_t flags1; //+14
			union {
				uint32_t flags2; //+18
				struct {
					uint32_t fuk1 : 1; //0x00000001
					uint32_t is_indentified : 1; //0x00000002
					uint32_t fuk2 : 2; //0x0000000C
					uint32_t is_unindentified : 1; //0x00000010
					uint32_t fuk3 : 3; //0x000000E0
					uint32_t is_broken : 1; //0x00000100
					uint32_t fuk4 : 2; //0x00000600
					uint32_t is_socketed : 1; //0x00000800
					uint32_t fuk5 : 10; //0x003FF000
					uint32_t is_etheral : 1; //0x00400000
					uint32_t fuk6 : 3; //0x03800000
					uint32_t is_runeword : 1; //0x04000000
					uint32_t fuk7 : 1; //0x08000000
					uint32_t is_personalized : 1; //0x10000000
					uint32_t fuk8 : 3; //0xE0000000
				};
			};

			/*
		ITEMFLAG_NEWITEM               = 0x00000001,
		ITEMFLAG_TAGETING               = 0x00000004,
		ITEMFLAG_UNIDENTIFIED               = 0x00000010,
		ITEMFLAG_QUANTITY               = 0x00000020,
		ITEMFLAG_Durability               = 0x00000100,
		ITEMFLAG_UNKNOWN2               = 0x00000400,
		ITEMFLAG_SOCKETED               = 0x00000800,
		ITEMFLAG_NON_SELLABLE               = 0x00001000,
		ITEMFLAG_NEWITEM2               = 0x00002000,
		ITEMFLAG_UNKNOWN3               = 0x00004000,
		ITEMFLAG_CHECKSECPRICE               = 0x00010000,
		ITEMFLAG_CHECKGAMBLEPRICE          = 0x00020000,
		ITEMFLAG_UNKNOWN4               = 0x00080000,
		ITEMFLAG_INDESTRUCTIBLE(ETHERAL) = 0x00400000,
		ITEMFLAG_UNKNOWN5               = 0x00800000,
		ITEMFLAG_FROMPLAYER               = 0x01000000,
		ITEMFLAG_RUNEuint16_t               = 0x04000000
		*/
			uint32_t guid1; //+1C Global Unique ID 1  
			uint32_t guid2; //+20 Global Unique ID 2  
			uint32_t guid3; //+24 Global Unique ID 3  
			uint32_t unique_id; //+28
			uint8_t ilvl; //+2C
			uint8_t uk1[0x03]; //+2D
			uint16_t version; //+30
			uint16_t rare_prefix; //+32
			uint16_t rare_suffix; //+34
			uint16_t auto_pref; //+36
			uint16_t prefix[3]; //+38
			uint16_t suffix[3]; //+3E
			uint8_t equip_loc; //+44
			/*	emplacement si équipé
			*	00 = noequip/inBelt
			*   01 = head
			*	02 = neck
			*	03 = tors
			*	04 = rarm
			*	05 = larm
			*	06 = lrin
			*	07 = rrin
			*	08 = belt
			*	09 = feet
			*	0A = glov
			*	0B = ralt
			*	0C = lalt
		*/
			uint8_t page; //+45
			/*	page dans laquel se trouve l'item
			*	FF = mouse/equip/onEarth
			*	00 = inventory
			*   01 = cube
			*	04 = stash
			*/
			uint8_t uk4[0x01]; //+46
			uint8_t item_data3; //+47 //D2Common10854 D2Common10853
			uint8_t p_ear_level; //+48
			uint8_t var_gfx; //+49
			char i_name[0x12]; //+4A //inscribed/ear get_name  
			inventory* inventory; //+5C
			unit* pt_prev_item; //+60
			unit* pt_next_item; //+64
			uint8_t uk8[0x01]; //+68
			uint8_t item_data2; //+69
			uint8_t uk9[0x0A]; //+6A
		};
	}
}
