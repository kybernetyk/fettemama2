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
		typedef std::function<lipido::response_t (lipido::context_t &context)> handler_t;
		struct server_t {
			std::map<std::string, handler_t> get_handlers;
			std::map<std::string, handler_t> post_handlers;
		};

	namespace server {
		server_t make();		

		void add_post_handler(server_t &srv, std::string URI, handler_t handler);
		void add_get_handler(server_t &srv, std::string URI, handler_t handler);

		void run(server_t &srv, unsigned short port);
	}
}
