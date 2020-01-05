#include <d2tweaks/server/modules/server_module.h>

#include <d2tweaks/server/server.h>

d2_tweaks::server::modules::server_module::server_module() {
	singleton<server>::instance().register_module(this);
}

bool d2_tweaks::server::modules::server_module::handle_packet(diablo2::structures::game* game,
															  diablo2::structures::unit* player, common::packet_header* packet) {
	return false;
}

void d2_tweaks::server::modules::server_module::tick() {}
