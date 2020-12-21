#ifndef ATM_UI
#define  ATM_UI
#include <mysql.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "TransactionQueries.h"
void displayOperations();
void displayCurrency();
void displayTransactions();
void depositProcedure(MYSQL*&, std::unique_ptr<Transaction>&, int);
void withdrawProcedure(MYSQL*&, std::unique_ptr<Transaction>&, int);
void transferProcedure(MYSQL*&, std::unique_ptr<Transaction>&, int);
void displayAllDeposits(MYSQL*&, MYSQL_RES*&, MYSQL_ROW&, int);
void displayAllWithdraws(MYSQL*&, MYSQL_RES*&, MYSQL_ROW&, int);
void displayAllTransfers(MYSQL*&, MYSQL_RES*&, MYSQL_ROW&, int);
void displayAllTransactions(MYSQL*&, MYSQL_RES*&, MYSQL_ROW&, int);
#endif