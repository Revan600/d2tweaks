#pragma once

#include <d2tweaks/server/modules/server_module.h>

namespace d2_tweaks {
	namespace server {
		class server;

		namespace modules {
			class identify_on_pickup final : public server_module {
			public:
				void init() override;
			};
		}
	}
}
