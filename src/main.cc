#include <stdio.h>
#include <memory>
#include "lipido.h"
#include "blog_index.h"
#include "blog_admin.h"
#include "blog_rss.h"

lipido::WebResponse test_handler(lipido::WebContext &context) {
	lipido::WebResponse resp;
	resp.body = "<pre>";
	resp.body += "RESPONSE for: ";
	resp.body += context.request.URI;
	resp.body += "\n";

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
	server.addGetHandler("/test_get", test_handler);
	server.addPostHandler("/test_post", test_handler);

	server.addGetHandler("/", handleIndex);
	server.addPostHandler("/new_post();", handleNewPost);
	server.addGetHandler("/rss.xml", handleRSS);
	server.addGetHandler("/wp-rss2.php", handleRSS);
	server.addGetHandler("/index.php/feed/", handleRSS);
	server.addGetHandler("/index.php/feed/atom/", handleRSS);
	server.addGetHandler("/feed/rss2/", handleRSS);
	server.addGetHandler("/feed/", handleRSS);

	auto interfaces = std::vector<std::pair<std::string, unsigned short>> { 
		std::make_pair("0.0.0.0", lipido::cfg::listen_port),
	};
	   
	server.run(interfaces);
}

