#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_menu.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings.h>

#include <common/hooking.h>
#include <common/asm_code.h>

#include <Windows.h>

#include <diablo2/d2client.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>

#include <d2tweaks/ui/controls/checkbox.h>

d2_tweaks::client::modules::loot_filter_settings_menu::loot_filter_settings_menu(token) {
	menu::set_enabled(false);
	menu::set_visible(false);

	load_xml("d2tweaks\\interfaces\\loot_filter_settings_menu.xml");

	register_misc_checkboxes();
	register_quality_checkboxes();

	setup_hooks();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::reload_settings() {
	register_misc_checkboxes();
	register_quality_checkboxes();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::draw() {
	if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MAINMENU))
		return;

	menu::draw();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::register_misc_checkboxes() {
	m_altonly = get_control<ui::controls::checkbox>("m_altonly");
	m_show_gold = get_control<ui::controls::checkbox>("m_show_gold");
	m_show_runes = get_control<ui::controls::checkbox>("m_show_runes");
	m_show_gems = get_control<ui::controls::checkbox>("m_show_gems");

	if (m_altonly) {
		m_altonly->set_state(loot_filter_settings::get().alt_only);
		m_altonly->set_on_click(std::bind(&loot_filter_settings_menu::update_alt_only,
										  this, std::placeholders::_1));
	}

	if (m_show_gold) {
		m_show_gold->set_state(loot_filter_settings::get().show_gold);
		m_show_gold->set_on_click(std::bind(&loot_filter_settings_menu::update_show_gold,
											this, std::placeholders::_1));
	}

	if (m_show_runes) {
		m_show_runes->set_state(loot_filter_settings::get().show_runes);
		m_show_runes->set_on_click(std::bind(&loot_filter_settings_menu::update_show_runes,
											 this, std::placeholders::_1));
	}

	if (m_show_gems) {
		m_show_gems->set_state(loot_filter_settings::get().show_gems);
		m_show_gems->set_on_click(std::bind(&loot_filter_settings_menu::update_show_gems,
											this, std::placeholders::_1));
	}
}

void d2_tweaks::client::modules::loot_filter_settings_menu::register_quality_checkboxes() {
	using namespace diablo2::structures;

	register_quality_checkbox("m_inferior", item_quality_t::ITEM_QUALITY_INFERIOR);
	register_quality_checkbox("m_normal", item_quality_t::ITEM_QUALITY_NORMAL);
	register_quality_checkbox("m_superior", item_quality_t::ITEM_QUALITY_SUPERIOR);
	register_quality_checkbox("m_magic", item_quality_t::ITEM_QUALITY_MAGIC);
	register_quality_checkbox("m_set", item_quality_t::ITEM_QUALITY_SET);
	register_quality_checkbox("m_rare", item_quality_t::ITEM_QUALITY_RARE);
	register_quality_checkbox("m_unique", item_quality_t::ITEM_QUALITY_UNIQUE);
	register_quality_checkbox("m_crafted", item_quality_t::ITEM_QUALITY_CRAFTED);
	register_quality_checkbox("m_tempered", item_quality_t::ITEM_QUALITY_TEMPERED);
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_alt_only(bool value) {
	loot_filter_settings::get().alt_only = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_show_gold(bool value) {
	loot_filter_settings::get().show_gold = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_show_runes(bool value) {
	loot_filter_settings::get().show_runes = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_show_gems(bool value) {
	loot_filter_settings::get().show_gems = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_quality_allowance(bool value,
																					 diablo2::structures::item_quality_t quality) {
	loot_filter_settings::get().quality_settings[static_cast<uint32_t>(quality)] = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::register_quality_checkbox(const std::string& name,
																					  diablo2::structures::item_quality_t quality) {
	auto control = get_control<ui::controls::checkbox>(name);

	if (!control)
		return;

	control->set_state(loot_filter_settings::get().quality_settings[static_cast<size_t>(quality)]);
	control->set_on_click(std::bind(&loot_filter_settings_menu::update_quality_allowance,
									this, std::placeholders::_1, quality));
}

void d2_tweaks::client::modules::loot_filter_settings_menu::setup_hooks() {
	m_handle_dropped_items_original =
		static_cast<decltype(m_handle_dropped_items_original)>(hooking::get_call(
			diablo2::d2_client::get_base() + 0x1641B));
	m_draw_dropped_items_names_original =
		static_cast<decltype(m_draw_dropped_items_names_original)>(hooking::get_call(
			diablo2::d2_client::get_base() + 0x81BF5));

	hooking::set_call(diablo2::d2_client::get_base() + 0x81BF5, draw_dropped_items_names);
	hooking::set_call(diablo2::d2_client::get_base() + 0x1641B, handle_dropped_items);

	setup_alt_hook();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::setup_alt_hook() const {
	asm_code asmCode;
	asmCode.add({ 0x83, 0x7D, 0x00, 0x04 }); //cmp dword ptr [ebp+0], 4
	asmCode.add({ 0x0F, 0x85 },
				new asm_address_relative(2, 6, diablo2::d2_client::get_base() + 0x6A399)); //jnz D2Client.dll+6A399

	asmCode.add({ 0x56 }); //push esi
	asmCode.add({ 0xB8 }, new asm_address_static(1, check_alt_item)); //mov eax, check_alt_item
	asmCode.add({ 0x8B, 0xCD }); //mov ecx, ebp
	asmCode.add({ 0xFF, 0xD0 }); //call eax
	asmCode.add({ 0x5E }); //pop esi

	asmCode.add({ 0x84, 0xC0 }); //test al, al
	asmCode.add({ 0x0F, 0x84 },
				new asm_address_relative(2, 6, diablo2::d2_client::get_base() + 0x6A399)); //je D2Client.dll+6A399
	asmCode.add({ 0xE9 },
				new asm_address_relative(1, 5, diablo2::d2_client::get_base() + 0x6A027)); //jmp D2Client.dll+6A027

	auto addr = diablo2::d2_client::get_base() + 0x6A022;

	DWORD old_protect;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &old_protect);

	memset(addr, 0x90, 10);
	*reinterpret_cast<unsigned char*>(addr) = 0xE9;
	*reinterpret_cast<int32_t*>(static_cast<char*>(addr) + 1) =
		reinterpret_cast<int32_t>(asmCode.get_code()) - reinterpret_cast<int32_t>(addr) - 5;

	VirtualProtect(addr, 5, old_protect, &old_protect);
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::is_gold(diablo2::structures::unit* item) {
	static auto goldRecordIndex = -1;

	if (item == nullptr)
		return false;

	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
		return false;

	if (item->item_data == nullptr)
		return false;

	if (goldRecordIndex == -1) {
		auto record = diablo2::d2_common::get_item_record(item->data_record_index);

		if (record->string_code[0] == 'g' &&
			record->string_code[1] == 'l' &&
			record->string_code[2] == 'd') { //Gold pile
			goldRecordIndex = item->data_record_index;
		}
	}

	return item->data_record_index == goldRecordIndex;
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::is_rune(diablo2::structures::unit* item) {
	static auto runeTypeId = -1;

	if (item == nullptr)
		return false;

	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
		return false;

	if (item->item_data == nullptr)
		return false;

	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	if (runeTypeId == -1) {
		const auto itemTypeRecord = diablo2::d2_common::get_item_type_record(record->type);

		const auto code = itemTypeRecord->code;

		if (code[0] == 'r' &&
			code[1] == 'u' &&
			code[2] == 'n' &&
			code[3] == 'e') {
			runeTypeId = record->type;
		}
	}

	return record->type == runeTypeId;
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::is_gem(diablo2::structures::unit* item) {
	static auto gemTypeId = -1;

	if (item == nullptr)
		return false;

	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
		return false;

	if (item->item_data == nullptr)
		return false;

	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	if (gemTypeId == -1) {
		const auto itemTypeRecord = diablo2::d2_common::get_item_type_record(record->type);

		const auto code = itemTypeRecord->code;

		if (code[0] == 'g' &&
			code[1] == 'e' &&
			code[2] == 'm') {
			gemTypeId = record->type;
		}
	}

	return record->type == gemTypeId;
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::check_alt_item(diablo2::structures::unit* unit) {
	static auto& instance = singleton<loot_filter_settings_menu>::instance();

	if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
		return true;
	}

	if (unit->item_data == nullptr) {
		return true;
	}

	if (is_gold(unit))
		return instance.m_show_gold && instance.m_show_gold->get_state();

	if (is_rune(unit))
		return instance.m_show_runes && instance.m_show_runes->get_state();

	if (is_gem(unit))
		return instance.m_show_gems && instance.m_show_gems->get_state();

	return loot_filter_settings::get().quality_settings[static_cast<int32_t>(unit->item_data->quality)];
}

void d2_tweaks::client::modules::loot_filter_settings_menu::draw_dropped_items_names(diablo2::structures::unit* unit,
																					 void* edx) {
	static auto& instance = singleton<loot_filter_settings_menu>::instance();

	if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
		instance.m_draw_dropped_items_names_original(unit, edx);
		return;
	}

	if (unit->item_data == nullptr) {
		instance.m_draw_dropped_items_names_original(unit, edx);
		return;
	}

	if (instance.m_altonly && instance.m_altonly->get_state())
		return;

	if (is_gold(unit) && instance.m_show_gold && !instance.m_show_gold->get_state())
		return;

	if (is_rune(unit) && instance.m_show_runes && !instance.m_show_runes->get_state())
		return;

	if (is_gem(unit) && instance.m_show_gems && !instance.m_show_gems->get_state())
		return;

	if (!loot_filter_settings::get().quality_settings[static_cast<int32_t>(unit->item_data->quality)])
		return;

	instance.m_draw_dropped_items_names_original(unit, edx);
}

void d2_tweaks::client::modules::loot_filter_settings_menu::handle_dropped_items(diablo2::structures::unit* unit,
																				 void* edx) {
	static auto& instance = singleton<loot_filter_settings_menu>::instance();

	if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
		instance.m_handle_dropped_items_original(unit, edx);
		return;
	}

	if (unit->item_data == nullptr) {
		instance.m_handle_dropped_items_original(unit, edx);
		return;
	}

	if (instance.m_altonly && instance.m_altonly->get_state())
		return;

	if (is_gold(unit) && instance.m_show_gold && !instance.m_show_gold->get_state())
		return;

	if (is_rune(unit) && instance.m_show_runes && !instance.m_show_runes->get_state())
		return;

	if (is_gem(unit) && instance.m_show_gems && !instance.m_show_gems->get_state())
		return;

	if (!loot_filter_settings::get().quality_settings[static_cast<int32_t>(unit->item_data->quality)])
		return;

	instance.m_handle_dropped_items_original(unit, edx);
}
