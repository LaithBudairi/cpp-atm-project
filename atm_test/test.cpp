#include "pch.h"
#include "../atm/MySQLDB.cpp"
#include "../atm/TransactionQueries.h"

const char* host = "localhost";
const char* username = "root";
const char* password = "root";
const char* dbName = "atm";
unsigned int port = 3306;

TEST(DBConnection, ConnectionSuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	
	ASSERT_TRUE(connection != nullptr);
}

TEST(FindAccountHolderWithId, AccountQuerySuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int acc = 1;

	int bankAccount = findAccountHolderWithId(connection, res, row, acc);

	ASSERT_TRUE(bankAccount != 0);
}

TEST(FindTransaction, DepositQuerySuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int acc = 1;

	findAllDeposits(connection, res, 1);

	ASSERT_EQ(41, res->row_count);
}

TEST(FindTransaction, WithdrawQuerySuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int acc = 1;

	findAllWithdraws(connection, res, 1);

	ASSERT_EQ(3, res->row_count);
}

TEST(FindTransaction, TransferQuerySuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int acc = 1;

	findAllTransfers(connection, res, 1);

	ASSERT_EQ(3, res->row_count);
}

TEST(FindTransaction, TransactionQuerySuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int acc = 1;

	findAllTransactions(connection, res, 1);

	ASSERT_EQ(47, res->row_count);
}

TEST(UpdateBalance, WithdrawSuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	std::stringstream query;
	std::unique_ptr<Transaction> transaction = TransactionFactory::createTransaction(TransactionType::WITHDRAW);

	transaction->setFrom(1);
	transaction->setTo(0);
	transaction->setAmountType(1);
	transaction->setAmount(100);

	updateBalance(connection, transaction.get());

	query << "SELECT balance FROM atm.account_holder_has_account_type "
		 << "where account_holder_id = 1 and account_type_id = 1";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);

	row = mysql_fetch_row(res);

	std::cout << row[0];

	ASSERT_EQ(2519, std::stoi(row[0]));	

	transaction = TransactionFactory::createTransaction(TransactionType::DEPOSIT);

	transaction->setFrom(1);
	transaction->setTo(1);
	transaction->setAmountType(1);
	transaction->setAmount(100);
	updateBalance(connection, transaction.get());
}

TEST(UpdateBalance, DepositSuccessful) {
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	std::stringstream query;
	std::unique_ptr<Transaction> transaction = TransactionFactory::createTransaction(TransactionType::DEPOSIT);

	transaction->setFrom(1);
	transaction->setTo(1);
	transaction->setAmountType(1);
	transaction->setAmount(100);

	updateBalance(connection, transaction.get());

	query << "SELECT balance FROM atm.account_holder_has_account_type "
		<< "where account_holder_id = 1 and account_type_id = 1";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);

	row = mysql_fetch_row(res);

	std::cout << row[0];

	ASSERT_EQ(2719, std::stoi(row[0]));

	transaction = TransactionFactory::createTransaction(TransactionType::WITHDRAW);

	transaction->setFrom(1);
	transaction->setTo(0);
	transaction->setAmountType(1);
	transaction->setAmount(100);
	updateBalance(connection, transaction.get());
}

TEST(UpdateBalance,TransferSuccessful) {
	std::stringstream query;
	std::unique_ptr<Transaction> transaction;
	MYSQL *connection = connect(host, username, password, dbName, port);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int balanceBefore;
	int balanceAfter;
	// balance before transaction
	query << "SELECT balance FROM atm.account_holder_has_account_type "
		<< "where account_holder_id = 7 and account_type_id = 1";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);
	row = mysql_fetch_row(res);

	balanceBefore = std::stoi(row[0]);
	mysql_free_result(res);

	// transaction
	transaction = TransactionFactory::createTransaction(TransactionType::WITHDRAW);

	transaction->setFrom(1);
	transaction->setTo(0);
	transaction->setAmountType(1);
	transaction->setAmount(100);
	updateBalance(connection, transaction.get());

	transaction = TransactionFactory::createTransaction(TransactionType::DEPOSIT);

	transaction->setFrom(7);
	transaction->setTo(7);
	transaction->setAmountType(1);
	transaction->setAmount(100);
	updateBalance(connection, transaction.get());

	// balance after transaction
	query.str("");
	query.clear();

	query << "SELECT balance FROM atm.account_holder_has_account_type "
		<< "where account_holder_id = 7 and account_type_id = 1";

	mysql_query(connection, query.str().c_str());
	res = mysql_store_result(connection);
	row = mysql_fetch_row(res);

	balanceAfter = std::stoi(row[0]);
	mysql_free_result(res);

	ASSERT_EQ(100, balanceAfter - balanceBefore);
}