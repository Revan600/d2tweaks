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

//https://phrozen-library.fandom.com/wiki/Items_line
struct items_line {
	char	szflippyfile[32];	//0x00
	char	szinvfile[32];		//0x20
	char	szuniqueinvfile[32];	//0x40
	char	szsetinvfile[32];	//0x60
	union {
		uint32_t	dwcode;		//0x80
		char	szcode[4];	//0x80
	};
	uint32_t	dwnormcode;		//0x84
	uint32_t	dwubercode;		//0x88
	uint32_t	dwultracode;		//0x8C
	uint32_t	dwalternategfx;		//0x90
	uint32_t	dwpSpell;		//0x94
	uint16_t	wstate;			//0x98
	uint16_t	wcstate1;		//0x9A
	uint16_t	wcstate2;		//0x9C
	uint16_t	wstat1;			//0x9E
	uint16_t	wstat2;			//0xA0
	uint16_t	wstat3;			//0xA2
	uint32_t	dwcalc1;		//0xA4
	uint32_t	dwcalc2;		//0xA8
	uint32_t	dwcalc3;		//0xAC
	uint32_t	dwLen;			//0xB0
	uint16_t	bSpellDesc;		//0xB4
	uint16_t	wSpellDescStr;		//0xB6
	uint32_t	dwSpellDescCalc;	//0xB8
	uint32_t	dwBetterGem;		//0xBC
	uint32_t	dwwclass;		//0xC0
	uint32_t	dw2handedwclass;	//0xC4
	uint32_t	dwTMogType;		//0xC8
	uint32_t	dwminac;		//0xCC
	uint32_t	dwmaxac;		//0xD0
	uint32_t	dwgamblecost;		//0xD4
	uint32_t	dwspeed;		//0xD8
	uint32_t	dwbitfield1;		//0xDC
	uint32_t	dwcost;			//0xE0
	uint32_t	dwminstack;		//0xE4
	uint32_t	dwmaxstack;		//0xE8
	uint32_t	dwspawnstack;		//0xEC
	uint32_t	dwgemoffset;		//0xF0
	uint16_t	wnamestr;		//0xF4
	uint16_t	wversion;		//0xF6
	uint16_t	wautoprefix;		//0xF8
	uint16_t	wmissiletype;		//0xFA
	uint8_t	brarity;		//0xFC
	uint8_t	blevel;			//0xFD
	uint8_t	bmindam;		//0xFE
	uint8_t	bmaxdam;		//0xFF
	uint8_t	bminmisdam;		//0x100
	uint8_t	bmaxmisdam;		//0x101
	uint8_t	b2handmindam;		//0x102
	uint8_t	b2handmaxdam;		//0x103
	uint16_t	brangeadder;		//0x104
	uint16_t	wstrbonus;		//0x106
	uint16_t	wdexbonus;		//0x108
	uint16_t	wreqstr;		//0x10A
	uint16_t	wreqdex;		//0x10C
	uint8_t	babsorbs;		//0x10E
	uint8_t	binvwidth;		//0x10F
	uint8_t	binvheight;		//0x110
	uint8_t	bblock;			//0x111
	uint8_t	bdurability;		//0x112
	uint8_t	bnodurability;		//0x113
	uint8_t	bmissile;		//0x114
	uint8_t	bcomponent;		//0x115
	uint8_t	brArm;			//0x116
	uint8_t	blArm;			//0x117
	uint8_t	btorso;			//0x118
	uint8_t	blegs;			//0x119
	uint8_t	brspad;			//0x11A
	uint8_t	blspad;			//0x11B
	uint8_t	b2handed;		//0x11C
	uint8_t	buseable;		//0x11D
	uint16_t	wtype;			//0x11E
	uint16_t	wtype2;			//0x120
	uint16_t	bsubtype;		//0x122
	uint16_t	wdropsound;		//0x124
	uint16_t	wusesound;		//0x126
	uint8_t	bdropsfxframe;		//0x128
	uint8_t	bunique;		//0x129
	uint8_t	bquest;			//0x12A
	uint8_t	bquestdiffcheck;	//0x12B
	uint8_t	btransparent;		//0x12C
	uint8_t	btranstbl;		//0x12D
	uint8_t 	pad0x12E;		//0x12E
	uint8_t	blightradius;		//0x12F
	uint8_t	bbelt;			//0x130
	uint8_t	bautobelt;		//0x131
	uint8_t	bstackable;		//0x132
	uint8_t	bspawnable;		//0x133
	uint8_t	bspellicon;		//0x134
	uint8_t	bdurwarning;		//0x135
	uint8_t	bqntwarning;		//0x136
	uint8_t	bhasinv;		//0x137
	uint8_t	bgemsockets;		//0x138
	uint8_t	bTransmogrify;		//0x139
	uint8_t	bTMogMin;		//0x13A
	uint8_t	bTMogMax;		//0x13B
	uint8_t	bhitclass;		//0x13C
	uint8_t	b1or2handed;		//0x13D
	uint8_t	bgemapplytype;		//0x13E
	uint8_t	blevelreq;		//0x13F
	uint8_t	bmagiclvl;		//0x140
	uint8_t	bTransform;		//0x141
	uint8_t	bInvTrans;		//0x142
	uint8_t	bcompactsave;		//0x143
	uint8_t	bSkipName;		//0x144
	uint8_t	bNameable;		//0x145
	uint8_t	bAkaraMin;		//0x146
	uint8_t	bGheedMin;		//0x147
	uint8_t	bCharsiMin;		//0x148
	uint8_t	bFaraMin;		//0x149
	uint8_t	bLysanderMin;		//0x14A
	uint8_t	bDrognanMin;		//0x14B
	uint8_t	bHraltiMin;		//0x14C
	uint8_t	bAlkorMin;		//0x14D
	uint8_t	bOrmusMin;		//0x14E
	uint8_t	bElzixMin;		//0x14F
	uint8_t	bAshearaMin;		//0x150
	uint8_t	bCainMin;		//0x151
	uint8_t	bHalbuMin;		//0x152
	uint8_t	bJamellaMin;		//0x153
	uint8_t	bMalahMin;		//0x154
	uint8_t	bLarzukMin;		//0x155
	uint8_t	bDrehyaMin;		//0x156
	uint8_t	bAkaraMax;		//0x157
	uint8_t	bGheedMax;		//0x158
	uint8_t	bCharsiMax;		//0x159
	uint8_t	bFaraMax;		//0x15A
	uint8_t	bLysanderMax;		//0x15B
	uint8_t	bDrognanMax;		//0x15C
	uint8_t	bHraltiMax;		//0x15D
	uint8_t	bAlkorMax;		//0x15E
	uint8_t	bOrmusMax;		//0x15F
	uint8_t	bElzixMax;		//0x160
	uint8_t	bAshearaMax;		//0x161
	uint8_t	bCainMax;		//0x162
	uint8_t	bHalbuMax;		//0x163
	uint8_t	bJamellaMax;		//0x164
	uint8_t	bMalahMax;		//0x165
	uint8_t	bLarzukMax;		//0x166
	uint8_t	bDrehyaMax;		//0x167
	uint8_t	bAkaraMagicMin;		//0x168
	uint8_t	bGheedMagicMin;		//0x169
	uint8_t	bCharsiMagicMin;	//0x16A
	uint8_t	bFaraMagicMin;		//0x16B
	uint8_t	bLysanderMagicMin;	//0x16C
	uint8_t	bDrognanMagicMin;	//0x16D
	uint8_t	bHraltiMagicMin;	//0x16E
	uint8_t	bAlkorMagicMin;		//0x16F
	uint8_t	bOrmusMagicMin;		//0x170
	uint8_t	bElzixMagicMin;		//0x171
	uint8_t	bAshearaMagicMin;	//0x172
	uint8_t	bCainMagicMin;		//0x173
	uint8_t	bHalbuMagicMin;		//0x174
	uint8_t	bJamellaMagicMin;	//0x175
	uint8_t	bMalahMagicMin;		//0x176
	uint8_t	bLarzukMagicMin;	//0x177
	uint8_t	bDrehyaMagicMin;	//0x178
	uint8_t	bAkaraMagicMax;		//0x179
	uint8_t	bGheedMagicMax;		//0x17A
	uint8_t	bCharsiMagicMax;	//0x17B
	uint8_t	bFaraMagicMax;		//0x17C
	uint8_t	bLysanderMagicMax;	//0x17D
	uint8_t	bDrognanMagicMax;	//0x17E
	uint8_t	bHraltiMagicMax;	//0x17F
	uint8_t	bAlkorMagicMax;		//0x180
	uint8_t	bOrmusMagicMax;		//0x181
	uint8_t	bElzixMagicMax;		//0x182
	uint8_t	bAshearaMagicMax;	//0x183
	uint8_t	bCainMagicMax;		//0x184
	uint8_t	bHalbuMagicMax;		//0x185
	uint8_t	bJamellaMagicMax;	//0x186
	uint8_t	bMalahMagicMax;		//0x187
	uint8_t	bLarzukMagicMax;	//0x188
	uint8_t	bDrehyaMagicMax;	//0x189
	uint8_t	bAkaraMagicLvl;		//0x18A
	uint8_t	bGheedMagicLvl;		//0x18B
	uint8_t	bCharsiMagicLvl;	//0x18C
	uint8_t	bFaraMagicLvl;		//0x18D
	uint8_t	bLysanderMagicLvl;	//0x18E
	uint8_t	bDrognanMagicLvl;	//0x18F
	uint8_t	bHraltiMagicLvl;	//0x190
	uint8_t	bAlkorMagicLvl;		//0x191
	uint8_t	bOrmusMagicLvl;		//0x192
	uint8_t	bElzixMagicLvl;		//0x193
	uint8_t	bAshearaMagicLvl;	//0x194
	uint8_t	bCainMagicLvl;		//0x195
	uint8_t	bHalbuMagicLvl;		//0x196
	uint8_t	bJamellaMagicLvl;	//0x197
	uint8_t	bMalahMagicLvl;		//0x198
	uint8_t	bLarzukMagicLvl;	//0x199
	uint8_t	bDrehyaMagicLvl;	//0x19A
	uint8_t	pad0x19B;		//0x19B
	uint32_t	dwNightmareUpgrade;	//0x19C
	uint32_t	dwHellUpgrade;		//0x1A0
	uint8_t	bPermStoreItem;		//0x1A4
	uint8_t	bmultibuy;		//0x1A5
	uint16_t	pad0x1A6;		//0x1A6
};