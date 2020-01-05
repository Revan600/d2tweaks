#pragma once

#include <d2tweaks/client/modules/client_module.h>

//Item moving between inventory pages (cube, inventory and stash) by ctrl+click client side

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class item_move final : public client_module {
			public:
				void init() override;

				void handle_packet(common::packet_header* packet) override;
			};
		}
	}
}