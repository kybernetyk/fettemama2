#pragma once
#include <memory>
#include "lipido.h"

namespace lipido {
		struct server_t;
		struct context_t {
			std::shared_ptr<server_t> server;
			//request &
			//session &
		};

		namespace context {

	}
}
