#include <stdio.h>
#include <memory>
#include "lipido.h"
#include "blog_index.h"
#include "blog_admin.h"

lipido::WebResponse test_handler(lipido::WebContext &context) {
    lipido::WebResponse resp;
    resp.body = "LOL ICH MACH DICH PLATT!";
    return resp;
}

lipido::WebResponse post_handler(lipido::WebContext &context) {
	lipido::WebResponse resp;
	resp.body = "<pre>";
	resp.body += "POST RESPONSE!\n";

	for (auto param : context.request.params) {
		resp.body += param.first;
		resp.body += " -> ";
		resp.body += param.second;
		resp.body += "\n";
	}

	resp.body += "</pre>\n";
	return resp;
}


int main(int argc, char **argv) {
    std::shared_ptr<void> defer(nullptr, [](void*) {
        printf("thx and bai\n");
    });

    printf("starting fettemama 2.0 ...\n");

    auto server = lipido::WebServer();

    server.addGetHandler("/", handleIndex);
	server.addPostHandler("/new_post();", handleNewPost);
#if 0
    server.addGetHandler("/fefe", [](lipido::WebContext &context) -> lipido::WebResponse {
        printf("fefe ist fett\n");
        lipido::WebResponse r;
        r.body = "<h1>Fefe ist ein Ferkel!</h1> LOL!";
        return r;
    });
	server.addPostHandler("/post", post_handler);
#endif

    server.run(8080);
}

