#include <memory>
#include "server.h"


namespace lipido {
	namespace server {

		server_t make() {
			server_t srv;
			return srv;
		}

		void add_post_handler(server_t &srv, std::string URI, handler_t handler) {
			srv.post_handlers[URI] = handler;
		}

		void add_get_handler(server_t &srv, std::string URI, handler_t handler) {
			srv.get_handlers[URI] = handler;
		}

		void run(server_t &srv, unsigned short port) {
			printf("server %p listening on port %i ...\n",&srv, port);

			printf("calling handler for / ...\n\t");
			auto h = srv.get_handlers["/"];
			if (h) {
				lipido::context_t ctx(srv);

				auto r = h(ctx);
			} else {
				throw "no handler for / found!";
			}
			printf("ok. self test succeeded. let's serve!\n");

			printf("server %p died\n", &srv);
		}
	}
}
