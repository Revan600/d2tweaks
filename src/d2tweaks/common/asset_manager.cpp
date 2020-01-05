#include <d2tweaks/common/asset_manager.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/asset.h>

#include <diablo2/d2client.h>
#include <diablo2/d2win.h>

#include <common/hooking.h>

static void* g_d2_tweaks_mpq = nullptr;

static int32_t(__stdcall* g_reload_original)();

d2_tweaks::common::asset_manager::asset_manager(token) {}

void d2_tweaks::common::asset_manager::init() {
	hooking::hook(diablo2::d2_client::get_base() + 0x5E370, reload, reinterpret_cast<void**>(&g_reload_original));

	g_d2_tweaks_mpq = diablo2::d2_win::load_mpq("d2tweaks.dll", "d2tweaks.mpq", "D2TWEAKS", 6000);

	if (g_d2_tweaks_mpq == nullptr) {
		spdlog::critical("Cannot find d2tweaks.mpq!");
		exit(-1);
	}

	spdlog::debug("d2tweaks.mpq: {0}", g_d2_tweaks_mpq);
}

int32_t d2_tweaks::common::asset_manager::reload() {
	static auto& instance = singleton<asset_manager>::instance();
	const auto result = g_reload_original();

	spdlog::info("Reloading image assets!");

	for (const auto& it : instance.m_assets) {
		if (it.second->get_type() != MPQ_FILE_TYPE_DC6 &&
			it.second->get_type() != MPQ_FILE_TYPE_DCC)
			continue;

		spdlog::info("Reloading {0}...", it.second->get_path());

		if (it.second->get()) {
			const auto ures = diablo2::d2_client::unload_gfx_resource(static_cast<diablo2::structures::cellfile*>(it.second->get()));
			spdlog::info("Unloaded: {0}", ures);
		}

		const auto newData = instance.load_asset_data(it.second->get_path(), it.second->get_type());
		it.second->update(newData);
		spdlog::info("Done! {0}", newData);
	}

	return result;
}

d2_tweaks::common::asset* d2_tweaks::common::asset_manager::get_mpq_file(const std::string& path, mpq_file_type_t type) {
	const auto it = m_assets.find(path);

	if (it != m_assets.end())
		return it->second;

	const auto asset = new common::asset(path, load_asset_data(path, type), type);
	m_assets[path] = asset;

	return asset;
}

void* d2_tweaks::common::asset_manager::load_asset_data(const std::string& path, mpq_file_type_t type) {
	switch (type) {
		case MPQ_FILE_TYPE_DC6:
			return diablo2::d2_client::load_gfx_resource(const_cast<char*>(path.c_str()));
		default:
			return nullptr;
	}
}
