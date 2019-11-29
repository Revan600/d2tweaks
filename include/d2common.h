#pragma once

//Took from https://github.com/Speakus/plugy/blob/master/Commons/D2UnitStruct.h

#include <cstdint>

#define F1_IS_SPAWNED 0x10 //or F_IS_IDENTIFIED ?
#define F2_IS_LOD 0x2000000
#define F2_ITEM_IS_IDENTIFIED 0x10 //or F_IS_IDENTIFIED ?

struct monster_data;
struct object_data;
struct item_data;
struct player_data;
struct path_t;
struct stats;
struct inventory;
struct game;
struct skills;
struct cb_player_data;
struct cb_item_data;
struct objects_bin;

struct unit {										//Offset from Code.		Size: 0xF4+4
	uint32_t		unit_type;				//+00
	union {
		uint32_t			player_class;
		uint32_t			txt_file_no;
	};									//+04
	uint32_t		unit_id;				//+08
	uint32_t		item_num;				//+0C
	uint32_t		current_anim;			//+10
	union {
		monster_data* monster_data;
		object_data* object_data;
		item_data* item_data;
		player_data* player_data;
	};									//+14
	uint8_t		act;					//+18
	uint8_t		uk12[3];				//+19
	void* pt_act;					//+1C
	uint32_t		seed_low;				//+20
	uint32_t		seed_hi;					//+24
	uint32_t		seed_starting;			//+28
	path_t* path;					//+2C
	uint8_t		uk1[0x08];				//+30
	path_t* pt_pos;					//+38
	uint32_t		anim_speed;				//+3C
	uint8_t		uk2[0x04];				//+40
	uint32_t		cur_frame;				//+44
	uint32_t		remaining_frame;			//+48
	uint16_t		anim_speed_w;				//+4C
	uint8_t		action_flag;				//+4E
	uint8_t		uk3[0x1];				//+4F
	void* pt_anim_data;				//+50
	uint8_t		uk4[0x08];				//+54
	stats* stats;				//+5C
	inventory* inventory;		    //+60
	uint8_t		uk5[0x1C];				//+64
	game* game;					//+80
	uint8_t		uk6[0x24];				//+84
	skills* skills;				//+A8
	void* pt_combat_data;			//+AC
	uint8_t		uk7[0x08];				//+B0
	uint32_t		item_code;				//+B8 for an item being dropped by this unit
	uint8_t		uk8[0x08];				//+BC
	uint32_t		flags1;					//+C4
	union {
		uint32_t		flags2;				//+C8	//flags
		struct {
			uint32_t	ukf1 : 25;
			uint32_t	is_lod : 1;
		};
	};
	uint8_t		uk9[0x04];				//+CC
	uint32_t		client_id;				//+D0
	uint8_t		uk10[0x0C];				//+D4
	unit* pt_first_monster;			//+E0
	unit* unit1;					//+E4
	unit* unit2;					//+E8
	uint8_t		uk11[0x08];				//+EC
	union {
		cb_player_data* pt_cb_player_data;
		cb_item_data* pt_cb_item_data;
		void* pt_cb_data;
	};									//+F4
};

struct monster_data					// sizeof(MonsterData)=0x60
{										//Offset from Code.
	uint8_t	uk[0x16];					//+00
	union {
		uint16_t	flags;					//+16
		struct {
			uint16_t	fuk1 : 1;
			uint16_t	is_super_unique : 1;
			uint16_t	is_champion : 1;
			uint16_t	is_unique : 1;
			uint16_t	fuk2 : 13;
		};
	};
	uint8_t		uk1[0x0E];					//+18
	uint16_t		super_unique_id;				//+26
	void* unknow1;					//+28
	uint8_t		uk2[0x38];					//+28
};

struct object_data {
	objects_bin* pt_objects_bin;
	uint8_t		level_id;
};

struct item_data//size=0x74
{										//Offset from Code.
	uint32_t	quality;					//+00
	uint32_t	seed_low;					//+04
	uint32_t	seed_hi;						//+08
	uint32_t	player_id;					//+0C #10734 / #10735 (PCInventory->ptPlayer->0C)
	uint32_t	seed_starting;				//+10
	uint32_t	flags1;						//+14
	union {
		uint32_t	flags2;					//+18
		struct {
			uint32_t	fuk1 : 1;				//0x00000001
			uint32_t	is_indentified : 1;	//0x00000002
			uint32_t	fuk2 : 2;				//0x0000000C
			uint32_t	is_unindentified : 1;	//0x00000010
			uint32_t	fuk3 : 3;				//0x000000E0
			uint32_t	is_broken : 1;			//0x00000100
			uint32_t	fuk4 : 2;				//0x00000600
			uint32_t	is_socketed : 1;		//0x00000800
			uint32_t	fuk5 : 10;			//0x003FF000
			uint32_t	is_etheral : 1;		//0x00400000
			uint32_t	fuk6 : 3;				//0x03800000
			uint32_t	is_runeword : 1;		//0x04000000
			uint32_t	fuk7 : 1;				//0x08000000
			uint32_t	is_personalized : 1;	//0x10000000
			uint32_t	fuk8 : 3;				//0xE0000000
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
	uint32_t	guid1;						//+1C Global Unique ID 1  
	uint32_t	guid2;						//+20 Global Unique ID 2  
	uint32_t	guid3;						//+24 Global Unique ID 3  
	uint32_t	unique_id;					//+28
	uint8_t	ilvl;						//+2C
	uint8_t	uk1[0x03];					//+2D
	uint16_t	version;					//+30
	uint16_t	rare_prefix;					//+32
	uint16_t	rare_suffix;					//+34
	uint16_t	auto_pref;					//+36
	uint16_t	prefix[3];					//+38
	uint16_t	suffix[3];					//+3E
	uint8_t	equip_loc;					//+44
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
	uint8_t	page;						//+45
	/*	page dans laquel se trouve l'item
	*	FF = mouse/equip/onEarth
	*	00 = inventory
	*   01 = cube
	*	04 = stash
	*/
	uint8_t	uk4[0x01];					//+46
	uint8_t	item_data3;					//+47 //D2Common10854 D2Common10853
	uint8_t	p_ear_level;					//+48
	uint8_t	var_gfx;						//+49
	char	i_name[0x12];				//+4A //inscribed/ear name  
	inventory* inventory;			//+5C
	unit* pt_prev_item;					//+60
	unit* pt_next_item;					//+64
	uint8_t	uk8[0x01];					//+68
	uint8_t	item_data2;					//+69
	uint8_t	uk9[0x0A];					//+6A
};

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
	//+1A8 is ptGame
	//+4A8 ptNextClient
/*
Kingpin: ptPclient
+16C is either act or unit type
+170 Active UniqueID for player
+174 Active ptPlayer on Client
+1a8 ptGame
+1b4 Current or next ptRoom
*/
};

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

struct position {
	uint32_t x;
	uint32_t y;
};

struct room//size=0x80
{
	//ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
	uint32_t	seed1;						//+00
	uint32_t	seed2;						//+04
	uint8_t	uk8[0x1C];					//+08
	room* pt_near_rooms;				//+24
	uint32_t	nb_near_rooms;				//+28
	unit* unit;						//+2C
	uint8_t	uk5[0x44];					//+30
	room* pt_next_room;					//+74
	union {
		uint8_t	flags;					//+78
		struct {
			uint8_t is_generated : 1;
			uint8_t is_generated2 : 1;//???
		};
	};
};

struct act_map//ptGame+BC size=0x60
{
	uint32_t	is_not_managed;
	uint32_t	uk4;
	uint32_t	uk8;//size = 0x488
	room* pt_first_room;
};

struct game {                              			//Offset from Code.
	uint8_t	uk1[0x18];					//+00
	uint32_t	pt_lock;					//+18 Unknown  
	uint32_t	memory_pool;					//+1C Memory Pool (??)  
	uint8_t	uk2[0x4D];					//+20
	uint8_t	difficulty_level;			//+6D (Difficulty 0,1 or 2)
	uint16_t	unknown1;					//+6E Cube puts 4 here
	uint32_t	is_lod_game;					//+70 (D2=0 LOD =1) (uint32_t ?)
	uint8_t	uk3[0x04];					//+71
	uint16_t	unknown2;					//+78
	uint8_t	uk4[0x0E];					//+7A
	net_client* pt_client;				//+88
	uint8_t	uk8_c[0x30];					//+8C
	act_map* map_act[5];					//+BC
	uint8_t	uk_d0[0x1024];				//+D0
	uint32_t* game10_f4;					//+10F4
	uint8_t	uk6[0x28];					//+10F8
	unit* units[0xA00];				//+1120
	unit* roomtitles[0x200];			//+1B20
};

struct path_t //(8 dword)
{                              			//Offset from Code.		Size: 20
	uint16_t	uk1;						//+00
	uint16_t	mapx;						//+02
	uint16_t	uk2;						//+04
	uint16_t	mapy;						//+06
	uint32_t	uk3;						//+08
	uint32_t	x;							//+0C
	uint32_t	y;							//+10
	uint32_t	uk6;						//+14
	uint32_t	uk7;						//+18
	room* pt_room;						//+1C
};

struct inventory {                               		//Offset from Code.		Size: 30 à 40
	uint32_t	tag;						//+00	= 0x01020304
	uint8_t	uk1[0x04];					//+04	=? 0
	unit* pt_char;						//+08
	unit* pt_first_item;						//+0C
	unit* pt_last_item; //+10
	void* inv_info_ptr; //+14
	uint32_t inv_info; //+18
	uint32_t weapon_guid; //+1C
	unit* inv_owner_item;					//+20
	uint8_t	owner_guid;					//+24
	uint32_t	current_used_socket;			//+28 //Kingpin : a variable to know how many sockets that have added to item
	uint32_t	inventory2_c;				//+2C //one value
	uint32_t	inventory30;				//+30
	void* pt_corpse;					//+34
	uint8_t	uk5[0x04];					//+38
	uint32_t	next_corpse_id;				//+3C //max = 15
	uint8_t	uk6[0x04];					//+40
};