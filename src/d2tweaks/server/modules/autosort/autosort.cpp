#include <d2tweaks/server/modules/autosort/autosort.h>

#include <d2tweaks/server/server.h>

#include <vector>
#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>

#include <diablo2/d2common.h>
#include <diablo2/d2game.h>

#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/items_line.h>
#include <diablo2/structures/player_data.h>

MODULE_INIT(autosort)

struct backup_item {
	diablo2::structures::unit* item;
	uint8_t x;
	uint8_t y;

	backup_item() : item(nullptr), x(0), y(0) {}
};

struct packed_area {
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
};

void d2_tweaks::server::modules::autosort::init() {
	singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_INVENTORY_SORT, this);
}

bool d2_tweaks::server::modules::autosort::handle_packet(diablo2::structures::game* game,
														 diablo2::structures::unit* player, common::packet_header* packet) {
	sort(game, player, static_cast<common::inventory_sort_cs*>(packet)->page);
	return true;
}

bool d2_tweaks::server::modules::autosort::sort(diablo2::structures::game* game, diablo2::structures::unit* player, uint8_t page) {
	static common::inventory_sort_sc packet;
	static auto& instance = singleton<server>::instance();

	if (player == nullptr)
		return false;

	const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, page, game->item_format == 101);
	char data[0x18];
	diablo2::d2_common::get_inventory_data(inventoryIndex, 0, data);

	const auto inventoryWidth = static_cast<uint32_t>(data[0]);
	const auto inventoryHeight = static_cast<uint32_t>(data[1]);

	std::vector<packed_area> areas{ {0,0, static_cast<uint8_t>(inventoryWidth), static_cast<uint8_t>(inventoryHeight)} };
	std::vector<diablo2::structures::unit*> items;
	std::vector<backup_item> backup_items;
	std::unordered_map<uint32_t, std::vector<diablo2::structures::unit*>> items_typed;
	uint32_t occupied_cells = 0;

	for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
		if (item->item_data->page != page)
			continue;

		const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

		backup_item backup;
		backup.item = item;
		backup.x = item->path->x;
		backup.y = item->path->y;

		backup_items.push_back(backup);

		items_typed[item->data_record_index].push_back(item);
		items.push_back(item);

		occupied_cells += record->inv_height * record->inv_width;
	}

	if (occupied_cells > inventoryHeight* inventoryWidth) {
		//should never be happen in normal cases
		spdlog::warn("occupied_cells > inventoryHeight* inventoryWidth");
		return false;
	}

	for (auto item : items)
		diablo2::d2_common::inv_remove_item(player->inventory, item);

	const auto itemsCount = items.size();

	if (itemsCount == 0)
		return true; //there's nothing to sort

	//sort by height*width
	for (size_t i = 0; i < itemsCount - 1; i++) {
		auto swapped = false;
		for (size_t j = 0; j < itemsCount - i - 1; j++) {
			const auto record1 = diablo2::d2_common::get_item_record(items[j]->data_record_index);
			const auto record2 = diablo2::d2_common::get_item_record(items[j + 1]->data_record_index);

			if (record1->inv_height * record1->inv_width > record2->inv_height* record2->inv_width)
				continue;

			const auto temp = items[j];
			items[j] = items[j + 1];
			items[j + 1] = temp;
			swapped = true;
		}

		if (swapped == false)
			break;
	}

	auto success = true;

	for (auto sorted_item : items) {
		for (auto& item : items_typed[sorted_item->data_record_index]) { //iterate through all items of the same type for grouping
			const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

			uint32_t tx, ty;

			if (!find_free_space(player->inventory, item, inventoryIndex, page, tx, ty)) {
				success = false;
				break;
			}

			if (!diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, page)) {
				success = false;
				break;
			}

			if (diablo2::d2_common::inv_update_item(player->inventory, item, false))
				continue;

			success = false;
			break;
		}

		if (!success)
			break;

		items_typed.erase(sorted_item->data_record_index);
	}

	if (success) {
		while (items.size() > 0) {
			packet.page = page;
			packet.tx = items.back()->path->x;
			packet.ty = items.back()->path->y;
			packet.guid = items.back()->guid;

			items.pop_back();

			instance.send_packet(player->player_data->net_client, &packet, sizeof packet);
		}

		diablo2::d2_game::update_inventory_items(game, player);
		return true;
	}

	//sorting failed, remove all items and re-insert them at old coords

	for (auto& backup_item : backup_items) {
		const auto item = backup_item.item;

		diablo2::d2_common::inv_remove_item(player->inventory, item);
	}

	while (backup_items.size() > 0) {
		const auto backup = backup_items.back();
		const auto item = backup.item;
		const auto tx = backup.x;
		const auto ty = backup.y;

		packet.page = page;
		packet.tx = tx;
		packet.ty = ty;
		packet.guid = item->guid;

		diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, page);
		diablo2::d2_common::inv_update_item(player->inventory, item, false);

		backup_items.pop_back();

		instance.send_packet(player->player_data->net_client, &packet, sizeof packet);
	}

	diablo2::d2_game::update_inventory_items(game, player);

	return false;
}

bool d2_tweaks::server::modules::autosort::find_free_space(diablo2::structures::inventory* inv,
														   diablo2::structures::unit* item,
														   int32_t inventoryIndex, char page, uint32_t& x,
														   uint32_t& y) {
	char data[0x18];

	diablo2::d2_common::get_inventory_data(inventoryIndex, 0, data);

	const auto mx = static_cast<uint32_t>(data[0]);
	const auto my = static_cast<uint32_t>(data[1]);

	for (x = 0; x < mx; x++) {
		for (y = 0; y < my; y++) {
			diablo2::structures::unit* blockingUnit = nullptr;
			uint32_t blockingUnitIndex = 0;

			if (diablo2::d2_common::can_put_into_slot(inv, item, x, y, inventoryIndex, &blockingUnit, &blockingUnitIndex, page))
				return true;
		}
	}

	return false;
}
