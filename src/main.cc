#include <stdio.h>
#include "lipido.h"

lipido::response_t test_handler(lipido::context_t &context) {

	printf("test handler!\n");

	auto resp = lipido::response::make("hai, I'm a test handler :P");
	return resp;
}


int main(int argc, char **argv) {
	printf("starting fettemama 2.0 ...\n");
	auto server = lipido::server::make();

	/*
	lipido::server::add_get_handler(server, "/", [](lipido::context::context &ctx) -> lipido::response_t {
			printf("I liek lambdas\n");
			return lipido::response::make("LOL!");
			});
	*/
	lipido::server::add_get_handler(server, "/", test_handler);
	lipido::server::run(server, 8080);
}

