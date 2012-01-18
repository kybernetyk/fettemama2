#include <stdio.h>
#include "lipido.h"

lipido::response::response test_handler(lipido::context::context &context) {

	printf("test handler!\n");

	lipido::response::response resp;
	return resp;
}


int main(int argc, char **argv) {
	printf("starting fettemama 2.0 ...\n");
	auto server = lipido::server::make_server();

	lipido::server::add_get_handler(server, "/", test_handler);

	lipido::server::run(server, 8080);
}

