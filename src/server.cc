#include <memory>
#include "server.h"

#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include <future>
#include <vector>
#include <fcntl.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

namespace lipido {

	//ev callback
	void ev_http_callback(evhttp_request *req, void *server_instance) {
		WebServer *me = (WebServer*)server_instance;
		me->handleEventCallback(req);
	}

	void WebServer::handleEventCallback(evhttp_request *request) {
		printf(">>> handling callback!\n");

		const evhttp_uri *uri = evhttp_request_get_evhttp_uri(request);

		std::string s_uri(evhttp_uri_get_path(uri));
		printf(">>> URI is %s\n", s_uri.c_str());

		WebServerHandler handler = nullptr;
		if (evhttp_request_get_command(request) == EVHTTP_REQ_GET) {
			handler = m_getHandlers[s_uri];
		} else if (evhttp_request_get_command(request) == EVHTTP_REQ_POST) {
			handler = m_postHandlers[s_uri];
		}

		if (!handler) {
			printf(">>> no handler for URI %s found. 404\n", s_uri.c_str());
			evhttp_send_reply(request, 404, "U SUCKS!", NULL);
			return;
		}

		WebRequest w_req;
		WebSession w_session;
		WebContext w_ctx(*this, w_req, w_session);
		auto resp = handler(w_ctx);

		printf(">>> response lenght %i\n", resp.body.length());
		evhttp_add_header(evhttp_request_get_output_headers(request),
							"Content-Type", "text/html");

		evbuffer *replbuf = evbuffer_new();
		evbuffer_add_printf(replbuf,"%s", resp.body.c_str());

		printf(">>> sending response ... ");
		evhttp_send_reply(request, 200, "OK", replbuf);
		printf("ok\n");

		if (replbuf)
			evbuffer_free(replbuf);

	}

	void WebServer::addGetHandler(std::string URI, WebServerHandler handler) {
		m_getHandlers[URI] = handler;
	}

	void WebServer::addPostHandler(std::string URI, WebServerHandler handler) {
		m_postHandlers[URI] = handler;
	}

	void WebServer::run(unsigned short port) {
		printf("server %p listening on port %i ...\n",this, port);

		//createMainSocket(port);
		std::shared_ptr<void> defer(nullptr, [m_mainSocketFD](void*){
									printf("closing socket FD: %i\n", m_mainSocketFD);
									close(m_mainSocketFD);
									});

		printf("socket created: %i\n", m_mainSocketFD);

		//shamelessly stolen from the libevent http sample
		event_base *base;
		evhttp *http;
		evhttp_bound_socket *handle;

		base = event_base_new();
		http = evhttp_new(base);

		evhttp_set_gencb(http, ev_http_callback, this);

		handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);

		event_base_dispatch(base);

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
		std::shared_ptr<void> defer(nullptr, [clientSockFD](void *){
										printf("closing sock %i!\n", clientSockFD);
										close(clientSockFD);
									});
		for (;;) {
			char buf[4096];
			bzero(buf, 4096);

			int n = read(clientSockFD, buf, 4096);
			printf("received: %i byes\n", n);
			if (n == 0 || n == -1) {
				return;
			}
		}
	}
}
