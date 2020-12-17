#include "ui.h"

void displayOperations() {
	std::cout << "Please Select an Operation: \n\n";

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

void displayTransactions() {
	std::cout << "Please Select an Operation: \n\n";

	std::cout << "1. View All Deposits.\n";
	std::cout << "2. View All Withdraws.\n";
	std::cout << "3. View All Transfers.\n";
	std::cout << "4. View All Transactions.\n";

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

	//std::cout << std::setfill('-') << std::setw(90) << "\n";
	//std::cout << "|From            |To                |Amount                |Date                        |\n";
	//std::cout << std::setfill('-') << std::setw(90) << "\n";

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
