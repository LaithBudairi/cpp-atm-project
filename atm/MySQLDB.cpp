#include <mysql.h>

MYSQL* connect(const char*& host, const char*& username, const char*& password, const char*& dbName, unsigned int& port) {

	MYSQL* connection = mysql_init(0);
	connection = mysql_real_connect(connection, host, username, password, dbName, port, NULL, 0);
	return connection;
}