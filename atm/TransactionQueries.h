#ifndef ATM_TRANSACTION_QUERIES
#define  ATM_TRANSACTION_QUERIES
#include <mysql.h>
#include "Transaction.h"

void createTransaction(MYSQL*&, Transaction*);
int updateBalance(MYSQL*&, Transaction*);

#endif