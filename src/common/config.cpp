#include <common/config.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

config::config(token) {
	m_json = new nlohmann::json();

	if (std::filesystem::exists("d2tweaks.json")) {
		const std::ifstream cfgFile("d2tweaks.json");
		std::stringstream ss;
		ss << cfgFile.rdbuf();

		m_json->parse(ss.str());
	}
}
