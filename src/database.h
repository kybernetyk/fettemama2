#pragma once
#include <vector>
#include <map>
#include <string>

class MySQLDatabase {
public:
	MySQLDatabase();
	~MySQLDatabase();

	//returns a vector of vectors of strings
	//-> a vector of rows. (each row itself is a vector of strings)
	std::vector<std::map<std::string, std::string>> query(std::string qry);
	std::string version();
};
