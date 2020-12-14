#ifndef ATM_UI
#define  ATM_UI

#include <iostream>
#include <mysql.h>
#include <string>

int validateCreditCard(MYSQL*&, MYSQL_RES*&, MYSQL_ROW&, std::string&);
#endif