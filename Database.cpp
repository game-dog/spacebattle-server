#include "Database.h"
#include "Logger.h"

MYSQL Database::conn = MYSQL();

bool Database::Init() {

}

void Database::Connect() {
	mysql_init(&conn);
	mysql_real_connect(&conn, NULL, "root", "1234", "battleship", 0, NULL, 0);

	mysql_query(&conn, "USE battleship");
	mysql_query(&conn, "SELECT * FROM account");

	MYSQL_RES* res = mysql_store_result(&conn);
	int fields = mysql_num_fields(res);

	MYSQL_ROW row;
	while (row = mysql_fetch_row(res)) {
		for (int i = 0; i < fields; ++i) {
			LOG(row[i]);
		}
	}

	mysql_free_result(res);
	mysql_close(&conn);
}