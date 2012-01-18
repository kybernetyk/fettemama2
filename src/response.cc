#include "lipido.h"

namespace lipido {
	namespace response {
		response_t make() {
			response_t resp;
			return resp;
		}

		response_t make(std::string body) {
			response_t resp;
			resp.body = body;
			return resp;
		}
	}
}
