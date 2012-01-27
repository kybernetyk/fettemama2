#include <memory>
#include "server.h"

#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>

namespace lipido {


	void WebServer::addGetHandler(std::string URI, WebServerHandler handler) {
		m_getHandlers[URI] = handler;
	}

	void WebServer::addPostHandler(std::string URI, WebServerHandler handler) {
		m_postHandlers[URI] = handler;
	}

	void WebServer::run(unsigned short port) {
		printf("server %p listening on port %i ...\n",this, port);

		createMainSocket(port);
		std::shared_ptr<void> defer(nullptr, [=](void*){
									printf("closing socket FD: %i\n", m_mainSocketFD);
									close(m_mainSocketFD);
									});

		printf("socket created: %i\n", m_mainSocketFD);

		for (;;) {
			sockaddr_in clientAddr;
			socklen_t clientAddrLen = sizeof(clientAddr);

			int newSockFD = accept(m_mainSocketFD, (struct sockaddr*)&clientAddr, &clientAddrLen);
			//printf("got client with len: %i\n", clientAddrLen);
			char hostname[256];
			char servname[256];

			getnameinfo((struct sockaddr*)&clientAddr, clientAddrLen,
						hostname, 256,
						servname, 256,
						0);
			printf("connection => %s:%s\n", hostname, servname);

			std::thread t(&WebServer::handleConnection, this, newSockFD);
			t.join();
		}


/*
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
*/
	}

	void WebServer::createMainSocket(unsigned short port) {
		m_mainSocketFD = socket(AF_INET, SOCK_STREAM, 0);
		if (m_mainSocketFD < 0) {
			throw "couldn't create socket!";
		}

		sockaddr_in server_addr;
		bzero((char *)&server_addr, sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(port);

		if (bind(m_mainSocketFD, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
			throw std::string("couldn't bind socket!");
		}
		if (listen(m_mainSocketFD, 5) < 0) {
			throw std::string("couldn't listen!");
		}
	}

	void WebServer::handleConnection(int clientSockFD) {
		printf("lol!\n");
		for (;;) {
			char buf[4096];
			bzero(buf, 4096);

			int n = read(clientSockFD, buf, 4096);
			printf("received:\n-----------------------------------\n%s\n-------------------------\n", buf);
			if (n == 0) {
				close(clientSockFD);
				return;
			}
		}
	}
}
