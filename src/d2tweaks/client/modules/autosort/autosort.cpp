#include <d2tweaks/client/modules/autosort/autosort.h>

#include <d2tweaks/client/client.h>

#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>

#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>

MODULE_INIT(autosort)

class inventory_sort_menu : public d2_tweaks::ui::menu {
	d2_tweaks::common::asset* m_buttons_img;

	d2_tweaks::ui::controls::button* m_sort_inventory_btn;
	d2_tweaks::ui::controls::button* m_sort_stash_btn;
	d2_tweaks::ui::controls::button* m_sort_cube_btn;
public:
	inventory_sort_menu() {
		menu::set_enabled(true);
		menu::set_visible(true);

		load_xml("d2tweaks\\interfaces\\autosort.xml");

		m_buttons_img = singleton<d2_tweaks::common::asset_manager>::instance().get_mpq_file(
			"d2tweaks\\assets\\buttons", d2_tweaks::common::MPQ_FILE_TYPE_DC6);

		m_sort_inventory_btn = get_button("m_sort_inventory_btn", std::bind(&inventory_sort_menu::sort_inventory_click, this));
		m_sort_stash_btn = get_button("m_sort_stash_btn", std::bind(&inventory_sort_menu::sort_stash_click, this));
		m_sort_cube_btn = get_button("m_sort_cube_btn", std::bind(&inventory_sort_menu::sort_cube_click, this));
	}

	void draw() override {
		if (!should_draw()) {
			m_sort_inventory_btn->set_enabled(false);
			m_sort_inventory_btn->set_visible(false);

			m_sort_stash_btn->set_enabled(false);
			m_sort_stash_btn->set_visible(false);

			m_sort_cube_btn->set_enabled(false);
			m_sort_cube_btn->set_visible(false);

			return;
		}

		m_sort_inventory_btn->set_enabled(true);
		m_sort_inventory_btn->set_visible(true);

		m_sort_stash_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH));
		m_sort_stash_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH));

		m_sort_cube_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE));
		m_sort_cube_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE));

		menu::draw();
	}
private:
	static bool should_draw() {
		return diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_TRADE) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCSHOP);
	}

	d2_tweaks::ui::controls::button* get_button(const std::string& name, const std::function<void()>& onClick) {
		auto result = static_cast<d2_tweaks::ui::controls::button*>(get_control(name));

		result->set_on_click(onClick);

		result->set_enabled(false);
		result->set_visible(false);

		return result;
	}

	void sort_inventory_click() {
		request_sort(0x00);
	}

	void sort_stash_click() {
		request_sort(0x04);
	}

	void sort_cube_click() {
		request_sort(0x03);
	}

	void request_sort(const uint8_t page) {
		static d2_tweaks::common::inventory_sort_cs packet;

		const auto player = diablo2::d2_client::get_local_player();

		std::vector<diablo2::structures::unit*> items;

		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			if (item->item_data->page == page)
				items.push_back(item);
		}

		for (auto item : items)
			diablo2::d2_common::inv_remove_item(player->inventory, item);

		packet.page = page;
		diablo2::d2_client::send_to_server(&packet, sizeof packet);
	}
};

void d2_tweaks::client::modules::autosort::init() {
	singleton<ui::ui_manager>::instance().add_menu(new inventory_sort_menu());

	singleton<client>::instance().register_packet_handler(common::MESSAGE_TYPE_INVENTORY_SORT, this);
}

void d2_tweaks::client::modules::autosort::handle_packet(common::packet_header* packet) {
	static auto& instance = singleton<client>::instance();
	const auto inventorySort = static_cast<common::inventory_sort_sc*>(packet);

	const auto item = instance.get_client_unit(0x04, inventorySort->guid); //0x03 -> 0x04 - item

	if (item == nullptr)
		return;;

	const auto player = diablo2::d2_client::get_local_player();

	//Last parameter is some boolean
	const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, inventorySort->page, diablo2::d2_client::is_lod());

	item->item_data->page = inventorySort->page;

	diablo2::d2_common::inv_add_item(player->inventory, item, inventorySort->tx, inventorySort->ty, inventoryIndex, true, item->item_data->page);
	diablo2::d2_common::inv_update_item(player->inventory, item, true);
}

