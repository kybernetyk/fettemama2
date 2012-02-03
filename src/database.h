#pragma once
#include <vector>
#include <map>
#include <string>
#include <my_global.h>
#include <mysql.h>


class MySQLDatabase {
public:
	MySQLDatabase();
	~MySQLDatabase();

	//returns a vector of vectors of strings
	//-> a vector of rows. (each row itself is a vector of strings)
	std::vector<std::map<std::string, std::string>> query(std::string qry);
	std::string version();

	std::string escape(std::string str);

protected:
	MYSQL *m_dbConnection;
};
