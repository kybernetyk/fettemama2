#include "blog_index.h"
#include "database.h"


lipido::WebResponse handleIndex(lipido::WebContext &ctx) {
	lipido::WebResponse response;
	MySQLDatabase db;

	response.body = "<html><head><title>fefemama</title></head><body>";
	response.body += "<h1>Welcome To The Thunderdome</h1>";
	response.body += "<ul>";

	auto posts = db.query("select content, DATE_FORMAT(timestamp,'%a %b %e %Y') as timestamp from posts order by id desc limit 20;");

	std::string day = "";
	for (auto post : posts) {
		std::string postdate = post["timestamp"];
		if (day != postdate) {
			day = postdate;
			response.body += "</ul>";
			response.body += "<h2>";
			response.body += postdate;
			response.body += "</h2><ul>";
		}
		response.body += "<li><p>";
		response.body += post["content"];
		response.body += "</p></li>";
	}

	response.body += "</ul>";

/*	for (std::map<std::string,std::string> row : posts) {
		for (auto k : row) {
			char buf[4096];

			sprintf(buf,"<p>%s => %s</p>\n", k.first.c_str(), k.second.c_str());
			response.body += buf;
		}
	}
*/

	response.body += "</body></html>";

	return response;
}


