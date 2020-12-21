// atm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mysql.h>
#include <string>
#include <processthreadsapi.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "MySQLDB.h"
#include "TransactionQueries.h"
#include "ui.h"

static int newCounter = 0;

void * operator new(size_t size) {
	void * p = malloc(size);
	newCounter++;
	return p;
}

int main() {

	bool isOnMainMenu = true;

	auto max_size = 1048576 * 5;
	auto max_files = 3;
	auto logger =  spdlog::rotating_logger_mt("atm-logger", "C:\\logs\\atm\\logs.txt", max_size, max_files);
	spdlog::flush_every(std::chrono::seconds(3));
	spdlog::set_level(spdlog::level::debug);

	logger->info("Starting Application With PID: " + std::to_string(GetCurrentProcessId()));

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
		logger->error("Invalid Credit Card Credentials...");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return 0;

	}
	mysql_free_result(res);
	
	int op;
	// atm actions
	do {
		displayOperations();
		std::cin >> op;

		std::unique_ptr<Transaction> transaction;

		if (op == 1) { // deposit
			depositProcedure(connection, transaction, bankAccount);
		}
		else if(op == 2){ // withdraw
			withdrawProcedure(connection, transaction, bankAccount);
		}
		else if(op == 3) { // transfer
			transferProcedure(connection, transaction, bankAccount);
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
			mysql_free_result(res);
		}
		else if(op == 5) {
			isOnMainMenu = false;
		}
	} while (isOnMainMenu);

	std::cout << "\nExiting...\n";
	logger->info("Exiting Application..");

	mysql_close(connection);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	return 0;
}