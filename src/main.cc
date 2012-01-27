#include <stdio.h>
#include <memory>
#include "lipido.h"

lipido::WebResponse test_handler(lipido::WebContext &context) {
	printf("test handler handling:\n\tserver: %p\n", &context.server);

	lipido::WebResponse resp;
	return resp;
}


int main(int argc, char **argv) {
	printf("starting fettemama 2.0 ...\n");
	auto server = std::shared_ptr<lipido::WebServer>(new lipido::WebServer());

	/*
	lipido::server::add_get_handler(server, "/", [](lipido::context::context &ctx) -> lipido::response_t {
			printf("I liek lambdas\n");
			return lipido::response::make("LOL!");
			});
	*/
	server->addGetHandler("/", test_handler);
	server->run(8080);
}

