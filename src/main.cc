#include <stdio.h>
#include <memory>
#include "lipido.h"

lipido::WebResponse test_handler(lipido::WebContext &context) {
	printf("test handler handling:\n\tserver: %p\n", &context.server);

	lipido::WebResponse resp;
	resp.body = "LOL ICH MACH DICH PLATT!";
	return resp;
}


int main(int argc, char **argv) {
	std::shared_ptr<void> defer(nullptr, [](void*) {
								printf("thx and bai\n");
								});

	printf("starting fettemama 2.0 ...\n");

	auto server = lipido::WebServer();

	/*
	lipido::server::add_get_handler(server, "/", [](lipido::context::context &ctx) -> lipido::response_t {
			printf("I liek lambdas\n");
			return lipido::response::make("LOL!");
			});
	*/
	server.addGetHandler("/", test_handler);
	server.addGetHandler("/fefe", [](lipido::WebContext &context) -> lipido::WebResponse {
							printf("fefe ist fett\n");
							lipido::WebResponse r;
							r.body = "<h1>Fefe ist ein Ferkel!</h1> LOL!";
							return r;
						});

	server.run(8080);
}

