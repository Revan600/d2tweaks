#pragma once

#include <cstdint>

#include <d2tweaks/server/modules/server_module.h>

//Display damage server side

namespace diablo2 {
	namespace structures {
		struct inventory;
		struct game;
		struct unit;
	}
}

namespace d2_tweaks {
	namespace server {
		class server;

		namespace modules {
			class damage_display final : public server_module {
			public:
				void init() override;

				bool handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player,
								   common::packet_header* packet) override;
				void tick() override;
			};
		}
	}
}