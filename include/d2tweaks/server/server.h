#pragma once

#include <cstdint>

#include <fw/singleton.h>
#include <d2tweaks/common/protocol.h>
#include <functional>

namespace diablo2 {
	namespace structures {
		struct game;
		struct inventory;
		struct unit;
		struct net_client;
	}
}

namespace d2_tweaks {
	namespace common {
		struct packet_header;
	}

	namespace server {
		namespace modules {
			class server_module;
		}

		class server : public singleton<server> {
			uint8_t m_module_id_counter;
			uint8_t m_tick_handler_id_counter;
			modules::server_module* m_modules[0xFF]{ nullptr }; //max 255 modules atm.
			modules::server_module* m_tick_handlers[0xFF]{ nullptr }; //max 255 modules atm.
			modules::server_module* m_packet_handlers[0xFF]{ nullptr }; //max 255 handlers because of one-byte packet header
		public:
			explicit server(token);

			void init();

			void send_packet(diablo2::structures::net_client* client, common::packet_header* packet, size_t size);
			bool handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet);

			void register_module(modules::server_module* module);

			void register_tick_handler(modules::server_module* module);
			void register_packet_handler(common::message_types_t type, modules::server_module* module);

			diablo2::structures::unit* get_server_unit(diablo2::structures::game* game, uint32_t guid, uint32_t type);
			void iterate_server_units(diablo2::structures::game* game, uint32_t type,
									  const std::function<bool(diablo2::structures::unit*)>& cb);
		private:
			static int32_t net_tick();
		};
	}
}