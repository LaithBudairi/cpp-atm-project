#ifndef ATM_MY_SQL_DB
#define  ATM_MY_SQL_DB
#include <mysql.h>

MYSQL* connect(const char*&, const char*&, const char*&, const char*&, unsigned int&);
#endif