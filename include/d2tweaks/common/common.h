#pragma once

#include <fw/singleton.h>

namespace d2_tweaks {
	namespace common {
		struct packet_header;

		class common : public singleton<common> {
		public:
			explicit common(token);

			void init();

			bool get_packet_size_cs(packet_header* packet, size_t& size);
			bool get_packet_size_sc(packet_header* packet, size_t& size);
		};
	}
}