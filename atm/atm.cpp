// atm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mysql.h>
#include <string>
#include "MySQLDB.h"
#include "TransactionQueries.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

static int newCounter = 0;

void displayOperations();
void displayCurrency();

void * operator new(size_t size)
{
	//std::cout << "New operator overloading \n\n";
	void * p = malloc(size);
	newCounter++;
	return p;
}

int main() {

	auto max_size = 1048576 * 5;
	auto max_files = 3;
	spdlog::flush_every(std::chrono::seconds(3));

	spdlog::set_level(spdlog::level::debug);
	auto logger =  spdlog::rotating_logger_mt("atm-logger", "C:\\logs\\atm\\logs.txt", max_size, max_files);


	MYSQL* connection;
	MYSQL_RES *res;
	MYSQL_ROW row;

	const char* host = "localhost";
	const char* username = "root";
	const char* password = "root";
	const char* dbName = "atm";
	unsigned int port = 3306;

	connection = connect(host, username, password, dbName, port);


    std::cout << "Please Enter Your Credit Card Number \n\n";
	std::string creditCard;

	std::getline(std::cin, creditCard);

	// validate credit card info
	int bankAccount = validateCreditCard(connection, res, row, creditCard);
	if(bankAccount) {
		logger->info("Valid Credit Card Credentials...");
	}
	else {
		std::cout << "Invalid Credit Card Credentials. Exiting...";
		return 0;
	}

	mysql_free_result(res);


	int op;

	// atm actions
	do {

		displayOperations();
		std::cin >> op;

	} while (false);

	Transaction* transaction;

	if (op == 1) { // deposit
		logger->info("Deposit Operation");
		transaction = TransactionFactory::createTransaction(TransactionType::DEPOSIT);		

		transaction->setFrom(bankAccount);
		transaction->setTo(bankAccount);

		std::cout << "Please Select The Currency You Wish To Deposit\n\n";
		displayCurrency();

		int amountType;
		std::cin >> amountType;
		transaction->setAmountType(amountType);

		std::cout << "Please Type The Amount to Deposit\n\n";

		float amount;
		std::cin >> amount;

		transaction->setAmount(amount);
		transaction->commit(connection);

		std::cout << "Deposit Complete\n\n";
	}
	else if(op == 2){ // withdraw
		logger->info("Withdraw Operation");
		transaction = TransactionFactory::createTransaction(TransactionType::WITHDRAW);

		transaction->setFrom(bankAccount);

		std::cout << "Please Select The Currency You Wish To Withdraw\n\n";
		displayCurrency();

		int amountType;
		std::cin >> amountType;
		transaction->setAmountType(amountType);

		std::cout << "Please Type The Amount To Withdraw\n\n";

		float amount;
		std::cin >> amount;

		transaction->setAmount(amount);
		transaction->commit(connection);
	}
	else if(op == 3) { // transfer
		logger->info("Transfer Operation");
		transaction = TransactionFactory::createTransaction(TransactionType::TRANSFER);

		transaction->setFrom(bankAccount);

		std::cout << "Please Type The Account Number You Wish To Transfer To\n\n";

		int to;
		std::cin >> to;

		transaction->setTo(to);


		std::cout << "Please Select The Currency You Wish To Transfer\n\n";
		displayCurrency();

		int amountType;
		std::cin >> amountType;
		transaction->setAmountType(amountType);

		std::cout << "Please Type The Amount To Transfer\n\n";

		float amount;
		std::cin >> amount;

		transaction->setAmount(amount);

		transaction->commit(connection);


		std::cout << "Transaction Complete...\n\n";

	}

	std::cin.get();

	std::cout << "Total New Allocations: " << newCounter;
	logger->info("Exiting Application");
	return 0;
}


void displayOperations() {
	std::cout << "Please select an operation: \n\n";

	std::cout << "1. Deposit To Your Account.\n";
	std::cout << "2. Withdraw From Your Account.\n";
	std::cout << "3. Transfer To Another Account.\n";
	std::cout << "4. View Transactions.\n";
	std::cout << "5. Exit System.\n\n";
}

void displayCurrency()
{
	std::cout << "1. ILS\n";
	std::cout << "2. USD\n";
	std::cout << "3. JOR\n";
}