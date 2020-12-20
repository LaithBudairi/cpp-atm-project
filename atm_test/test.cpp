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