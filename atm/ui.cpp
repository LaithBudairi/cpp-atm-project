#include "ui.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

void displayOperations() {
	std::cout << "Please Select an Operation: \n\n";

	std::cout << "1. Deposit To Your Account.\n";
	std::cout << "2. Withdraw From Your Account.\n";
	std::cout << "3. Transfer To Another Account.\n";
	std::cout << "4. View Transactions.\n";
	std::cout << "5. Exit System.\n\n";
}

void displayCurrency() {
	std::cout << "1. ILS\n";
	std::cout << "2. USD\n";
	std::cout << "3. JOR\n";
}

void displayTransactions() {
	std::cout << "Please Select an Operation: \n\n";

	std::cout << "1. View All Deposits.\n";
	std::cout << "2. View All Withdraws.\n";
	std::cout << "3. View All Transfers.\n";
	std::cout << "4. View All Transactions.\n";
}

void depositProcedure(MYSQL*& connection, std::unique_ptr<Transaction>& transaction, int bankAccount) {
	auto logger = spdlog::get("atm-logger");
	transaction = TransactionFactory::createTransaction(TransactionType::DEPOSIT);

	logger->info("Deposit Operation");

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

void withdrawProcedure(MYSQL*& connection, std::unique_ptr<Transaction>& transaction, int bankAccount) {
	auto logger = spdlog::get("atm-logger");
	transaction = TransactionFactory::createTransaction(TransactionType::WITHDRAW);

	logger->info("Withdraw Operation");

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

	std::cout << "Withdraw Complete\n\n";
}

void transferProcedure(MYSQL *& connection, std::unique_ptr<Transaction>& transaction, int bankAccount) {
	auto logger = spdlog::get("atm-logger");
	transaction = TransactionFactory::createTransaction(TransactionType::TRANSFER);

	logger->info("Transfer Operation");

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

void displayAllDeposits(MYSQL*& connection, MYSQL_RES*& res, MYSQL_ROW& row, int bankAccount) {

	findAllDeposits(connection, res, bankAccount);
	if (res->row_count == 0) {
		std::cout << "\nNo Deposits Found.\n\n";
		return;
	}

	std::cout << std::setfill('-') << std::setw(47) << "\n";
	std::cout << "|        Amount         |        Date        |\n";
	std::cout << std::setfill('-') << std::setw(47) << "\n";

	while (row = mysql_fetch_row(res)) {
		printf("|");
		std::string currencyOut;
		const char* currency = row[1];

		if (currency[0] == '1') {
			currencyOut = "ILS";
		}
		else if (currency[0] == '2') {
			currencyOut = "USD";
		}
		else if (currency[0] == '3') {
			currencyOut = "JOR";
		}


		printf(" %-18s %s|", row[0], currencyOut);
		printf(" %s| \n", row[2]);
		std::cout << std::setfill('-') << std::setw(47) << "\n";
	}
}

void displayAllWithdraws(MYSQL *& connection, MYSQL_RES*& res, MYSQL_ROW & row, int bankAccount)
{
	findAllWithdraws(connection, res, bankAccount);
	if (res->row_count == 0) {
		std::cout << "\nNo Withdraws Found.\n\n";
		return;
	}

	std::cout << std::setfill('-') << std::setw(48) << "\n";
	std::cout << "|        Amount         |         Date        |\n";
	std::cout << std::setfill('-') << std::setw(48) << "\n";

	while (row = mysql_fetch_row(res)) {
		printf("|");
		std::string currencyOut;
		const char* currency = row[1];

		if (currency[0] == '1') {
			currencyOut = "ILS";
		}
		else if (currency[0] == '2') {
			currencyOut = "USD";
		}
		else if (currency[0] == '3') {
			currencyOut = "JOR";
		}

		printf(" %-18s %s|", row[0], currencyOut);
		printf(" %s |\n", row[2]);
		std::cout << std::setfill('-') << std::setw(48) << "\n";
	}
}

void displayAllTransfers(MYSQL *& connection, MYSQL_RES *& res, MYSQL_ROW & row, int bankAccount)
{
	findAllTransfers(connection, res, bankAccount);
	if (res->row_count == 0) {
		std::cout << "\nNo Transfers Found.\n\n";
		return;
	}

	std::cout << std::setfill('-') << std::setw(78) << "\n";
	std::cout << "|        Transfered To        |         Amount         |      Created       |\n";
	std::cout << std::setfill('-') << std::setw(78) << "\n";

	while (row = mysql_fetch_row(res)) {
		printf("|");
		std::string currencyOut;
		const char* currency = row[2];

		if (currency[0] == '1') {
			currencyOut = "ILS";
		}
		else if (currency[0] == '2') {
			currencyOut = "USD";
		}
		else if (currency[0] == '3') {
			currencyOut = "JOR";
		}

		
		printf(" %-28s|", row[0]);
		printf(" %-19s %s|", row[1], currencyOut);
		printf(" %s|\n", row[3]);
		std::cout << std::setfill('-') << std::setw(78) << "\n";
	}
}

void displayAllTransactions(MYSQL *& connection, MYSQL_RES *& res, MYSQL_ROW & row, int bankAccount)
{
	findAllTransactions(connection, res, bankAccount);

	if (res->row_count == 0) {
		std::cout << "\nNo Transaction Found.\n\n";
		return;
	}

	std::cout << std::setfill('-') << std::setw(78) << "\n";
	std::cout << "|        Transfered To        |         Amount         |       Created      |\n";
	std::cout << std::setfill('-') << std::setw(78) << "\n";

	while (row = mysql_fetch_row(res)) {
		printf("|");
		std::string currencyOut;
		const char* currency = row[2];

		if (currency[0] == '1') {
			currencyOut = "ILS";
		}
		else if (currency[0] == '2') {
			currencyOut = "USD";
		}
		else if (currency[0] == '3') {
			currencyOut = "JOR";
		}


		printf(" %-28s|", row[0]);
		printf(" %-19s %s|", row[1], currencyOut);
		printf(" %s|\n", row[3]);
		std::cout << std::setfill('-') << std::setw(78) << "\n";
	}
}
