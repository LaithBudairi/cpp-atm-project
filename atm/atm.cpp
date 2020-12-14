// atm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mysql.h>
#include <string>
#include "MySQLDB.h"
#include "ui.h"
#include "Transaction.h"

static int newCounter = 0;

void displayOperations();
void displayCurrency();

void * operator new(size_t size)
{
	std::cout << "New operator overloading \n\n";
	void * p = malloc(size);
	newCounter++;
	return p;
}

int main() {

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
		std::cout << "valid info\n\n";
	}
	else {
		std::cout << "invalid info\n\n";
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
	bool isTran = false;

	if (op == 1) { // deposit
		transaction = TransactionFactory::createTransaction(TransactionType::DEPOSIT);		
		isTran = true;

		transaction->setFrom(bankAccount);
		transaction->setTo(bankAccount);

		std::cout << "Please Select The Currency You Wish To Deposit\n\n";
		displayCurrency();

		int amountType;
		std::cin >> amountType;
		std::cout << amountType;
		transaction->setAmountType(amountType);

		std::cout << "Please Type The Amount to Deposit\n\n";

		float amount;
		std::cin >> amount;
		std::cout << amount;

		transaction->setAmount(amount);
		transaction->commit(connection);
		

		std::cout << "Deposit Complete\n\n";
	}
	else if(op == 2){ // withdraw
		transaction = TransactionFactory::createTransaction(TransactionType::WITHDRAW);
		isTran = true;

		transaction->setFrom(bankAccount);

		std::cout << "Please Select The Currency You Wish To Withdraw\n\n";
		displayCurrency();

		int amountType;
		std::cin >> amountType;
		std::cout << amountType;
		transaction->setAmountType(amountType);

		std::cout << "Please Type The Amount to Withdraw\n\n";

		float amount;
		std::cin >> amount;
		std::cout << amount;

		transaction->setAmount(amount);
		transaction->commit(connection);
	}
	else if(op == 3) { // transfer
		transaction = TransactionFactory::createTransaction(TransactionType::TRANSFER);
		isTran = true;

		std::cout << "Transaction Complete...\n\n";

	}

	std::cin.get();

	std::cout << "Total New Allocations: " << newCounter;
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
