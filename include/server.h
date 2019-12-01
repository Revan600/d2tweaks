#pragma once

#include <cstdint>

struct game;
struct unit;
struct inventory;

namespace d2_protocol {
	struct packet_header;
}

namespace item_mover {
	class common;
	
	class server {
	public:
		server();

		void init(common* common);

		bool handle_packet(game* game, unit* player, d2_protocol::packet_header* packet);
	private:
		static unit* get_server_unit(game* game, uint32_t guid, uint32_t type);
		bool find_free_space(inventory* inv, unit* item, int32_t inventoryIndex, char page, uint32_t& x, uint32_t& y);

		bool handle_item_move(game* game, unit* player, d2_protocol::packet_header* packet);
	};
}