#include "blog_index.h"

lipido::WebResponse handleIndex(lipido::WebContext &ctx) {
	lipido::WebResponse response;

	//1. get list of posts
	//2. render posts
	//3. profit
	//4. ???

	response.body = "<html><head><title>fefemama</title></head><body>";
	response.body += "<h1>Welcome To The Thunderdome</h1>";
	response.body += "lol</body></html>";

	return response;
}


