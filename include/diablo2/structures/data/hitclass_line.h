#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct hitclass_line {
			union {
				uint32_t code;
				char str[4];
			};
		};
	}
}
