#pragma once
#include <string>
#include "lipido.h"

namespace lipido {

	struct WebResponse {
		int httpCode;
		std::string httpReason;

		std::string contentType;
		std::string body;

		bool deliverFile;
		std::string path;

		WebResponse() {
			httpCode = 200;
			httpReason = "OK";
			deliverFile = false;
			contentType = "text/html";
		}
	};

}
