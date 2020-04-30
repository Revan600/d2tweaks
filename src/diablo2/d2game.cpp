#include <diablo2/d2game.h>
#include <common/ptr_wrapper.h>

char* diablo2::d2_game::get_base() {
	static auto base = reinterpret_cast<char*>(GetModuleHandle("d2game.dll"));
	return base;
}

void diablo2::d2_game::enqueue_packet(structures::net_client* client, void* packet, size_t size) {
	static wrap_func_fast<void(structures::net_client*, void*, size_t)> enqueue_packet(0xC710, get_base());
	enqueue_packet(client, packet, size);
}

diablo2::structures::game* diablo2::d2_game::get_game_from_client_id(int32_t id) {
	static wrap_func_fast<structures::game * (int32_t)> get_game_from_client_id(0x94E0, get_base());
	return get_game_from_client_id(id);
}

diablo2::structures::net_client* diablo2::d2_game::get_net_client_from_id(structures::game* game, int32_t id) {
	static wrap_func_fast<structures::net_client * (structures::game*, int32_t)>get_net_client_from_id(0x1DE0, get_base());
	return get_net_client_from_id(game, id);
}

diablo2::structures::unit* diablo2::d2_game::get_player_pet(structures::game* game, structures::unit* unit,
															uint32_t type, uint32_t index) {
	static wrap_func_fast<structures::unit * (structures::game*, structures::unit*, uint32_t, uint32_t)>get_player_pet(0x4E8B0, get_base());
	return get_player_pet(game, unit, type, index);
}

int32_t diablo2::d2_game::identify_item(structures::game* game, structures::unit* player, structures::unit* item) {
	static wrap_func_fast<int32_t(structures::game*, structures::unit*, structures::unit*)> identify_item(0x19670, get_base());
	return identify_item(game, player, item);
}

int32_t diablo2::d2_game::pickup_gold_pile(structures::game* game, structures::unit* unit, structures::unit* item) {
	static wrap_func_fast<int32_t(structures::game*, structures::unit*, structures::unit*)> pickup_gold_pile(0x12DD0, get_base());
	return pickup_gold_pile(game, unit, item);
}

void diablo2::d2_game::update_inventory_items(structures::game* game, structures::unit* player) {
	static wrap_func_fast<void(structures::game*, structures::unit*, uint32_t)>update_inventory_items(0x14A90, get_base());
	update_inventory_items(game, player, 0);
}

uint32_t* diablo2::d2_game::get_game_id_array_begin() {
	static wrap_value<uint32_t> game_id_array_begin(0x1147F8, get_base());
	return game_id_array_begin;
}

uint32_t* diablo2::d2_game::get_game_id_array_end() {
	static wrap_value<uint32_t> game_id_array_end(0x1157F8, get_base());
	return game_id_array_end;
}

diablo2::structures::game_server* diablo2::d2_game::get_game_server() {
	static wrap_value<structures::game_server*> game_server(0x115818, get_base());
	return *game_server;
}

static diablo2::structures::game* (__thiscall* g_get_game)(diablo2::structures::game_server* gs, uint32_t gameId, void*)
= decltype(g_get_game)(0xB6A0 + diablo2::d2_game::get_base());

diablo2::structures::game* diablo2::d2_game::get_game(structures::game_server* gs, uint32_t gameId) {
	static wrap_func_std<structures::game * (uint32_t, void*)> get_game(0xB6A0, get_base());
	return g_get_game(gs, gameId, reinterpret_cast<char*>(gs) + 0x44);
}

diablo2::structures::unit* diablo2::d2_game::get_unit_owner(structures::game* game, structures::unit* unit) {
	static wrap_func_fast<structures::unit * (structures::game*, structures::unit*)> get_unit_owner(0x8BB70, get_base());
	return get_unit_owner(game, unit);
}

static void __fastcall unit_pet_iterator(diablo2::structures::game* game, diablo2::structures::unit* owner,
										 diablo2::structures::unit* unit, void* arg) {
	const std::function<void(diablo2::structures::game*, diablo2::structures::unit*, diablo2::structures::unit*)>* cb =
		reinterpret_cast<decltype(cb)>(arg);
	cb->operator()(game, owner, unit);
}

void* diablo2::d2_game::iterate_unit_pets(structures::game* game, structures::unit* unit,
										  const std::function<void(structures::game*, structures::unit*, diablo2::structures::unit*)>& cb) {
	static wrap_func_fast<void* (structures::game*, structures::unit*,
								 void(__fastcall*)(structures::game*, structures::unit*, diablo2::structures::unit*, void*), void*)> iterate_unit_pets(0x4E7C0, get_base());
	const auto cbref = &cb;
	// ReSharper disable once CppCStyleCast
	return iterate_unit_pets(game, unit, unit_pet_iterator, (void*)cbref);
}
