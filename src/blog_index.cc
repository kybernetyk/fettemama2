#include <iostream>
#include <string>
#include <sstream>

#include "blog_index.h"
#include "database.h"

static void render_head(std::stringstream &body) {
	body << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//ENi\">";
	body << "<html>";
	body << "<head>";
	body << "<title>fefemama.org - THE BEST BLOG IN THE UNIVERSE WRITTEN IN C++ :]</title>";
	body << "<body>";

	body << "<h1><a href=/>Fefemama</a></h1>";
	body << "<b>I love the smell of nopslides in the morning ...</b>";
	
	body << "<p align=right>Fragen? <a href=/faq.html>Antworten!</a><br>";
	body << "<i>Einsendungen und Beschwerden bitte an blog@fettemama.org</i><br>";
	body << "IRC: irc.hackint.eu | #fm</p>";
}

static void render_ass(std::stringstream &body) {
	body << "<div align=right>Proudly made without PHP, Java, Perl, and Postgres<br><a href=/impressum.html>Impressum</a></div>";
	body << "</body></html>";
}

static lipido::WebResponse render_frontpage(lipido::WebContext &ctx) {
	lipido::WebResponse response;
	MySQLDatabase db;

	std::stringstream body;
	render_head(body);

	std::vector<std::map<std::string, std::string>> posts;
	try {
		posts = db.query("select id, content, DATE_FORMAT(timestamp,'%a %b %e %Y') as timestamp from posts order by id desc limit 20;");
	} catch (std::string &ex) {
		response.body = ex;
		response.httpCode = 501;
		return response;
	}

	std::string day = "";
	for (auto post : posts) {
		std::string postdate = post["timestamp"];
		if (day != postdate) {
			day = postdate;
			body << "</ul>";
			body << "<h2>";
			body << postdate;
			body << "</h2><ul>";
		}
		//streams SIND SO FERDAMMT KUHL WEIL SIE LESBAR SIND UNSO ...
		body << "<li><a href=/?pid=" << post["id"] << ">[l]</a> " << post["content"] << "</li>";
	}
	
	body << "</ul>";
	render_ass(body);
	response.body = body.str();
	return response;
}

static lipido::WebResponse render_post(lipido::WebContext &ctx) {
	MySQLDatabase db;
	lipido::WebResponse response;
	std::stringstream body;

	render_head(body);

	std::vector<std::map<std::string, std::string>> posts;
	try {
		std::stringstream qry;
		qry << "select id, content, DATE_FORMAT(timestamp,'%a %b %e %Y') as timestamp from posts where id = ";
		qry << ctx.request.params["pid"];
		qry << " order by id desc limit 1;";

		printf("query: %s\n", qry.str().c_str());
		posts = db.query(qry.str());
	} catch (std::string &ex) {
		response.body = ex;
		response.httpCode = 501;
		return response;
	}

	if (posts.size() <= 0) {
		body << "LOL THIS POSTS NOT FOUNDS DU TARD!";
		render_ass(body);
		response.body = body.str();
		return response;
	}

	auto post = posts[0];

	body << "<h2>";
	body << post["timestamp"];
	body << "</h2><ul>";
	
	body << "<li><a href=/?pid=" << post["id"] << ">[l]</a> " << post["content"] << "</li></ul>";


	render_ass(body);
	response.body = body.str();
	return response;
}

static lipido::WebResponse render_niy(lipido::WebContext &ctx) {
	lipido::WebResponse response;
	std::stringstream body;
	render_head(body);

	body << "<pre>\n";
	body << "Not implemented yet!\n";

	body << "Debug:" << std::endl;
	if (ctx.request.params.size()) {
		body << "GET params:\n";
		for (auto param : ctx.request.params) {
			body << param.first << " -> " << param.second << std::endl;
		}
	}
	body << "</pre>\n";
	render_ass(body);
	response.body = body.str();
	return response;
}

lipido::WebResponse handleIndex(lipido::WebContext &ctx) {
	if (ctx.request.params.size() > 0) {
		if (ctx.request.params["pid"].length() > 0)
			return render_post(ctx);

		return render_niy(ctx);
	}

	return render_frontpage(ctx);
}
