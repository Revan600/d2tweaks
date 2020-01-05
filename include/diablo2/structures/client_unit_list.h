#pragma once

namespace diablo2 {
	namespace structures {
		struct unit;

		struct client_unit_list {
			unit* unit_list[5][128]; //0x1120
		};
	}
}
