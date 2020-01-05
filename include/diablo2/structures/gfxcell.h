#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct gfxcell {
			int32_t version;
			int32_t width;
			int32_t height;
			int32_t offset_x;
			int32_t offset_y;
			void* fram_pixel_data;
		};
	}
}
