#include "database.h"
#include <my_global.h>
#include <mysql.h>

MySQLDatabase::MySQLDatabase() {
	//mysql connect

}

MySQLDatabase::~MySQLDatabase() {
	//mysql disconnect
}

std::string MySQLDatabase::version() {
 auto pstr = mysql_get_client_info();
 std::string s(pstr);
 return s;
}

std::vector<std::map<std::string, std::string>> MySQLDatabase::query(std::string qry) {
	std::vector<std::map<std::string, std::string>> vec;

	MYSQL *conn = mysql_init(NULL);
	mysql_real_connect(conn,
						"localhost",
						"root", //user
						"", //pass
						"fettemama", //db
						0, NULL, 0 );
	mysql_query(conn, qry.c_str());
	MYSQL_RES *result = mysql_store_result(conn);
	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	MYSQL_FIELD *fields = mysql_fetch_fields(result);


	while ((row = mysql_fetch_row(result))) {
		std::map<std::string, std::string> cur_row;
		for (int i = 0; i < num_fields; i++) {
			std::string fieldname(fields[i].name);
			cur_row[fieldname] = std::string(row[i]);
		}
		vec.push_back(cur_row);
	}
	return vec;
}
