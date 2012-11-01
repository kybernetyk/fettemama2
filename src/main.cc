#include <stdio.h>
#include <memory>
#include "lipido.h"
#include "blog_index.h"
#include "blog_admin.h"
#include "blog_rss.h"

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
	std::shared_ptr<void> defer(nullptr, [](void *) {
		printf("thx and bai\n");
	});

	printf("starting fettemama 2.0 ...\n");

	auto server = lipido::WebServer();

	server.addGetHandler("/", handleIndex);
	server.addPostHandler("/new_post();", handleNewPost);
	server.addGetHandler("/rss.xml", handleRSS);
	server.addGetHandler("/wp-rss2.php", handleRSS);
	server.addGetHandler("/index.php/feed/", handleRSS);
	server.addGetHandler("/index.php/feed/atom/", handleRSS);
	server.addGetHandler("/feed/rss2/", handleRSS);
	server.addGetHandler("/feed/", handleRSS);
	/*
	    server.addGetHandler("/fefe", [](lipido::WebContext &context) -> lipido::WebResponse {
	        printf("fefe ist fett\n");
	        lipido::WebResponse r;
	        r.body = "<h1>Fefe ist ein Ferkel!</h1> LOL!";
	        return r;
	    });
		server.addPostHandler("/post", post_handler);
	*/
	auto if1 = std::pair<std::string,unsigned short>("0.0.0.0", 80);
	auto if2 = std::pair<std::string, unsigned short>("fc0a:ef4c:adff:e2f9:639f:7f79:cc16:99db", 80);
	auto interfaces = std::vector<std::pair<std::string, unsigned short>>;
	interfaces.push_bak(if1);
	interfaces.push_bak(if2);
	server.run(interfaces);
}

