#include <iostream>
#include <string>
#include <sstream>

#include "database.h"
#include "html_strip.h"
#include "blog_rss.h"

static void render_head(std::stringstream &body) {
	body << "<?xml version='1.0' encoding='UTF-8'?>";
	body << "<rss version='2.0'>";
	body << "<channel>";
	body << "<title>fefemama</title>";
	body << "<link>https://fettemama.org</link>";
	body << "<description>Fefe liebt Spanferkel.jpg</description>";
	body << "<language>de</language>";

}

static void render_ass(std::stringstream &body) {
	body << "</channel></rss>";
}

static lipido::WebResponse render_rss(lipido::WebContext &ctx, int limit = 0) {
	lipido::WebResponse response;
	MySQLDatabase db;

	std::stringstream body;
	render_head(body);

	std::vector<std::map<std::string, std::string>> posts;
	try {
		std::stringstream qry;
		qry << "select id, content, DATE_FORMAT(timestamp,'%a, %d %b %Y %T') as timestamp from posts order by id desc";
		if (limit > 0) {
			qry << " limit ";
			qry << limit;
		}
		qry << ";";

		posts = db.query(qry.str());
	} catch (std::string &ex) {
		response.body = ex;
		response.httpCode = 501;
		return response;
	}

	std::string day = "";
	for (auto post : posts) {
			body << "<item>";
			body << "<description><![CDATA[";
			body << post["content"];
			body << "]]>";
			body << "</description>";
			
			body << "<title><![CDATA[";
			size_t title_len = 64;
			std::string title = html_strip(post["content"]);
			if (title.length() < title_len)
				title_len = title.length(); 
			title = title.substr(0, title_len);	
			body << title;
			body << "]]></title>";
			
			body << "<link>";
			body << "https://fettemama.org/?pid=";
			body << post["id"];
			body << "</link>";
			
			body << "<guid>";
			body << "https://fettemama.org?pid=3000"; //3000.id so google will update lol
			body << post["id"];
			body << "</guid>";
			
			body << "<pubDate>";
			body << post["timestamp"];	
			body << " +0000</pubDate>";
			body << "</item>";
	}
	
	render_ass(body);
	response.body = body.str();
	response.contentType = "application/rss+xml";
	return response;
}

lipido::WebResponse handleRSS(lipido::WebContext &ctx) {
	return render_rss(ctx);
}
