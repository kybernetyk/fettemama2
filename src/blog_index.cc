#include "blog_index.h"
#include "database.h"


lipido::WebResponse handleIndex(lipido::WebContext &ctx) {
	lipido::WebResponse response;

	//1. get list of posts
	//2. render posts
	//3. profit
	//4. ???

	MySQLDatabase db;


	response.body = "<html><head><title>fefemama</title></head><body>";
	response.body += "<h1>Welcome To The Thunderdome</h1>";

	char buf[512];
	sprintf(buf, "<i>running mysql %s</i>", db.version().c_str());
	response.body += std::string(buf);

	response.body += "<div class='content'>";

	auto posts = db.query("select * from posts order by id desc;");

	for (auto post : posts) {
		response.body += "<li>(";
		response.body += post["id"];
		response.body += ", ";
		response.body += post["timestamp"];
		response.body += "): ";
		response.body += post["content"];
		response.body += "</li>";
	}
/*
	for (std::map<std::string,std::string> row : rows) {
		for (auto k : row) {
			char buf[4096];

			sprintf(buf,"<p>%s => %s</p>\n", k.first.c_str(), k.second.c_str());
			response.body += buf;
		}
	}
*/

	response.body += "</div>";
	response.body += "</body></html>";

	return response;
}


