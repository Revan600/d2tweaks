#pragma once

#include <cstdint>

#pragma pack(push, 1)

namespace diablo2 {
	namespace structures {
		struct skills;
		struct game;
		struct inventory;
		struct stats;
		struct path;
		struct player_data;
		struct item_data;
		struct object_data;
		struct monster_data;
		struct drlg_act;
		struct skill_sequence;
		struct anim_data;
		struct unit_gfxdata;
		struct statslistex;
		struct quest_record;

		enum class unit_type_t : int32_t {
			UNIT_TYPE_PLAYER = 0,
			UNIT_TYPE_MONSTER = 1,
			UNIT_TYPE_OBJECT = 2,
			UNIT_TYPE_MISSILE = 3,
			UNIT_TYPE_ITEM = 4,
			UNIT_TYPE_VIS_TILE = 5 //unused?
		};

		enum player_class_t : int32_t {
			PLAYER_CLASS_AMAZON = 0,
			PLAYER_CLASS_SORCERESS = 1,
			PLAYER_CLASS_NECROMANCER = 2,
			PLAYER_CLASS_PALADIN = 3,
			PLAYER_CLASS_BARBARIAN = 4,
			PLAYER_CLASS_DRUID = 5,
			PLAYER_CLASS_ASSASSIN = 6
		};

		struct unit {
			unit_type_t type;

			union {
				player_class_t player_class;
				int32_t data_record_index;
			};

			void* memory_pool;
			uint32_t guid;
			uint32_t mode;

			union {
				player_data* player_data;
				monster_data* monster_data;
				object_data* object_data;
				//missile_data* missile_data;
				item_data* item_data;
			};

			int8_t act;
			int8_t act_padding[0x03];
			drlg_act* drlg_act;

			struct {
				uint32_t low_seed;
				uint32_t high_seed;
			} seed;
			uint32_t init_seed;

			path* path;

			skill_sequence* skill_sequence;
			uint32_t skill_sequence_frame_count;
			uint32_t skill_sequence_frame;

			uint32_t anim_speed;
			uint32_t skill_sequence_mode;

			uint32_t current_frame;
			uint32_t frame_count;
			uint16_t anim_speed_w;

			uint8_t action_frame;
			uint8_t pad1;

			anim_data* anim_data;

			unit_gfxdata* gfxdata;
			unit_gfxdata* gfxdata_copy;

			statslistex* statslistex;
			inventory* inventory;

			union {
				struct {
					uint32_t interact_guid;
					uint32_t interact_type;
					uint8_t interacting;
				} interaction;

				struct {
					void* light_map;
					uint32_t start_light_radius;
					uint16_t p12_shift_index;
				} lightning;
			};

			uint16_t update_type;
			unit* update_unit;

			quest_record* quest_record;
			uint32_t sparky_chest;
			void* timer_args;

			union {
				game* game;
				uint32_t sound_sync;
			};

			char pad2[0x0C];

			void* event;

			unit_type_t owner_type;
			uint32_t owner_guid;

			char pad3[8];

			char* hover_text;

			void* skills;

			void* combat;
			uint32_t hit_class;

			char pad4[4];

			uint32_t drop_code;

			char pad5[8];

			union {
				struct {
					uint32_t unit_flags;
					uint32_t unit_flags_ex;
				} flags;
				uint64_t flags64;
			};

			char pad6[4];

			uint32_t node_index;
			uint32_t get_tick_count;

			union {
				uint32_t get_tick_count2;
				void* particle_stream;
			};

			void* timer;

			unit* change_next_unit; //?
			unit* prev_unit;
			unit* prev_unit_in_room;

			void* msg_first;
			void* msg_last;

			bool is_hireling() const {
				if (type != unit_type_t::UNIT_TYPE_MONSTER)
					return false;

				return (flags.unit_flags & 0x00000200) == 0x00000200;
			}

			bool is_pet() const {
				if (type != unit_type_t::UNIT_TYPE_MONSTER)
					return false;

				return (flags.unit_flags & 0x80000000) == 0x80000000;
			}
		};

		//struct unit {										//Offset from Code.		Size: 0xF4+4
		//	uint32_t		unit_type;				//+00
		//	union {
		//		uint32_t			player_class;
		//		uint32_t			txt_file_no;
		//	};									//+04
		//	uint32_t		unit_id;				//+08
		//	uint32_t		item_num;				//+0C
		//	uint32_t		current_anim;			//+10
		//	union {
		//		monster_data* monster_data;
		//		object_data* object_data;
		//		item_data* item_data;
		//		player_data* player_data;
		//	};									//+14
		//	uint8_t		act;					//+18
		//	uint8_t		uk12[3];				//+19
		//	void* pt_act;					//+1C
		//	uint32_t		seed_low;				//+20
		//	uint32_t		seed_hi;					//+24
		//	uint32_t		seed_starting;			//+28
		//	path* path;					//+2C
		//	uint8_t		uk1[0x08];				//+30
		//	structures::path* pt_pos;					//+38
		//	uint32_t		anim_speed;				//+3C
		//	uint8_t		uk2[0x04];				//+40
		//	uint32_t		cur_frame;				//+44
		//	uint32_t		remaining_frame;			//+48
		//	uint16_t		anim_speed_w;				//+4C
		//	uint8_t		action_flag;				//+4E
		//	uint8_t		uk3[0x1];				//+4F
		//	void* pt_anim_data;				//+50
		//	uint8_t		uk4[0x08];				//+54
		//	stats* stats;				//+5C
		//	inventory* inventory;		    //+60
		//	uint8_t		uk5[0x1C];				//+64
		//	game* game;					//+80
		//	uint8_t		uk6[0x24];				//+84
		//	skills* skills;				//+A8
		//	void* pt_combat_data;			//+AC
		//	uint8_t		uk7[0x08];				//+B0
		//	uint32_t		item_code;				//+B8 for an item being dropped by this unit
		//	uint8_t		uk8[0x08];				//+BC
		//	uint32_t		flags1;					//+C4
		//	union {
		//		uint32_t		flags2;				//+C8	//flags
		//		struct {
		//			uint32_t	ukf1 : 25;
		//			uint32_t	is_lod : 1;
		//		};
		//	};
		//	uint8_t		uk9[0x04];				//+CC
		//	uint32_t		client_id;				//+D0
		//	uint8_t		uk10[0x0C];				//+D4
		//	unit* pt_first_monster;			//+E0
		//	unit* unit1;					//+E4
		//	unit* unit2;					//+E8
		//	uint8_t		uk11[0x08];				//+EC
		//};


	}
}

#pragma pack(pop)