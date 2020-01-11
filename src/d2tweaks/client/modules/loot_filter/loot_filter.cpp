#include <d2tweaks/client/modules/loot_filter/loot_filter.h>
#include <d2tweaks/ui/menu.h>

#include <diablo2/d2client.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/ui/controls/image.h>
#include <d2tweaks/ui/controls/checkbox.h>
#include <spdlog/spdlog.h>
#include <common/hooking.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/item_data.h>

MODULE_INIT(loot_filter)

class loot_filter_settings_toggle;
class loot_filter_settings;

loot_filter_settings_toggle* g_loot_filter_settings_toggle = nullptr;
loot_filter_settings* g_loot_filter_settings = nullptr;

class loot_filter_settings final : public d2_tweaks::ui::menu {
	bool m_allowed_qualities[15]{ false };
	d2_tweaks::ui::controls::checkbox* m_nopickup;
	void(__fastcall* m_draw_dropped_items_names_original)(void*, void*);
	void(__fastcall* m_handle_dropped_items_original)(void*, void*);
public:
	loot_filter_settings() {
		using namespace diablo2::structures;
		g_loot_filter_settings = this;

		menu::set_enabled(false);
		menu::set_visible(false);

		load_xml("d2tweaks\\interfaces\\loot_filter_settings.xml");

		m_nopickup = get_control<d2_tweaks::ui::controls::checkbox>("m_nopickup");

		register_quality_checkbox("m_normal", item_quality_t::ITEM_QUALITY_NORMAL);
		register_quality_checkbox("m_magic", item_quality_t::ITEM_QUALITY_MAGIC);
		register_quality_checkbox("m_rare", item_quality_t::ITEM_QUALITY_RARE);
		register_quality_checkbox("m_crafted", item_quality_t::ITEM_QUALITY_CRAFTED);
		register_quality_checkbox("m_set", item_quality_t::ITEM_QUALITY_SET);
		register_quality_checkbox("m_unique", item_quality_t::ITEM_QUALITY_UNIQUE);

		setup_hooks();
	}

	void draw() override {
		if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MAINMENU))
			return;

		menu::draw();
	}
private:
	void update_quality_allowance(bool value, diablo2::structures::item_quality_t quality) {
		m_allowed_qualities[static_cast<uint32_t>(quality)] = value;
	}

	void register_quality_checkbox(const std::string& name, diablo2::structures::item_quality_t quality) {
		auto control = get_control<d2_tweaks::ui::controls::checkbox>(name);

		if (!control)
			return;

		control->set_on_click(std::bind(&loot_filter_settings::update_quality_allowance,
										this, std::placeholders::_1, quality));
	}

	void setup_hooks() {
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

	void setup_alt_hook() {
		//Yes, I know that I should use constexpr here, but I'm writing this late night so yeah...
#define SET_RELATIVE_ADDRESS(offset, fullOffset, instructionSize, address) *reinterpret_cast<int32_t*>(stub + fullOffset) = reinterpret_cast<int32_t>(address) - (stub + offset) - instructionSize
#define SET_ADDRESS(fullOffset, address) *reinterpret_cast<int32_t*>(stub + fullOffset) = reinterpret_cast<int32_t>(address)

		unsigned char code[]
		{
			0x83, 0x7D, 0x00, 0x04, //cmp dword ptr [ebp+0], 4
			0x0F, 0x85, 0x00, 0x00, 0x00, 0x00, //jnz D2Client.dll+6A399

			0x56, //push esi
			0xB8, 0x00, 0x00, 0x00, 0x00, //mov eax, check_alt_item
			0x8B, 0xCD, //mov ecx, ebp
			0xFF, 0xD0, //call eax
			0x5E, //pop esi
			0x84, 0xC0, //test al, al
			0x0F, 0x84, 0x00, 0x00, 0x00, 0x00, //je D2Client.dll+6A399
			0xE9, 0x00, 0x00, 0x00, 0x00 //jmp D2Client.dll+6A027
		};

		const auto stub = hooking::get_executable_memory(diablo2::d2_client::get_base() + 0x6A022, sizeof code);
		memcpy(reinterpret_cast<void*>(stub), code, sizeof code);

		SET_RELATIVE_ADDRESS(4, 6, 6, (diablo2::d2_client::get_base() + 0x6A399));
		SET_ADDRESS(12, check_alt_item);
		SET_RELATIVE_ADDRESS(23, 25, 6, (diablo2::d2_client::get_base() + 0x6A399));
		SET_RELATIVE_ADDRESS(29, 30, 5, (diablo2::d2_client::get_base() + 0x6A027));

		auto addr = diablo2::d2_client::get_base() + 0x6A022;

		DWORD old_protect;
		VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &old_protect);

		memset(addr, 0x90, 10);
		*reinterpret_cast<unsigned char*>(addr) = 0xE9;
		*reinterpret_cast<int32_t*>(static_cast<char*>(addr) + 1) = stub - reinterpret_cast<int32_t>(addr) - 5;

		VirtualProtect(addr, 5, old_protect, &old_protect);

#undef SET_ADDRESS
#undef SET_RELATIVE_ADDRESS
	}

	static bool __fastcall check_alt_item(diablo2::structures::unit* unit) {
		if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
			return true;
		}

		if (unit->item_data == nullptr) {
			return true;
		}

		return g_loot_filter_settings->m_allowed_qualities[static_cast<int32_t>(unit->item_data->quality)];
	}

	//draw labels over dropped items
	static void __fastcall draw_dropped_items_names(diablo2::structures::unit* unit, void* edx) {
		if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
			g_loot_filter_settings->m_draw_dropped_items_names_original(unit, edx);
			return;
		}

		if (unit->item_data == nullptr) {
			g_loot_filter_settings->m_draw_dropped_items_names_original(unit, edx);
			return;
		}

		if (!g_loot_filter_settings->m_allowed_qualities[static_cast<int32_t>(unit->item_data->quality)])
			return;

		if (g_loot_filter_settings->m_nopickup->get_state())
			return;

		g_loot_filter_settings->m_draw_dropped_items_names_original(unit, edx);
	}

	//handle hovering over item and actual click
	static void __fastcall handle_dropped_items(diablo2::structures::unit* unit, void* edx) {
		if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
			g_loot_filter_settings->m_handle_dropped_items_original(unit, edx);
			return;
		}

		if (unit->item_data == nullptr) {
			g_loot_filter_settings->m_handle_dropped_items_original(unit, edx);
			return;
		}

		if (!g_loot_filter_settings->m_allowed_qualities[static_cast<int32_t>(unit->item_data->quality)])
			return;

		if (g_loot_filter_settings->m_nopickup->get_state())
			return;

		g_loot_filter_settings->m_handle_dropped_items_original(unit, edx);
	}
};

class loot_filter_settings_toggle final : public d2_tweaks::ui::menu {
	d2_tweaks::ui::controls::button* m_toggle_filter_settings_btn;
	bool m_show;
public:
	loot_filter_settings_toggle() {
		g_loot_filter_settings_toggle = this;

		m_show = false;

		menu::set_enabled(true);
		menu::set_visible(true);

		load_xml("d2tweaks\\interfaces\\loot_filter_settings_toggle.xml");

		m_toggle_filter_settings_btn = static_cast<d2_tweaks::ui::controls::button*>(
			menu::get_control("m_toggle_filter_settings_btn"));
		m_toggle_filter_settings_btn->set_enabled(false);
		m_toggle_filter_settings_btn->set_visible(false);
		m_toggle_filter_settings_btn->set_on_click(std::bind(&loot_filter_settings_toggle::toggle_filter_settings_click, this));
	}

	void toggle_filter_settings_click() {
		m_show = !m_show;

		g_loot_filter_settings->set_enabled(m_show);
		g_loot_filter_settings->set_visible(m_show);
	}

	void draw() override {
		m_toggle_filter_settings_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MINIPANEL));
		m_toggle_filter_settings_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MINIPANEL));

		menu::draw();
	}

	bool key_event(uint32_t key, bool up) override {
		if (key == VK_ESCAPE && m_show) {
			m_show = false;

			g_loot_filter_settings->set_enabled(m_show);
			g_loot_filter_settings->set_visible(m_show);

			return true; //block escape key stroke
		}

		return menu::key_event(key, up);
	}
};

void d2_tweaks::client::modules::loot_filter::init() {
	singleton<ui::ui_manager>::instance().add_menu(new loot_filter_settings());
	singleton<ui::ui_manager>::instance().add_menu(new loot_filter_settings_toggle());
}
