#ifndef NDEBUG
#pragma once

#include <d2tweaks/client/modules/client_module.h>

//Test client side module

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class test final : public client_module {
			public:
				void init() override;

				void handle_packet(common::packet_header* packet) override;
			};
		}
	}
}
#endif