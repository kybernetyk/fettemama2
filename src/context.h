#pragma once
#include <memory>
#include "lipido.h"

namespace lipido {
		struct server_t;
		struct context_t {
			server_t &server;
			//std::shared_ptr<server_t> server;
			//server &
			//request &
			//session &

			context_t(server_t &srv) : server(srv) { }
		};
		
		namespace context {

	}
}
