#pragma once

#include <cstdint>

#define MODULE_INIT(module_name) static d2_tweaks::client::modules::module_name g_instance;

namespace d2_tweaks {
	namespace common {
		struct packet_header;
	}

	namespace client {
		namespace modules {
			class client_module {
			public:
				virtual ~client_module() = default;
				client_module();

				virtual void init() = 0;
				virtual void tick();
				virtual void handle_packet(common::packet_header* packet);
			};
		}
	}
}
