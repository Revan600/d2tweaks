#pragma once

#include <cstdint>

namespace d2_protocol {
	struct packet_header;
}

namespace item_mover {
	class common {
	public:
		common();

		void init();

		bool get_packet_size_cs(d2_protocol::packet_header* packet, size_t& size);
		bool get_packet_size_sc(d2_protocol::packet_header* packet, size_t& size);
	};
}