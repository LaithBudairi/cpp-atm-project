// atm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mysql.h>
#include <string>

#include "MySQLDB.h"
#include "TransactionQueries.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "ui.h"

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

	bool isOnMainMenu = true;

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
		std::cout << "Invalid Credit Card Credentials. Exiting...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return 0;

	}

	mysql_free_result(res);


	int op;

	// atm actions
	do {

		displayOperations();
		std::cin >> op;

		std::shared_ptr<Transaction> transaction;

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

		}
		else if(op == 4) { //transaction
			int tranOption;

			displayTransactions();

			std::cin >> tranOption;

			if (tranOption == 1) {
				displayAllDeposits(connection, res, row, bankAccount);
				logger->info("View Deposits");

			}
			else if (tranOption == 2) {
				displayAllWithdraws(connection, res, row, bankAccount);
				logger->info("View Withdraws");
			}
			else if (tranOption == 3) {
				displayAllTransfers(connection, res, row, bankAccount);
				logger->info("View Transfers");
			}
			else if (tranOption = 4) {
				displayAllTransactions(connection, res, row, bankAccount);
				logger->info("View Transactions");
			}

		}
		else if(op == 5) {
			isOnMainMenu = false;
		}

		//mysql_free_result(res);


	} while (isOnMainMenu);

	std::cout << "\nExiting...\n";
	logger->info("Exiting Application..");
	mysql_close(connection);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));


	return 0;
}