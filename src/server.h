/*
 * fettemama 2.0 - c++ rocks
 *
 * (c) Leon Szpilewski, 2012, GPL3
 */
#pragma once
#include <memory>
#include <map>
#include <string>
#include <functional>
#include "lipido.h"

		struct evhttp_request;

namespace lipido {

	class WebServer {
		typedef std::function<WebResponse (lipido::WebContext &context)> WebServerHandler;

	public:
		WebServer();
		~WebServer() {}
		void addPostHandler(std::string URI, WebServerHandler handler);
		void addGetHandler(std::string URI, WebServerHandler handler);
		void run(unsigned short port);

		void handleEventCallback(evhttp_request *request);

	protected:
		void createMainSocket(unsigned short port);

		void handleConnection(int clientSockFD);

	private:
		std::map<std::string, WebServerHandler> m_getHandlers;
		std::map<std::string, WebServerHandler> m_postHandlers;

		int m_mainSocketFD;
	};

}
