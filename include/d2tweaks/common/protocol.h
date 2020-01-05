#pragma once

#include <cstdint>

namespace d2_tweaks {
	namespace common {
#pragma pack(push,1)
		enum message_types_t {
			MESSAGE_TYPE_ITEM_MOVE,
			MESSAGE_TYPE_INVENTORY_SORT,
			MESSAGE_TYPE_DAMAGE_INFO,

			MESSAGE_TYPE_COUNT
		};

		enum damage_type_t : uint8_t {
			DAMAGE_TYPE_PHYSICAL = 0,

			DAMAGE_TYPE_COLD = 1,
			DAMAGE_TYPE_FIRE = 2,
			DAMAGE_TYPE_LIGHTNING = 3,
			DAMAGE_TYPE_POISON = 4,
			DAMAGE_TYPE_MAGIC = 5,

			DAMAGE_TYPE_COUNT,
			DAMAGE_TYPE_UNKNOWN = 0xFF
		};

		struct packet_header {
			uint8_t d2_packet_type;
			uint8_t message_type;

			packet_header() : d2_packet_type(0xBB), message_type(0) {}
		};

		struct item_move_cs : packet_header {
			uint32_t item_guid;
			uint8_t target_page;

			item_move_cs() : item_guid(0), target_page(0) {
				message_type = MESSAGE_TYPE_ITEM_MOVE;
			}
		};

		struct item_move_sc : packet_header {
			uint32_t item_guid;
			uint32_t tx;
			uint32_t ty;
			uint8_t target_page;

			item_move_sc() : item_guid(0), tx(0), ty(0), target_page(0) {
				message_type = MESSAGE_TYPE_ITEM_MOVE;
			}
		};

		struct inventory_sort_cs : packet_header {
			uint8_t page;

			inventory_sort_cs() : page(0) {
				message_type = MESSAGE_TYPE_INVENTORY_SORT;
			}
		};

		struct inventory_sort_sc : packet_header {
			uint8_t page;
			uint8_t tx;
			uint8_t ty;
			uint32_t guid;

			inventory_sort_sc() : page(0), tx(0), ty(0), guid(0) {
				message_type = MESSAGE_TYPE_INVENTORY_SORT;
			}
		};

		struct damage_info_cs : packet_header {
			uint8_t state; //on or off

			damage_info_cs() : state(0) {
				message_type = MESSAGE_TYPE_DAMAGE_INFO;
			}
		};

		struct damage_info_sc : packet_header {
			uint8_t unit_type;
			uint32_t guid;
			damage_type_t damage_type;
			uint32_t damage;

			damage_info_sc() : unit_type(0), guid(0), damage_type(DAMAGE_TYPE_UNKNOWN), damage(0) {
				message_type = MESSAGE_TYPE_DAMAGE_INFO;
			}
		};
#pragma pack(pop)
	}
}