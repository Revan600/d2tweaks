#pragma once

#define MODULE_INIT(module_name) static d2_tweaks::server::modules::module_name g_instance;

namespace diablo2 {
	namespace structures {
		struct game;
		struct unit;
	}
}

namespace d2_tweaks {
	namespace common {
		struct packet_header;
	}

	namespace server {
		namespace modules {
			class server_module {
			public:
				virtual ~server_module() = default;
				server_module();

				virtual void init() = 0;

				/**
				 * \brief
				 * \param game
				 * \param player
				 * \param packet
				 * \return true - block further packet processing, false - pass packet to game
				 */
				virtual bool handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet);
				virtual void tick();
			};
		}
	}
}
