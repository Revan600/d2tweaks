#pragma once

#include <cstdint>

#include <d2tweaks/server/modules/server_module.h>

//Item moving between inventory pages (cube, inventory and stash) by ctrl+click server side

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
			class item_move final : public server_module {
			public:
				void init() override;

				bool handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player,
								   common::packet_header* packet) override;
			private:
				bool find_free_space(diablo2::structures::inventory* inv,
									 diablo2::structures::unit* item, int32_t inventoryIndex, char page, uint32_t& x, uint32_t& y);
			};
		}
	}
}