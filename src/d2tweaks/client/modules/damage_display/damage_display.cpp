#include <d2tweaks/client/modules/damage_display/damage_display.h>

#include <d2tweaks/common/protocol.h>
#include <d2tweaks/client/client.h>
#include <spdlog/spdlog.h>
#include <diablo2/utils/screen.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>
#include <diablo2/d2client.h>
#include <d2tweaks/ui/ui_manager.h>
#include <diablo2/d2win.h>

#include <fw/pool.h>

#include <diablo2/structures/gfxdata.h>
#include <diablo2/structures/gfxcell.h>
#include <diablo2/d2cmp.h>
#include <common/hooking.h>

MODULE_INIT(damage_display)

struct damage_label {
	bool screen_space;
	diablo2::ui_color_t color;
	uint32_t x;
	uint32_t y;
	uint32_t unit_width;
	uint32_t unit_height;
	uint64_t start;
	int32_t damage;
	int32_t text_width;
	wchar_t text[64];

	damage_label(uint32_t x, uint32_t y, uint32_t uw, uint32_t uh, int32_t damage)
		: screen_space(false), color(diablo2::ui_color_t::UI_COLOR_WHITE), x(x), y(y),
		unit_width(uw), unit_height(uh), damage(damage), text_width(0) {
		start = GetTickCount64();
		memset(text, 0x00, sizeof text);
	}

	damage_label()
		: screen_space(false), color(diablo2::UI_COLOR_WHITE), x(0), y(0),
		unit_width(0), unit_height(0), damage(0), text_width(0) {
		start = GetTickCount64();
		memset(text, 0x00, sizeof text);
	}
};

static growing_object_pool<damage_label> g_label_pool([]() {
	return new damage_label();
});

static const size_t DAMAGE_LABELS_SIZE = 1024;
static damage_label* g_labels[DAMAGE_LABELS_SIZE]{ nullptr };
static size_t g_labels_count = 0;

static const uint32_t DISPLAY_TIME = 2500;

static float lerp(const float v0, const float v1, const float t) {
	return v0 + t * (v1 - v0);
}

static bool add_label(damage_label* label) {
	for (size_t i = 0; i < DAMAGE_LABELS_SIZE; i++) {
		if (g_labels[i] != nullptr)
			continue;

		g_labels[i] = label;
		g_labels_count++;
		return true;
	}

	return false;
}

static void remove_label(damage_label* label) {
	for (size_t i = 0; i < DAMAGE_LABELS_SIZE; i++) {
		if (g_labels[i] != label)
			continue;

		g_labels[i] = nullptr;
		g_labels_count--;
		return;
	}
}

static void remove_label(size_t index) {
	if (index < 0 || index >= DAMAGE_LABELS_SIZE)
		return;

	g_labels[index] = nullptr;
	g_labels_count--;
}

static void draw_damage_labels() {
	const auto player = diablo2::d2_client::get_local_player();

	if (!player)
		return;

	if (g_labels_count == 0)
		return;

	const auto currentTime = GetTickCount64();

	size_t updatedLabels = 0;
	auto mx = 0, my = 0;

	const auto font = diablo2::d2_win::get_current_font();

	for (size_t i = 0; i < DAMAGE_LABELS_SIZE; i++) {
		const auto label = g_labels[i];

		if (label == nullptr) {
			auto next = g_labels[i + 1];

			if (updatedLabels == g_labels_count)
				break;

			continue;
		}

		const auto delta = currentTime - label->start;

		if (delta >= DISPLAY_TIME) {
			remove_label(i);
			g_label_pool.put(label);
			continue;
		}

		updatedLabels++;

		if (label->screen_space) {
			mx = label->x;
			my = label->y;
			diablo2::d2_win::set_current_font(diablo2::UI_FONT_16);
		} else {
			diablo2::utils::screen::world_to_screen(label->x, label->y, mx, my);
			diablo2::d2_win::set_current_font(diablo2::UI_FONT_8);
		}

		const auto offset = static_cast<int32_t>(lerp(static_cast<float>(label->unit_height) + 5.f,
													  static_cast<float>(label->unit_height) + 30.f,
													  static_cast<float>(delta) / static_cast<float>(DISPLAY_TIME)));

		mx -= label->text_width / 2;
		mx -= label->unit_width / 2;
		my -= offset;

		diablo2::d2_win::draw_text(label->text,
								   mx,
								   my,
								   label->color, 0);
	}

	diablo2::d2_win::set_current_font(font);
}

static void(__fastcall* g_draw_game_ui)(int32_t a1);
static void __fastcall draw_game_ui(int32_t a1) {
	draw_damage_labels();

	g_draw_game_ui(a1);
}

static diablo2::ui_color_t damage_type_to_color(d2_tweaks::common::damage_type_t type) {
	switch (type) {
		case d2_tweaks::common::DAMAGE_TYPE_PHYSICAL: return diablo2::UI_COLOR_GREY;
		case d2_tweaks::common::DAMAGE_TYPE_COLD: return diablo2::UI_COLOR_BLUE;
		case d2_tweaks::common::DAMAGE_TYPE_FIRE: return diablo2::UI_COLOR_ORANGE;
		case d2_tweaks::common::DAMAGE_TYPE_LIGHTNING: return diablo2::UI_COLOR_YELLOW;
		case d2_tweaks::common::DAMAGE_TYPE_POISON: return diablo2::UI_COLOR_GREEN;
		case d2_tweaks::common::DAMAGE_TYPE_MAGIC: return diablo2::UI_COLOR_PURPLE;
		default: return diablo2::UI_COLOR_BLACK;
	}
}

void d2_tweaks::client::modules::damage_display::init() {
	singleton<client>::instance().register_packet_handler(common::MESSAGE_TYPE_DAMAGE_INFO, this);

	hooking::hook(diablo2::d2_client::get_base() + 0x80A30, draw_game_ui, reinterpret_cast<void**>(&g_draw_game_ui));
}

void d2_tweaks::client::modules::damage_display::handle_packet(common::packet_header* packet) {
	static auto& instance = singleton<client>::instance();
	static diablo2::structures::gfxdata gfxdata;
	const auto player = diablo2::d2_client::get_local_player();
	const auto info = static_cast<common::damage_info_sc*>(packet);

	if (player == nullptr)
		return;

	if (info->unit_type == 0x00 && info->guid == player->guid) {
		const auto label = g_label_pool.get();
		label->screen_space = true;
		label->color = damage_type_to_color(info->damage_type);
		label->unit_width = 0;
		label->unit_height = 0;
		label->x = 70;
		label->y = 500;
		label->damage = info->damage;
		label->start = GetTickCount64();
		swprintf_s(label->text, L"%u", label->damage);
		label->text_width = diablo2::d2_win::get_text_pixel_width(label->text);

		if (add_label(label))
			return;

		g_label_pool.put(label); //prevent memory leak if there's no room for another label
		return;
	}

	if (info->unit_type != 0x01)
		return;

	const auto monsterUnit = instance.get_client_unit(0x01, info->guid);

	if (monsterUnit == nullptr)
		return;

	const auto mx = diablo2::d2_common::get_unit_x(monsterUnit->path);
	const auto my = diablo2::d2_common::get_unit_y(monsterUnit->path);

	const auto label = g_label_pool.get();

	label->color = damage_type_to_color(info->damage_type);

	memset(&gfxdata, 0x00, sizeof gfxdata);

	int32_t index;
	if (diablo2::d2_client::cache_gfx_data(&gfxdata, monsterUnit, nullptr, 0, 0, &index, 1, -1) &&
		diablo2::structures::d2_cmp::init_gfx_data(&gfxdata) &&
		gfxdata.cell_init) {
		label->unit_width = gfxdata.cell_init->width + gfxdata.cell_init->offset_x;
		label->unit_height = gfxdata.cell_init->height - gfxdata.cell_init->offset_y;
	} else {
		label->unit_width = 0;
		label->unit_height = 0;
	}

	label->screen_space = false;
	label->x = mx;
	label->y = my;
	label->damage = info->damage;
	label->start = GetTickCount64();
	swprintf_s(label->text, L"%i", label->damage);
	label->text_width = diablo2::d2_win::get_text_pixel_width(label->text);

	if (add_label(label))
		return;

	g_label_pool.put(label); //prevent memory leak if there's no room for another label
}

void d2_tweaks::client::modules::damage_display::tick() {

}
