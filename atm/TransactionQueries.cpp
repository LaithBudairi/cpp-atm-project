#include <iostream>
#include <sstream>
#include <mysql.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "Transaction.h"
#include "TransactionQueries.h"

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
	else {
		op = '-';
	}

	query << "UPDATE `atm`.`account_holder_has_account_type` SET `balance` =" <<
		"balance " << op << t->getAmount() << " WHERE (`account_type_id` =" << t->getAmountType() <<
		") and (`account_holder_id` = '" << t->getFrom() << "')";

	return mysql_query(connection, query.str().c_str());
}
int tranferTo(MYSQL*& connection, Transaction* t) {
	std::stringstream query;

	query << "UPDATE `atm`.`account_holder_has_account_type` SET `balance` =" <<
		"balance " << '+' << t->getAmount() << " WHERE (`account_type_id` =" << t->getAmountType() <<
		") AND (`account_holder_id` = '" << t->getTo() << "')";

	return mysql_query(connection, query.str().c_str());
}


int findAccountHolderWithId(MYSQL*& connection, MYSQL_RES*& res, MYSQL_ROW& row, int& id)
{
	std::stringstream query;

	query << "SELECT id FROM atm.account_holder WHERE id =" << id;
	int state = mysql_query(connection, query.str().c_str());

	res = mysql_store_result(connection);
	row = mysql_fetch_row(res);
	if (row != NULL) {
		return std::stoi(row[0]);
	}
	return 0;

}

int validateCreditCard(MYSQL*& connection, MYSQL_RES*& res, MYSQL_ROW& row, std::string& creditCard)
{
	std::string query = "SELECT * FROM atm.credit_card WHERE card_number='";
	query += creditCard;
	query += "'";

	int state = mysql_query(connection, query.c_str());

	res = mysql_store_result(connection);
	row = mysql_fetch_row(res);

	if (row == NULL) {
		return 0;
	}
	// checking credit card pincode

	std::cout << "Please Enter Your PIN Code \n\n";
	std::string pin;
	std::getline(std::cin, pin);

	if (pin.compare(row[1])) {
		return 0;
	}
	else {
		return std::stoi(row[2]);
	}

}

void findAllDeposits(MYSQL*& connection, MYSQL_RES*& res, int bankAccount)
{
	std::stringstream query;

	query << "SELECT amount, account_type_id, created FROM atm.transaction where `from`= " << bankAccount
		<< " and `type`= 0";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);

}

void findAllWithdraws(MYSQL *& connection, MYSQL_RES *& res, int bankAccount)
{
	std::stringstream query;

	query << "SELECT amount, account_type_id, created FROM atm.transaction where `from`= " << bankAccount
		<< " and `type`= 1";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);
}

void findAllTransfers(MYSQL*& connection, MYSQL_RES*& res, int bankAccount)
{
	std::stringstream query;

	query << "SELECT `to`, amount, account_type_id, created FROM atm.transaction Where `from` = " 
		  << bankAccount << " AND `type` = 2";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);
}

void findAllTransactions(MYSQL *& connection, MYSQL_RES *& res, int bankAccount)
{
	std::stringstream query;

	query << "SELECT `to`, amount, account_type_id, created FROM atm.transaction Where `from` = "
		  << bankAccount;

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);
}

