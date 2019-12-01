#pragma once

#include <cstdint>

#pragma pack(push, 1)
namespace d2_protocol {
	enum message_types_t {
		MESSAGE_TYPE_ITEM_MOVE,

		MESSAGE_TYPE_COUNT
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
}
#pragma pack(pop, 1)