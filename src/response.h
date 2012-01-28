#pragma once
#include <string>
#include "lipido.h"

namespace lipido {

	struct WebResponse {
		//http code
		//body
		//etc
		int httpCode;
		std::string contentType;
		std::string body;

		bool deliverFile;
		std::string path;

		WebResponse() {
			httpCode = 200;
			deliverFile = false;
			contentType = "text/html";
		}
	};

}
