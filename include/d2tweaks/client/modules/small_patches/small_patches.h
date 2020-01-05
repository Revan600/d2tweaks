#pragma once

#include <d2tweaks/client/modules/client_module.h>

//Client side patches that are too small to implement as separate modules

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class small_patches final : public client_module {
			public:
				void init() override;
			};
		}
	}
}