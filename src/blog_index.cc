#include <iostream>
#include <string>
#include <sstream>
#include <string.h>

#include "blog_index.h"
#include "database.h"

static void render_head(std::stringstream &body) {
	body << "<!doctype html>";
	body << "\n";
	body << "<html lang='de'><meta charset='utf-8'>";
	body << "\n";
	body << "<meta name='viewport' content='width=device-width, initial-scale=1'>";
	body << "\n";
	body << "<link rel='alternate' type='application/rss+xml' title='RSS FEED DU AFFE' href='/rss.xml'>";
	body << "\n";
	body << "<title>fefemama.org - Danke, Greta!</title>";
	body << "\n";

	body << "<h2><a href='/' style='text-decoration:none;color:black'>Fefemama</a></h2>";
	body << "\n\n";
	body << "<b>Das Leitmedium der Petromaskulinitaet im deutschsprachigen Netz</b>";
	body << "\n\n";
	
	body << "<p style='text-align:right'>Fragen? <a href=/faq.html>Antworten!</a><br>";
	body << "\n\n";
	body << "<i>Einsendungen und Beschwerden bitte an blog@fettemama.org</i><br>";
	body << "\n\n";
}

static void render_ass(std::stringstream &body) {
	body << "<p><div style='text-align:center'><a href=/?mode=all>Alle Posts</a></div>";
	body << "\n";
	body << "<div style='text-align:right'>I use <a href=/img/archbtw.png>Arch Linux btw...</a><br><a href=/impressum.html>Impressum</a></div>";
	body << "\n";
	body << "</html>";
}

/** render the frontpage. optionally limit the post count. default is 20. if limit == 0 all posts in db will be rendered */
static lipido::WebResponse render_frontpage(lipido::WebContext &ctx, int limit = 20) {
	lipido::WebResponse response;
	MySQLDatabase db;

	std::stringstream body;
	render_head(body);

	std::vector<std::map<std::string, std::string>> posts;
	try {
		std::stringstream qry;
		qry << "select id, content, DATE_FORMAT(timestamp,'%a %b %e %Y') as timestamp from posts order by id desc";
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

	std::string day = "first";
	for (auto post : posts) {
		std::string postdate = post["timestamp"];
		if (day != postdate) {
			if (day != "first") {
				body << "</ul>";
				body << "\n";
			}
			day = postdate;
			body << "<h3>";
			body << postdate;
			body << "</h3>";
			body << "\n\n";
			body << "<ul>\n";
		}
		body << "<li><a href=/?pid=" << post["id"] << ">[l]</a> " << post["content"] << "\n";
	}
	
	body << "</ul>";
	body << "\n";
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
	auto post_id = ctx.request.params["pid"];
	size_t pos = post_id.find("3000");
	if (pos == 0) {
		printf("post id contains hacky prefix at index %lu. purging '%s'\n", pos, post_id.c_str());
		pos += strlen("3000");
		post_id = post_id.substr(pos, std::string::npos);
		printf("new post id is: '%s'\n", post_id.c_str());
		response.httpCode = 301;
		response.httpReason = "Moved Permanently";
		response.body = "/?pid=";
		response.body += post_id;
		return response;
	}
	try {
		std::stringstream qry;
		qry << "select id, content, DATE_FORMAT(timestamp,'%a %b %e %Y') as timestamp from posts where id = ";
		qry << db.escape(post_id);
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

static lipido::WebResponse render_month(lipido::WebContext &ctx) {
	lipido::WebResponse response;
	std::stringstream body;
	render_head(body);

	response.body += "<pre>underconstruction.gif</pre>";

	render_ass(body);
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
		if (ctx.request.params["mon"].length() > 0)
			return render_month(ctx);
		if (ctx.request.params["mode"] == "all") {
			return render_frontpage(ctx, 0);
		}

		return render_niy(ctx);
	}

	return render_frontpage(ctx);
}
