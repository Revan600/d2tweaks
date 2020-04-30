#include <d2tweaks/client/modules/client_module.h>

#include <d2tweaks/client/client.h>

d2_tweaks::client::modules::client_module::client_module() {
	singleton<client>::instance().register_module(this);
}

void d2_tweaks::client::modules::client_module::draw_ui() {}

void d2_tweaks::client::modules::client_module::tick() {}

void d2_tweaks::client::modules::client_module::handle_packet(common::packet_header* packet) {}
