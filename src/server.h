/*
 * fettemama 2.0 - c++ rocks
 *
 * (c) Leon Szpilewski, 2012, GPL3
 */
#pragma once
#include <map>
#include <string>
#include <functional>
#include "lipido.h"

namespace lipido {
	namespace server {

		typedef std::function<lipido::response::response (lipido::context::context &context)> handler_t;
		struct server {
			std::map<std::string, handler_t> get_handlers;
			std::map<std::string, handler_t> post_handlers;
		};

		server make_server();

		void add_post_handler(server &srv, std::string URI, handler_t handler);
		void add_get_handler(server &srv, std::string URI, handler_t handler);

		void run(server &srv, unsigned short port);
	}
}
