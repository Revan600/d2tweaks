#include <diablo2/d2common.h>
#include <common/ptr_wrapper.h>

char* diablo2::d2_common::get_base() {
	static auto base = reinterpret_cast<char*>(GetModuleHandle("d2common.dll"));
	return base;
}

int32_t diablo2::d2_common::get_inventory_index(structures::unit* item, char page, BOOL lod) {
	static wrap_func_std_import<int32_t(structures::unit*, char, BOOL)> get_inventory_index(10409, get_base());
	return get_inventory_index(item, page, lod);
}

void* diablo2::d2_common::get_inventory_data(int32_t index, int32_t zero, char* data) {
	static wrap_func_std_import<void* (int32_t, int32_t, char*)> get_inventory_data(10636, get_base());
	return get_inventory_data(index, zero, data);
}

diablo2::structures::unit* diablo2::d2_common::get_item_at_cell(structures::inventory* inv, uint32_t cellx,
																uint32_t celly, uint32_t* pcellx, uint32_t* pcelly, int32_t invIndex, uint8_t page) {
	static wrap_func_std_import<structures::unit * (structures::inventory*, uint32_t, uint32_t, uint32_t*, uint32_t*,
													int32_t, uint8_t)> get_item_at_cell(10252, get_base());
	return get_item_at_cell(inv, cellx, celly, pcellx, pcelly, invIndex, page);
}

uint32_t diablo2::d2_common::can_put_into_slot(structures::inventory* inv, structures::unit* item, uint32_t x,
											   uint32_t y, uint32_t invIndex, structures::unit** lastBlockingUnit, uint32_t* lastBlockingUnitIndex, uint8_t page) {
	static wrap_func_std_import< uint32_t(structures::inventory*, structures::unit*, uint32_t, uint32_t,
										  uint32_t, structures::unit**, uint32_t*, uint8_t)> can_put_into_slot(
											  10247, get_base());
	return can_put_into_slot(inv, item, x, y, invIndex, lastBlockingUnit, lastBlockingUnitIndex, page);
}

diablo2::structures::unit* diablo2::d2_common::inv_remove_item(structures::inventory* inventory, structures::unit* item) {
	static wrap_func_std_import<structures::unit * (structures::inventory*, structures::unit*)> inv_remove_item(10243, get_base());
	return inv_remove_item(inventory, item);
}

BOOL diablo2::d2_common::inv_add_item(structures::inventory* inv, structures::unit* item, uint32_t x, uint32_t y,
									  uint32_t invIndex, BOOL isClient, uint8_t page) {
	static wrap_func_std_import<BOOL(structures::inventory*, structures::unit*, uint32_t, uint32_t,
									 uint32_t, BOOL, uint8_t)> inv_add_item(10249, get_base());

	return inv_add_item(inv, item, x, y, invIndex, isClient, page);
}

BOOL diablo2::d2_common::inv_update_item(structures::inventory* inv, structures::unit* item, BOOL isClient) {
	static wrap_func_std_import<BOOL(structures::inventory*, structures::unit*, BOOL)> inv_update_item(10242, get_base());
	return inv_update_item(inv, item, isClient);
}

diablo2::structures::items_line* diablo2::d2_common::get_item_record(uint32_t guid) {
	static wrap_func_std_import<structures::items_line * (uint32_t)> get_item_record(10600, get_base());
	return get_item_record(guid);
}

diablo2::structures::item_types_line* diablo2::d2_common::get_item_type_record(uint32_t typeId) {
	static wrap_func_fast<structures::item_types_line * (uint32_t)>get_item_type_record(0x2B1A0, get_base());
	return get_item_type_record(typeId);
}

uint32_t diablo2::d2_common::get_maximum_character_gold(structures::unit* player) {
	static wrap_func_std_import<uint32_t(structures::unit*)> get_maximum_character_gold(10439, get_base());
	return get_maximum_character_gold(player);
}

int32_t diablo2::d2_common::set_stat(structures::unit* unit, unit_stats_t stat, uint32_t value, int16_t param) {
	static wrap_func_std_import<int32_t(structures::unit*, int32_t, int32_t, int32_t)> set_stat(10517, get_base());
	return set_stat(unit, stat, value, param);
}

int32_t diablo2::d2_common::get_stat(structures::unit* unit, unit_stats_t stat, int16_t param) {
	static wrap_func_std_import<int32_t(structures::unit*, int32_t, int32_t)> get_stat(10519, get_base());
	return get_stat(unit, stat, param);
}

int32_t diablo2::d2_common::get_stat_signed(structures::unit* unit, unit_stats_t stat, int16_t param) {
	static wrap_func_std_import<int32_t(structures::unit*, int32_t, int32_t)> get_stat_signed(10520, get_base());
	return get_stat_signed(unit, stat, param);
}

int32_t diablo2::d2_common::_10111(int32_t* x, int32_t* y) {
	static wrap_func_std_import<int32_t(int32_t*, int32_t*)> get_unk_coords(10111, get_base());
	return get_unk_coords(x, y);
}

int32_t diablo2::d2_common::_10116(int32_t x1, int32_t y1, int32_t* x, int32_t* y) {
	static wrap_func_std_import<int32_t(int32_t, int32_t, int32_t*, int32_t*)> get_unk_coords2(10116, get_base());
	return get_unk_coords2(x1, y1, x, y);
}

diablo2::structures::room* diablo2::d2_common::get_room_from_unit(structures::unit* unit) {
	static wrap_func_std_import<structures::room * (structures::unit*)> get_room_from_unit(10342, get_base());
	return get_room_from_unit(unit);
}

int32_t diablo2::d2_common::get_unit_size_x(structures::unit* unit) {
	static wrap_func_std_import<int32_t(structures::unit*)> get_unit_size_x(10336, get_base());
	return get_unit_size_x(unit);
}

int32_t diablo2::d2_common::get_unit_size_y(structures::unit* unit) {
	static wrap_func_std_import<int32_t(structures::unit*)> get_unit_size_y(10337, get_base());
	return get_unit_size_y(unit);
}

int32_t diablo2::d2_common::get_distance_between_units(structures::unit* unit1, structures::unit* unit2) {
	static wrap_func_std_import<int32_t(structures::unit*, structures::unit*)> get_distance_between_units(10397, get_base());
	return get_distance_between_units(unit1, unit2);
}

int32_t diablo2::d2_common::get_unit_x(structures::path* path) {
	static wrap_func_std_import<int32_t(structures::path*)> get_unit_x(10162, get_base());
	return get_unit_x(path);
}

int32_t diablo2::d2_common::get_unit_y(structures::path* path) {
	static wrap_func_std_import<int32_t(structures::path*)> get_unit_y(10163, get_base());
	return get_unit_y(path);
}

int32_t diablo2::d2_common::get_unit_precise_x(structures::unit* unit) {
	static wrap_func_std_import<int32_t(structures::unit*)> get_unit_precise_x(10327, get_base());
	return get_unit_precise_x(unit);
}

int32_t diablo2::d2_common::get_unit_precise_y(structures::unit* unit) {
	static wrap_func_std_import<int32_t(structures::unit*)> get_unit_precise_y(10330, get_base());
	return get_unit_precise_y(unit);
}

diablo2::structures::unit* diablo2::d2_common::get_target_from_path(structures::path* path) {
	static wrap_func_std_import<structures::unit * (structures::path*)> get_target_from_path(10180, get_base());
	return get_target_from_path(path);
}
