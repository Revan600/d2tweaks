#include <d2tweaks/client/client.h>

#include <common/hooking.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>
#include <d2tweaks/client/modules/client_module.h>
#include <d2tweaks/ui/ui_manager.h>

#include <diablo2/d2client.h>

#include <diablo2/structures/unit.h>
#include <diablo2/structures/client_unit_list.h>

static void(__fastcall* g_handle_packet)(d2_tweaks::common::packet_header* packet, size_t size);
static int32_t(__stdcall* g_draw_game_ui_original)();
static int32_t(__fastcall* g_game_tick_original)(int32_t a1);

d2_tweaks::client::client::client(token) {
	m_module_id_counter = 0;
	m_tick_handler_id_counter = 0;
}

void d2_tweaks::client::client::init() {
	hooking::hook(diablo2::d2_client::get_base() + 0x11CB0, handle_packet, reinterpret_cast<void**>(&g_handle_packet));
	hooking::hook(diablo2::d2_client::get_base() + 0x9640, game_tick, reinterpret_cast<void**>(&g_game_tick_original));
	hooking::hook(diablo2::d2_client::get_base() + 0x5E650, draw_game_ui, reinterpret_cast<void**>(&g_draw_game_ui_original));

	for (size_t i = 0; i < sizeof m_modules / sizeof(void*); i++) {
		if (m_modules[i] == nullptr)
			break;

		m_modules[i]->init();
	}
}

void d2_tweaks::client::client::handle_packet(common::packet_header* packet, size_t size) {
	static common::packet_header dummy;
	static auto& instance = singleton<client>::instance();

	if (size == -1)
		return;

	if (packet->d2_packet_type != dummy.d2_packet_type) {
		g_handle_packet(packet, size);
		return;
	}

	auto handler = instance.m_packet_handlers[packet->message_type];

	if (!handler)
		return;

	handler->handle_packet(packet);
}

int32_t d2_tweaks::client::client::game_tick(int32_t a1) {
	static auto& instance = singleton<client>::instance();

	for (size_t i = 0; i < sizeof instance.m_modules / sizeof(void*); i++) {
		if (instance.m_tick_handlers[i] == nullptr)
			break;

		instance.m_tick_handlers[i]->tick();
	}

	return g_game_tick_original(a1);
}

int32_t d2_tweaks::client::client::draw_game_ui() {
	static auto& ui = singleton<ui::ui_manager>::instance();

	const auto result = g_draw_game_ui_original();

	ui.draw();

	return result;
}

void d2_tweaks::client::client::register_module(modules::client_module* module) {
	m_modules[m_module_id_counter++] = module;
}

void d2_tweaks::client::client::register_tick_handler(modules::client_module* module) {
	m_tick_handlers[m_tick_handler_id_counter++] = module;
}

void d2_tweaks::client::client::register_packet_handler(common::message_types_t type, modules::client_module* module) {
	if (m_packet_handlers[type] != nullptr) {
		spdlog::warn("Clientside packet handler for {0} is already registered!", type);
	}

	m_packet_handlers[type] = module;
}

diablo2::structures::unit* d2_tweaks::client::client::get_client_unit(uint32_t type, uint32_t guid) {
	static auto units = diablo2::d2_client::get_client_unit_list();

	const auto index = guid & 127;

	auto result = units->unit_list[type][index];

	while (result != nullptr && result->guid != guid) {
		result = result->prev_unit;
	}

	return result;
}
