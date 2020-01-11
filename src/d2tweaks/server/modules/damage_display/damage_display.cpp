#include <d2tweaks/server/modules/damage_display/damage_display.h>

#include <d2tweaks/server/server.h>

#include <diablo2/d2game.h>
#include <diablo2/structures/damage.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/net_client.h>
#include <common/hooking.h>
#include <diablo2/d2common.h>
#include <spdlog/spdlog.h>

MODULE_INIT(damage_display)

static char(__fastcall* g_apply_attack_results_origin)(diablo2::structures::game* game,
													   diablo2::structures::unit* attacker,
													   diablo2::structures::unit* defender,
													   BOOL recalculateDamage,
													   diablo2::structures::damage* dmg);

#define PRINT_DMG_DELIMITER(name, delimiter) if(dmg->name > 0) spdlog::debug(#name ": {0}", dmg->name / delimiter)
#define PRINT_DMG(name) PRINT_DMG_DELIMITER(name, 256)

d2_tweaks::common::damage_type_t get_damage_type(diablo2::structures::damage* dmg) {
	uint32_t damage[d2_tweaks::common::DAMAGE_TYPE_COUNT]{ 0 };

	if (dmg == nullptr || dmg->dmg_total == 0)
		return d2_tweaks::common::DAMAGE_TYPE_UNKNOWN;

	damage[d2_tweaks::common::DAMAGE_TYPE_PHYSICAL] = dmg->phys_damage;
	damage[d2_tweaks::common::DAMAGE_TYPE_COLD] = dmg->cold_damage;
	damage[d2_tweaks::common::DAMAGE_TYPE_FIRE] = dmg->fire_damage;
	damage[d2_tweaks::common::DAMAGE_TYPE_LIGHTNING] = dmg->ltng_damage;
	damage[d2_tweaks::common::DAMAGE_TYPE_POISON] = dmg->pois_damage;
	damage[d2_tweaks::common::DAMAGE_TYPE_MAGIC] = dmg->mag_damage;

	auto result = d2_tweaks::common::DAMAGE_TYPE_UNKNOWN;
	uint32_t damage_temp = 0;

	for (size_t i = 0; i < d2_tweaks::common::DAMAGE_TYPE_COUNT; i++) {
		if (damage[i] <= damage_temp)
			continue;

		result = static_cast<d2_tweaks::common::damage_type_t>(i);
		damage_temp = damage[i];
	}

	return result;
}

static void send_damage_data(diablo2::structures::unit* defender,
							 diablo2::structures::net_client* client,
							 diablo2::structures::damage* dmg) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();
	static d2_tweaks::common::damage_info_sc packet;

	if (dmg->dmg_total <= 0)
		return;

	packet.unit_type = static_cast<uint8_t>(defender->type);
	packet.guid = defender->guid;
	packet.damage_type = get_damage_type(dmg);
	packet.damage = dmg->dmg_total / 256;

	if (packet.damage_type == d2_tweaks::common::DAMAGE_TYPE_UNKNOWN)
		return;

	if (packet.damage <= 0)
		return;

	instance.send_packet(client, &packet, sizeof packet);
}

static bool has_players(diablo2::structures::unit* attacker,
						diablo2::structures::unit* defender) {
	return attacker->type == diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER ||
		defender->type == diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER;
}

static bool has_hirelings(diablo2::structures::unit* attacker,
						  diablo2::structures::unit* defender) {
	return attacker && attacker->is_hireling() || defender && defender->is_hireling();
}

static bool has_pets(diablo2::structures::unit* attacker,
					 diablo2::structures::unit* defender) {
	return attacker && attacker->is_pet() || defender && defender->is_pet();
}

static diablo2::structures::unit* get_hireling_owner(diablo2::structures::game* game,
													 diablo2::structures::unit* unit) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	if (!unit)
		return nullptr;

	auto guid = diablo2::d2_common::get_stat(unit, diablo2::UNIT_STAT_UNUSED212, 0);

	if (guid != 0)
		return instance.get_server_unit(game, guid, 0x00);

	instance.iterate_server_units(game, 0x00, [&](diablo2::structures::unit* player) {
		const auto pet = diablo2::d2_game::get_player_pet(game, player, 7, 0);

		if (pet == unit) {
			guid = player->guid;
			return false; //stop iterator
		}

		return true;
	});

	if (guid == 0)
		return nullptr;

	diablo2::d2_common::set_stat(unit, diablo2::UNIT_STAT_UNUSED212, guid, 0);

	return instance.get_server_unit(game, guid, 0x00);
}

static diablo2::structures::unit* get_pet_owner(diablo2::structures::game* game,
												diablo2::structures::unit* unit) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	if (!unit)
		return nullptr;

	auto guid = diablo2::d2_common::get_stat(unit, diablo2::UNIT_STAT_UNUSED212, 0);

	if (guid != 0)
		return instance.get_server_unit(game, guid, 0x00);

	instance.iterate_server_units(game, 0x00, [&](diablo2::structures::unit* player) {
		diablo2::d2_game::iterate_unit_pets(
			game, player, [&](diablo2::structures::game*, diablo2::structures::unit*, diablo2::structures::unit* u) {
			if (u != unit)
				return;

			guid = player->guid;
		});

		return guid == 0;
	});

	if (guid == 0)
		return nullptr;

	diablo2::d2_common::set_stat(unit, diablo2::UNIT_STAT_UNUSED212, guid, 0);

	return instance.get_server_unit(game, guid, 0x00);
}

static void process_players_damage(diablo2::structures::unit* attacker,
								   diablo2::structures::unit* defender,
								   diablo2::structures::damage* dmg) {
	diablo2::structures::net_client* client = nullptr;

	if (attacker->type == diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		client = attacker->player_data->net_client;
	else if (defender->type == diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		client = defender->player_data->net_client;

	send_damage_data(defender, client, dmg);
}

static void process_hireling_damage(diablo2::structures::game* game,
									diablo2::structures::unit* attacker,
									diablo2::structures::unit* defender,
									diablo2::structures::damage* dmg) {
	diablo2::structures::unit* owner = nullptr;

	if (attacker->is_hireling()) {
		owner = get_hireling_owner(game, attacker);
	} else if (defender->is_hireling()) {
		owner = get_hireling_owner(game, defender);
	}

	if (!owner)
		return;

	send_damage_data(defender, owner->player_data->net_client, dmg);
}

static void process_pet_damage(diablo2::structures::game* game,
							   diablo2::structures::unit* attacker,
							   diablo2::structures::unit* defender,
							   diablo2::structures::damage* dmg) {
	diablo2::structures::unit* owner = nullptr;

	if (attacker->is_pet()) {
		owner = get_pet_owner(game, attacker);
	} else if (defender->is_pet()) {
		owner = get_pet_owner(game, defender);
	}

	if (!owner)
		return;

	send_damage_data(defender, owner->player_data->net_client, dmg);
}

static char __fastcall apply_attack_results(diablo2::structures::game* game,
											diablo2::structures::unit* attacker,
											diablo2::structures::unit* defender,
											BOOL recalculateDamage,
											diablo2::structures::damage* dmg) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	const auto result = g_apply_attack_results_origin(game, attacker, defender, recalculateDamage, dmg);

	if (has_players(attacker, defender)) {
		process_players_damage(attacker, defender, dmg);
		return result;
	}

	if (has_hirelings(attacker, defender)) {
		process_hireling_damage(game, attacker, defender, dmg);
		return result;
	}

	if (has_pets(attacker, defender)) {
		process_pet_damage(game, attacker, defender, dmg);
		return result;
	}

	return result;
}

void d2_tweaks::server::modules::damage_display::init() {
	hooking::hook(diablo2::d2_game::get_base() + 0x8FE90, apply_attack_results, reinterpret_cast<void**>(&g_apply_attack_results_origin));

	singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_DAMAGE_INFO, this);
}

bool d2_tweaks::server::modules::damage_display::handle_packet(diablo2::structures::game* game,
															   diablo2::structures::unit* player, common::packet_header* packet) {
	return true;
}

void d2_tweaks::server::modules::damage_display::tick() {}
