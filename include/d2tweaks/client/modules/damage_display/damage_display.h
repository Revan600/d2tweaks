#pragma once

#include <d2tweaks/client/modules/client_module.h>

//Display damage client side

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class damage_display final : public client_module {
			public:
				void init() override;

				void handle_packet(common::packet_header* packet) override;
				void tick() override;
			};
		}
	}
}