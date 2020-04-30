#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct bodylocs_line {
			union {
				uint32_t code;
				char str[4];
			};
		};
	}
}
