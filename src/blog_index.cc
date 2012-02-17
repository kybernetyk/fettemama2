#include <iostream>
#include <string>
#include <sstream>

#include "blog_index.h"
#include "database.h"

lipido::WebResponse handleIndex(lipido::WebContext &ctx) {
	lipido::WebResponse response;
	MySQLDatabase db;

	//TODO: move this html generation to a template system ... !!!
	std::stringstream body;

	body << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//ENi\">";
	body << "<html>";
	body << "<head>";
	body << "<title>fefemama.org - THE BEST BLOG IN THE UNIVERSE WRITTEN IN C++ :]</title>";
	body << "<body>";

	body << "<h1><a href=/>Fefemama</a></h1>";
	body << "<b>I love the smell of nopslides in the morning ...</b><br>";
	
	body << "<p align=right>Fragen? <a href=/faq.html>Antworten!</a><br>";
	body << "<i>Einsendungen und Beschwerden bitte an blog@fettemama.org</i></p>";
	body << "<p align=right>IRC: irc.hackint.eu | #fm</p>";

	body << "<ul>";

	std::vector<std::map<std::string, std::string>> posts;
	try {
		posts = db.query("select content, DATE_FORMAT(timestamp,'%a %b %e %Y') as timestamp from posts order by id desc limit 20;");
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

		body << "<li><p>" << post["content"] << "</p></li>";
	}

	body << "</ul>";

	body << "<hr>Debug:<hr>";

	if (ctx.request.params.size()) {
		body << "GET params:<pre>\n";

		for (auto param : ctx.request.params) {
			body << param.first << " -> " << param.second << "\n";
		}

		body << "</pre>";
	}

	body << "</body></html>";
	response.body = body.str();
	return response;
}


