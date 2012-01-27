#pragma once
#include <memory>
#include "lipido.h"

namespace lipido {
		class WebServer;
		struct WebRequest;
		struct WebSession;
		struct WebContext {
			const WebServer &server;
			const WebRequest &request;
			const WebSession &session;

			WebContext (WebServer &srv, WebRequest &req, WebSession &sess) : server(srv), request(req), session(sess) {}
		};

}
