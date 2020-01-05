#pragma once

#include <cstdint>

namespace diablo2 {
	namespace utils {
		class screen {
		public:
			static void screen_to_world(int32_t sx, int32_t sy, int32_t& wx, int32_t& wy);
			static void world_to_screen(int32_t wx, int32_t wy, int32_t& sx, int32_t& sy);
		};
	}
}