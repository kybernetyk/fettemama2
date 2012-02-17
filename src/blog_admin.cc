#include "blog_admin.h"
#include "database.h"
#include "admin_password"

lipido::WebResponse handleNewPost(lipido::WebContext &ctx) {
	lipido::WebResponse response;
	MySQLDatabase db;

	response.body = "<html><head><title>fefemama</title></head><body>";
	response.body += "<h1><a href=/>Thanks ur for ur cooperation lol</a></h1>";
	response.body += "<ul>";

	for (auto param : ctx.request.params) {
		response.body += param.first;
		response.body += " -> ";
		response.body += param.second;
		response.body += "<br>";
	}

	if (ctx.request.params["password"] != admin_password) {
		response.body = "<a href=/>LOL UR SUCK!</a><br>";
		return response;
	}

	std::string query = "insert into posts (content) values ('";
	query += db.escape(ctx.request.params["content"]);
	query += "');";

	try {
		db.query(query);
	} catch (std::string &ex) {
		response.httpCode = 501;
		response.httpReason = "Invalid Shit";
		response.body = ex;
		return response;
	}

	return response;
}
