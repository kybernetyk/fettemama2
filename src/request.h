#include <string>
#include <map>

namespace lipido {

	struct WebRequest {
		std::string URI;
		std::map<std::string, std::string> params;
	};

}
