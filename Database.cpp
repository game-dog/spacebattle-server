#include "Database.h"
#include "Logger.h"

MYSQL Database::conn = MYSQL();

const char* Database::user = "root";
const char* Database::passwd = "1234";
const char* Database::db = "battleship";

bool Database::Init() {
	MYSQL* err;

	err = mysql_init(&conn);
	if (err == NULL) {
		Database::ReportError("Database::Init (mysql_init)");
		return FALSE;
	}

	err = mysql_real_connect(&conn, NULL, user, passwd, db, port, NULL, 0);
	if (err == NULL) {
		Database::ReportError("Database::Init (mysql_real_connect)");
		return FALSE;
	}

	return TRUE;
}

std::vector<std::vector<std::string>> Database::Query(const char *inQuery) {
	mysql_query(&conn, inQuery);

	MYSQL_RES* res = mysql_store_result(&conn);
	int fields = mysql_num_fields(res);

	std::vector<std::vector<std::string>> ret;

	MYSQL_ROW row;
	while (row = mysql_fetch_row(res)) {
		ret.push_back(std::vector<std::string>());
		for (int i = 0; i < fields; ++i) ret.back().push_back(row[i]);
	}

	mysql_free_result(res);
	return ret;
}

void Database::ReportError(const char* inOperationDesc) {
	LPVOID lpMsgBuf;
	DWORD errNum = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	// TODO: 에러 처리 코드
}