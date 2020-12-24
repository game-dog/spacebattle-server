#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

class Database {
public:
	static bool Init();
	static void Connect();

private:
	static MYSQL conn;
};

#endif