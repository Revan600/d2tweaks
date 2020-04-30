#include <d2tweaks/common/common.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <diablo2/d2net.h>
#include <common/hooking.h>

static int32_t(__fastcall* g_get_packet_size_client_to_server)(d2_tweaks::common::packet_header* data, size_t size, size_t* sizeOut);
static char(__fastcall* g_get_packet_size_server_to_client)(d2_tweaks::common::packet_header* data, size_t size, size_t* outSize);

static int32_t __fastcall get_packet_size_client_to_server(d2_tweaks::common::packet_header* data, size_t size, size_t* sizeOut) {
	static d2_tweaks::common::packet_header dummy;
	static auto& instance = singleton<d2_tweaks::common::common>::instance();

	if (data->d2_packet_type == dummy.d2_packet_type) {
		size_t packetSize;

		if (!instance.get_packet_size_cs(data, packetSize))
			return g_get_packet_size_client_to_server(data, size, sizeOut);

		*sizeOut = packetSize;
		return packetSize;
	}

	return g_get_packet_size_client_to_server(data, size, sizeOut);
}

static char __fastcall get_packet_size_server_to_client(d2_tweaks::common::packet_header* data, size_t size, size_t* sizeOut) {
	static d2_tweaks::common::packet_header dummy;
	static auto& instance = singleton<d2_tweaks::common::common>::instance();

	if (data->d2_packet_type == dummy.d2_packet_type) {
		size_t packetSize;

		if (!instance.get_packet_size_sc(data, packetSize))
			return g_get_packet_size_server_to_client(data, size, sizeOut);

		*sizeOut = packetSize;
		return static_cast<char>(packetSize);
	}

	return g_get_packet_size_server_to_client(data, size, sizeOut);
}

d2_tweaks::common::common::common(token) {}

void d2_tweaks::common::common::init() {
	singleton<asset_manager>::instance().init();

	hooking::hook(reinterpret_cast<void*>(diablo2::d2_net::get_base() + 0x1B60), get_packet_size_server_to_client, reinterpret_cast<void**>(&g_get_packet_size_server_to_client));
	hooking::hook(reinterpret_cast<void*>(diablo2::d2_net::get_base() + 0x1E60), get_packet_size_client_to_server, reinterpret_cast<void**>(&g_get_packet_size_client_to_server));
}

bool d2_tweaks::common::common::get_packet_size_cs(packet_header* packet, size_t& size) {
	switch (packet->message_type) {
		case MESSAGE_TYPE_ITEM_MOVE:
		{
			size = sizeof item_move_cs;
			return true;
		}
		case MESSAGE_TYPE_INVENTORY_SORT:
		{
			size = sizeof inventory_sort_cs;
			return true;
		}
		case MESSAGE_TYPE_DAMAGE_INFO:
		{
			size = sizeof damage_info_cs;
			return true;
		}
		case MESSAGE_TYPE_GOLD_PICKUP_INFO:
		{
			size = sizeof gold_pickup_info_sc;
			return true;
		}
		default:
			return false;
	}
}

bool d2_tweaks::common::common::get_packet_size_sc(packet_header* packet, size_t& size) {
	switch (packet->message_type) {
		case MESSAGE_TYPE_ITEM_MOVE:
		{
			size = sizeof item_move_sc;
			return true;
		}
		case MESSAGE_TYPE_INVENTORY_SORT:
		{
			size = sizeof inventory_sort_sc;
			return true;
		}
		case MESSAGE_TYPE_DAMAGE_INFO:
		{
			size = sizeof damage_info_sc;
			return true;
		}
		case MESSAGE_TYPE_GOLD_PICKUP_INFO:
		{
			size = sizeof gold_pickup_info_sc;
			return true;
		}
		default:
			return false;
	}
}
