#include <common.h>

#include <Windows.h>
#include <MinHook.h>

#include <spdlog/spdlog.h>

#include <common/d2protocol.h>

static char* d2_net = reinterpret_cast<char*>(GetModuleHandle("d2net.dll"));

static item_mover::common* g_common;

static int32_t(__fastcall* g_get_packet_size_client_to_server)(d2_protocol::packet_header* data, size_t size, size_t* sizeOut);
static char(__fastcall* g_get_packet_size_server_to_client)(d2_protocol::packet_header* data, size_t size, size_t* outSize);

static int32_t __fastcall get_packet_size_client_to_server(d2_protocol::packet_header* data, size_t size, size_t* sizeOut) {
	static d2_protocol::packet_header dummy;

	if (data->d2_packet_type == dummy.d2_packet_type) {
		size_t packetSize;

		if (!g_common->get_packet_size_cs(data, packetSize))
			return g_get_packet_size_client_to_server(data, size, sizeOut);

		*sizeOut = packetSize;
		return packetSize;
	}

	return g_get_packet_size_client_to_server(data, size, sizeOut);
}

static char __fastcall get_packet_size_server_to_client(d2_protocol::packet_header* data, size_t size, size_t* sizeOut) {
	static d2_protocol::packet_header dummy;

	if (data->d2_packet_type == dummy.d2_packet_type) {
		size_t packetSize;

		if (!g_common->get_packet_size_sc(data, packetSize))
			return g_get_packet_size_server_to_client(data, size, sizeOut);

		*sizeOut = packetSize;
		return packetSize;
	}

	return g_get_packet_size_server_to_client(data, size, sizeOut);
}

item_mover::common::common() {
	g_common = this;
}

void item_mover::common::init() {
	MH_CreateHook(reinterpret_cast<void*>(d2_net + 0x1B60), get_packet_size_server_to_client, reinterpret_cast<void**>(&g_get_packet_size_server_to_client));
	MH_CreateHook(reinterpret_cast<void*>(d2_net + 0x1E60), get_packet_size_client_to_server, reinterpret_cast<void**>(&g_get_packet_size_client_to_server));
}

bool item_mover::common::get_packet_size_cs(d2_protocol::packet_header* packet, size_t& size) {
	switch (packet->message_type) {
		case d2_protocol::MESSAGE_TYPE_ITEM_MOVE:
		{
			size = sizeof d2_protocol::item_move_cs;
			return true;
		}
		default:
			return false;
	}
}

bool item_mover::common::get_packet_size_sc(d2_protocol::packet_header* packet, size_t& size) {
	switch (packet->message_type) {
		case d2_protocol::MESSAGE_TYPE_ITEM_MOVE:
		{
			size = sizeof d2_protocol::item_move_sc;
			return true;
		}
		default:
			return false;
	}
}
