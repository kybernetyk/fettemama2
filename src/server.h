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
struct evhttp_uri;

namespace lipido {

	class WebServer {
			typedef std::function<WebResponse(lipido::WebContext &context)> WebServerHandler;

		public:
			WebServer();
			~WebServer() {}
			void addPostHandler(std::string URI, WebServerHandler handler);
			void addGetHandler(std::string URI, WebServerHandler handler);
			void run(std::vector<std::pair<std::string, unsigned short>> interfaces);

			void handleEventCallback(evhttp_request *request);

		protected:
			void createMainSocket(unsigned short port);
			void handleConnection(int clientSockFD);
			std::map<std::string, std::string> extractParams(const char *query);
			std::map<std::string, std::string> extractGETParams(evhttp_request *request);
			std::map<std::string, std::string> extractPOSTParams(evhttp_request *request);

		private:
			std::map<std::string, WebServerHandler> m_getHandlers;
			std::map<std::string, WebServerHandler> m_postHandlers;

			int m_mainSocketFD;
	};

}
