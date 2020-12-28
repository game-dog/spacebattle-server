#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <vector>
#include <string>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

class Database {
public:
	//~Database() { mysql_close(&conn); }
	static bool Init();
	static std::vector<std::vector<std::string>> Query(const char* inQuery);

	static void ReportError(const char* inOperationDesc);

private:
	static MYSQL conn;

	static const char* user;
	static const char* passwd;
	static const char* db;

	static const int port = 0;
};

#endif