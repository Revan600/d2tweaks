#pragma once

#include <fw/singleton.h>
#include <nlohmann/json.hpp>

class config : public singleton<config> {
	nlohmann::json* m_json;

	bool m_unlock_fps;
	bool m_prevent_minimize;
	uint32_t m_gold_pickup_range;
	
public:
	explicit config(token);
};