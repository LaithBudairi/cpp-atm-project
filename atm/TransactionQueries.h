#ifndef ATM_TRANSACTION_QUERIES
#define  ATM_TRANSACTION_QUERIES
#include <mysql.h>
#include "Transaction.h"

void createTransaction(MYSQL*&, Transaction*);
int updateBalance(MYSQL*&, Transaction*);
int tranferTo(MYSQL*&, Transaction*);
int findAccountHolderWithId(MYSQL*&, MYSQL_RES*&, MYSQL_ROW& ,int&);
int validateCreditCard(MYSQL*&, MYSQL_RES*&, MYSQL_ROW&, std::string&);
void findAllDeposits(MYSQL*&, MYSQL_RES*&, int);
void findAllWithdraws(MYSQL*&, MYSQL_RES*&, int);
void findAllTransfers(MYSQL*&, MYSQL_RES*&, int);
void findAllTransactions(MYSQL*&, MYSQL_RES*&, int);
#endif