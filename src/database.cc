#include "database.h"

MySQLDatabase::MySQLDatabase() {
    //mysql connect
    m_dbConnection = mysql_init(NULL);
    mysql_real_connect(m_dbConnection,
                       "localhost",
                       "root", //user
                       "", //pass
                       "fettemama", //db
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

std::vector<std::map<std::string, std::string>> MySQLDatabase::query(std::string qry) {
    std::vector<std::map<std::string, std::string>> vec;

    mysql_query(m_dbConnection, qry.c_str());
    MYSQL_RES *result = mysql_store_result(m_dbConnection);
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
