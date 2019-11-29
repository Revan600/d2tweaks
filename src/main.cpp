#include <Windows.h>

#include <MinHook.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

#include <d2common.h>
#include <d2function.h>

char* d2_client = reinterpret_cast<char*>(GetModuleHandle("d2client.dll"));
char* d2_common = reinterpret_cast<char*>(GetModuleHandle("d2common.dll"));
char* d2_game = reinterpret_cast<char*>(GetModuleHandle("d2game.dll"));

game** g_game_ptr = reinterpret_cast<game**>(d2_client + 0x107834);

unit* (__stdcall* get_player)();

static d2_func_std_import<int32_t(unit * item, char page, int a3)> get_unk_stat_offset(10409, d2_common);
static d2_func_std_import < uint32_t(inventory * inv, unit * item, uint32_t x, uint32_t y,
	uint32_t unkStatOffset, unit * *lastBlockingUnit, uint32_t * lastBlockingUnitIndex, uint8_t page)> can_put_into_slot(
		10247, d2_common);
static d2_func_std_import<BOOL(inventory * inv, unit * item, uint32_t x, uint32_t y,
	uint32_t unkStatOffset, BOOL isClient, uint8_t page)> inv_add_item(
		10249, d2_common);
static d2_func_std_import<BOOL(inventory * inv, unit * item, BOOL isClient)> inv_update_item(10242, d2_common);

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
	const auto logPath = "easylife.log";
	const auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath);

	// ReSharper disable once CppSmartPointerVsMakeFunction
	auto logger = std::shared_ptr<spdlog::logger>(new spdlog::logger("easylife", { file, console_err }));

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

//Somehow, local (I don't know how to call them properly) units doesn't seem to have pointer to game structure
//it looks like they are some kind of facades or just clientside dummies (need further investigation)
unit* get_net_unit(unit* localUnit) {
	if (localUnit == nullptr)
		return nullptr;

	auto pGame = *g_game_ptr;
	auto typeIndex = localUnit->unit_type;

	if (localUnit->unit_type == 3)
		typeIndex = 4;

	if (localUnit->unit_type == 4)
		typeIndex = 3;

	const auto unitCount = pGame->unit_counts[localUnit->unit_type];
	const auto index = localUnit->item_num & 127;

	auto result = pGame->unit_list[typeIndex][index];

	while (result != nullptr && result->item_num != localUnit->item_num) {
		result = result->unit1;
	}

	return result;
}

bool find_free_space(inventory* inv, unit* item, int32_t statOffset, char page, uint32_t& x, uint32_t& y) {
	//15x15 max page size because I'm too lazy to implement proper page size fetching
	const auto mx = 15;
	const auto my = 15;

	for (x = 0; x < mx; x++) {
		for (y = 0; y < my; y++) {
			unit* blockingUnit = nullptr;
			uint32_t blockingUnitIndex = 0;

			if (can_put_into_slot(inv, item, x, y, statOffset, &blockingUnit, &blockingUnitIndex, page))
				return true;
		}
	}

	return false;
}

int32_t __fastcall item_click(unit* playerUnit, inventory* inventory, int mouse_x, int mouse_y, uint8_t flag, char* a6, unsigned int page) {
	if ((static_cast<uint16_t>(GetAsyncKeyState(VK_CONTROL)) >> 8 & 0x80u) == 0)
		return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);

	if (!does_cube_window_open() && !does_stash_window_open())
		return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);

	const auto player = get_player();

	//code below taken from IDA directly, so that's why it is so ugly
	const auto coefx1 = *(reinterpret_cast<uint32_t*>(a6) + 1);
	const auto coefx2 = static_cast<unsigned int>(a6[20]);

	const auto coefy1 = *(reinterpret_cast<uint32_t*>(a6) + 3);
	const auto coefy2 = static_cast<unsigned int>(a6[21]);

	const auto itemx = (mouse_x - coefx1) / coefx2;
	const auto itemy = (mouse_y - coefy1) / coefy2;

	unit* clickedItem = nullptr;
	unit* cubeItem = nullptr;

	for (auto item = player->inventory->pt_first_item; item != nullptr; item = item->item_data->pt_next_item) {
		if (item->txt_file_no == 561) { //Cube
			cubeItem = item;
		}

		if (item->item_data->page == page && item->path->x == itemx && item->path->y == itemy)
			clickedItem = item;
	}

	if (clickedItem == nullptr || cubeItem == nullptr)
		return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);

	const auto targetPage = get_target_page(page);

	if (targetPage == 0x03 && clickedItem == cubeItem)
		return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);

	const auto netPlayer = get_net_unit(player);
	const auto netItem = get_net_unit(clickedItem);

	if (!netPlayer || !netItem)
		return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);

	const auto netStatOffset = get_unk_stat_offset(netPlayer, targetPage, *reinterpret_cast<int*>(0x6FBA77C4));
	const auto statOffset = get_unk_stat_offset(player, targetPage, *reinterpret_cast<int*>(0x6FBA77C4));

	uint32_t tx, ty;

	if (!find_free_space(player->inventory, clickedItem, statOffset, targetPage, tx, ty))
		return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);

	netItem->item_data->page = targetPage;
	clickedItem->item_data->page = targetPage;

	inv_add_item(netPlayer->inventory, netItem, tx, ty, netStatOffset, true, netItem->item_data->page);
	inv_add_item(player->inventory, clickedItem, tx, ty, statOffset, true, clickedItem->item_data->page);

	inv_update_item(netPlayer->inventory, netItem, true);
	inv_update_item(player->inventory, clickedItem, true);

	return g_item_click_original(playerUnit, inventory, mouse_x, mouse_y, flag, a6, page);
}

extern "C" {
	// ReSharper disable once CppInconsistentNaming
	__declspec(dllexport) void __stdcall Init() {
		init_log();

		if (MH_Initialize() != MH_OK) {
			MessageBox(nullptr, "Cannot initialize hook system!", "Error", MB_OK | MB_ICONSTOP);
			exit(0);
		}

		spdlog::debug("item_click: {0}", static_cast<void*>(d2_client + 0x475C0));

		MH_CreateHook(d2_client + 0x475C0, item_click, reinterpret_cast<void**>(&g_item_click_original));

		get_player = reinterpret_cast<decltype(get_player)>(d2_client + 0x883D0);

		MH_EnableHook(nullptr);
	}
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
