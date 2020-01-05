#pragma once

#include <cstdint>

#include <fw/singleton.h>

namespace diablo2 {
	namespace structures {
		struct unit;
	}
}

namespace d2_tweaks {
	namespace common {
		struct packet_header;
		enum message_types_t;
	}

	namespace client {
		namespace modules {
			class client_module;
		}

		class client : public singleton<client> {
			uint8_t m_module_id_counter;
			uint8_t m_tick_handler_id_counter;
			modules::client_module* m_modules[0xFF]{ nullptr }; //max 255 modules atm.
			modules::client_module* m_tick_handlers[0xFF]{ nullptr }; //max 255 handlers
			modules::client_module* m_packet_handlers[0xFF]{ nullptr }; //max 255 handlers because of one-byte packet header
		public:
			explicit client(token);

			void init();

			void register_module(modules::client_module* module);

			void register_tick_handler(modules::client_module* module);
			void register_packet_handler(common::message_types_t type, modules::client_module* module);
			
			static diablo2::structures::unit* get_client_unit(uint32_t type, uint32_t guid);

		private:
			static void __fastcall handle_packet(common::packet_header* packet, size_t size);
			static int32_t __fastcall game_tick(int32_t a1);
			static int32_t __stdcall draw_game_ui();
		};
	}
}