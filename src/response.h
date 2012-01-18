#pragma once
#include <string>
#include "lipido.h"

namespace lipido {
		struct response_t {
			std::string body;
			// code
			// etc
			// text
		};	
		namespace response {
			response_t make();
			response_t make(std::string body);

		}
}
