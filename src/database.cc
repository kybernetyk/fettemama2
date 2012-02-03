#include "database.h"
#include "config.h"
#include <memory>

MySQLDatabase::MySQLDatabase() {
    //mysql connect
    m_dbConnection = mysql_init(NULL);
    mysql_real_connect(m_dbConnection,
                       cfg::db::host,
                       cfg::db::user, //user
                       cfg::db::pass, //pass
                       cfg::db::dbname, //db
                       0, NULL, 0 );
}

MySQLDatabase::~MySQLDatabase() {
    //mysql disconnect
    mysql_close(m_dbConnection);
}

std::string MySQLDatabase::version() {
    auto pstr = mysql_get_client_info();
    std::string s(pstr);
    return s;
}

std::string MySQLDatabase::escape(std::string str) {
	size_t escaped_len = str.length()*2+1;
	char *escaped_qry = new char[escaped_len];
	std::shared_ptr<void> deferes(nullptr, [escaped_qry](void*) {
					delete []escaped_qry;
				});
	if (!mysql_real_escape_string(m_dbConnection,
							escaped_qry,
							str.c_str(),
							str.length())) {
    	auto err = mysql_error(m_dbConnection);
    	std::string reason = "mysql query error: ";
    	reason += std::string(err);
		throw reason;
	}
	return std::string(escaped_qry);
}

std::vector<std::map<std::string, std::string>> MySQLDatabase::query(std::string qry) {
    std::vector<std::map<std::string, std::string>> vec;


    if (mysql_query(m_dbConnection, qry.c_str()) != 0) {
    	auto err = mysql_error(m_dbConnection);
    	std::string reason = "mysql query error: ";
    	reason += std::string(err);
		throw reason;
    }
    MYSQL_RES *result = mysql_store_result(m_dbConnection);
    if (!result)
      return vec;

	std::shared_ptr<void> defer(nullptr, [result](void*) {
					mysql_free_result(result);
				});
    int num_fields = mysql_num_fields(result);
	if (num_fields == 0)
		return vec;

    MYSQL_ROW row;
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    if (!fields)
		return vec;

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
