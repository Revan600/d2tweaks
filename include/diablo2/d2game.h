#pragma once

#include <cstdint>
#include <functional>

namespace diablo2 {
	namespace structures {
		struct game_server;
		struct net_client;
		struct game;
		struct unit;
	}

	class d2_game {
	public:
		static char* get_base();

		static void enqueue_packet(structures::net_client* client, void* packet, size_t size);

		static uint32_t* get_game_id_array_begin();
		static uint32_t* get_game_id_array_end();

		static structures::game_server* get_game_server();
		static structures::game* get_game(structures::game_server* gs, uint32_t gameId);

		static structures::game* get_game_from_client_id(int32_t id);
		static structures::net_client* get_net_client_from_id(structures::game* game, int32_t id);

		static structures::unit* get_player_pet(structures::game* game, structures::unit* unit, uint32_t type, uint32_t index);

		static structures::unit* get_unit_owner(structures::game* game, structures::unit* unit);
		static void* iterate_unit_pets(structures::game* game, structures::unit* unit,
									   const std::function<void(structures::game*, structures::unit*, structures::unit*)>& cb);

		static void update_inventory_items(structures::game* game, structures::unit* player);
	};
}
