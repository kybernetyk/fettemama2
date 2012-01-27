#include <memory>
#include "server.h"


namespace lipido {

	void WebServer::addGetHandler(std::string URI, WebServerHandler handler) {
		m_getHandlers[URI] = handler;
	}

	void WebServer::addPostHandler(std::string URI, WebServerHandler handler) {
		m_postHandlers[URI] = handler;
	}

	void WebServer::run(unsigned short port) {
		printf("server %p listening on port %i ...\n",this, port);

		printf("calling handler for / ...\n\t");
		auto h = m_getHandlers["/"];
		if (h) {
			WebRequest req; //= the request we got from the client
			WebSession sess; // = retrieve session from cookies()
			WebContext ctx(*this, req, sess);
			auto response = h(ctx);

			//send(response)

		} else {
			throw "no handler for / found!";
		}
		printf("ok. self test succeeded. let's serve!\n");

		printf("server %p died\n", this);

	}
}
