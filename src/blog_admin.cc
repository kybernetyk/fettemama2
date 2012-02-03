#include "blog_admin.h"
#include "database.h"
#include "admin_password"

lipido::WebResponse handleNewPost(lipido::WebContext &ctx) {
    lipido::WebResponse response;
    MySQLDatabase db;

    response.body = "<html><head><title>fefemama</title></head><body>";
    response.body += "<h1>Welcome To The Thunderdome</h1>";
    response.body += "<ul>";

    for (auto param : ctx.request.params) {
		response.body += param.first;
		response.body += " -> ";
		response.body += param.second;
		response.body += "<br>";
    }

	if (ctx.request.params["password"] != admin_password) {
		response.body = "LOL UR SUCK!<br>";
#if 0
		response.body += "the password '";
		response.body += ctx.request.params["password"];
		response.body += "' doesn't match the required password ";
		response.body += admin_password;
#endif
		return response;
	}

	std::string query = "insert into posts (content) values ('";
	query += db.escape(ctx.request.params["content"]);
	query += "');";
	printf("query will be: %s\n", query.c_str());
	try {
		db.query(query);
	} catch (std::string &ex) {
		printf("error while mysql query! %s\n", ex.c_str());
		response.httpCode = 501;
		response.httpReason = "Invalid Shit";
		response.body = ex;
		return response;
	}
	printf("query succeeded!\n");
	return response;
}
