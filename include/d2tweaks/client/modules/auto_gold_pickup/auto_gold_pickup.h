#pragma once

#include <d2tweaks/client/modules/client_module.h>

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class auto_gold_pickup final : public client_module {
			public:
				void init() override;

				void handle_packet(common::packet_header* packet) override;
			};
		}
	}
}