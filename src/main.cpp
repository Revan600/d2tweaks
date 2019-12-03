#include <Windows.h>

#include <MinHook.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

#include <common/d2common.h>
#include <common/d2function.h>
#include <common/d2protocol.h>

#include <common.h>
#include <server.h>
#include <client.h>

//Uncomment on of the defines that corresponds to your mod loader
//#define PLUGY
//OR
//#define D2_MOD

#include <loader.h>

char* d2_client = reinterpret_cast<char*>(GetModuleHandle("d2client.dll"));
char* d2_common = reinterpret_cast<char*>(GetModuleHandle("d2common.dll"));
char* d2_game = reinterpret_cast<char*>(GetModuleHandle("d2game.dll"));
char* d2_net = reinterpret_cast<char*>(GetModuleHandle("d2net.dll"));

static d2_func_fast<void(void*, size_t)> send_to_server(reinterpret_cast<void*>(0xD850), d2_client);
static d2_func_std_import<int32_t(int32_t queue, void*, size_t)> net_send_to_server(10005, d2_net);

static d2_func_std_import<int32_t(unit * item, char page, BOOL lod)> get_inventory_index(10409, d2_common);
static d2_func_std_import<unit * (::inventory * inv, uint32_t cellx, uint32_t celly, uint32_t * pcellx, uint32_t * pcelly,
	int32_t invIndex, uint8_t page)> get_item_at_cell(10252, d2_common);
static d2_func_std_import<items_line * (uint32_t id)> get_item_record(10600, d2_common);

bool is_only_inventory_open() {
	return *reinterpret_cast<int32_t*>(d2_client + 0x11A6AC) > 0;
}

bool does_trade_window_open() {
	return *reinterpret_cast<int32_t*>(d2_client + 0x11A6D8) > 0;
}

bool does_stash_window_open() {
	return *reinterpret_cast<int32_t*>(d2_client + 0x11A70C) > 0;
}

bool does_cube_window_open() {
	return *reinterpret_cast<int32_t*>(d2_client + 0x11A710) > 0;
}

void init_log() {
	const auto console_err = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
	const auto logPath = "item_mover.log";
	const auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath);

	// ReSharper disable once CppSmartPointerVsMakeFunction
	auto logger = std::shared_ptr<spdlog::logger>(new spdlog::logger("item_mover", { file, console_err }));

	logger->flush_on(spdlog::level::trace);

	set_default_logger(logger);

#ifndef NDEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	spdlog::info("Log system initialized");
}

int32_t(__fastcall* g_item_click_original)(unit* playerUnit, inventory* inventory, int mouse_x, int mouse_y, uint8_t flag, void* a6, unsigned int page);

char get_target_page(char currentPage) {
	if (currentPage == 0) { //item is in inventory
		if (does_stash_window_open())
			return 4;

		if (does_cube_window_open())
			return 3;
	}

	return 0;
}

void request_item_move(unit* item, char targetPage) {
	static d2_protocol::item_move_cs packet;

	packet.item_guid = item->item_num;
	packet.target_page = targetPage;
	send_to_server(&packet, sizeof packet);
}

int32_t __fastcall item_click(unit* player, inventory* inventory, int mouse_x, int mouse_y, uint8_t flag, char* a6, unsigned int page) {
	if ((static_cast<uint16_t>(GetAsyncKeyState(VK_CONTROL)) >> 8 & 0x80u) == 0)
		return g_item_click_original(player, inventory, mouse_x, mouse_y, flag, a6, page);

	if (!does_cube_window_open() && !does_stash_window_open())
		return g_item_click_original(player, inventory, mouse_x, mouse_y, flag, a6, page);

	//code below taken from IDA directly, so that's why it is so ugly
	const auto coefx1 = *(reinterpret_cast<uint32_t*>(a6) + 1);
	const auto coefx2 = static_cast<unsigned int>(a6[20]);

	const auto coefy1 = *(reinterpret_cast<uint32_t*>(a6) + 3);
	const auto coefy2 = static_cast<unsigned int>(a6[21]);

	const auto itemx = (mouse_x - coefx1) / coefx2;
	const auto itemy = (mouse_y - coefy1) / coefy2;

	unit* cubeItem = nullptr;

	uint32_t px, py;

	const auto currentInventoryIndex = get_inventory_index(player, page, *reinterpret_cast<int*>(d2_client + 0x1077C4));
	const auto clickedItem = get_item_at_cell(player->inventory, itemx, itemy, &px, &py, currentInventoryIndex, page);

	for (auto item = player->inventory->pt_first_item; item != nullptr; item = item->item_data->pt_next_item) {
		auto record = get_item_record(item->txt_file_no);

		if (record->szcode[0] == 'b' &&
			record->szcode[1] == 'o' &&
			record->szcode[2] == 'x') { //Cube
			cubeItem = item;
			break;
		}
	}

	if (clickedItem == nullptr || cubeItem == nullptr)
		return g_item_click_original(player, inventory, mouse_x, mouse_y, flag, a6, page);

	const auto targetPage = get_target_page(page);

	if (targetPage == 0x03 && clickedItem == cubeItem)
		return g_item_click_original(player, inventory, mouse_x, mouse_y, flag, a6, page);

	request_item_move(clickedItem, targetPage);
	return 0;
}

static item_mover::common g_common;
static item_mover::server g_server;
static item_mover::client g_client;

extern "C" {
	EP_HEADER
		init_log();

	if (MH_Initialize() != MH_OK) {
		MessageBox(nullptr, "Cannot initialize hook system!", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}

	spdlog::debug("item_click: {0}", static_cast<void*>(d2_client + 0x475C0));

	MH_CreateHook(d2_client + 0x475C0, item_click, reinterpret_cast<void**>(&g_item_click_original));

	g_common.init();
	g_server.init(&g_common);
	g_client.init();

	MH_EnableHook(nullptr);
	EP_FOOTER
}

// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(HMODULE moduleHandle,
					  DWORD reason,
					  LPVOID reserved) {
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (reason) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		default:
			break;
	}
	return true;
}
