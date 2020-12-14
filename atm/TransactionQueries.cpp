#include <sstream>
#include "Transaction.h"
#include <mysql.h>
void createTransaction(MYSQL*& connection, Transaction* t) {
	std::stringstream query;

	query << "INSERT INTO `atm`.`transaction` (`type`, `from`, `to`, `amount`, `account_type_id`) VALUES" <<
		"('" << t->getTranType() << "', '" << t->getFrom() << "', '" << t->getTo() << "', '" 
		<< t->getAmount() << "', '" << t->getAmountType() << "')";

	mysql_query(connection, query.str().c_str());
}

int updateBalance(MYSQL*& connection, Transaction* t)
{
	std::stringstream query;
	char op;

	if (t->getTranType() == DEPOSIT) {
		op = '+';
	}
	else if (t->getTranType() == WITHDRAW) {
		op = '-';
	}

	query << "UPDATE `atm`.`account_holder_has_account_type` SET `balance` =" <<
		"balance " << op << t->getAmount() << " WHERE (`account_type_id` =" << t->getAmountType() <<
		") and (`account_holder_id` = '" << t->getFrom() << "')";

	return mysql_query(connection, query.str().c_str());
}
