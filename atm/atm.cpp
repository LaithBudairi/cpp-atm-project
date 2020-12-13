// atm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mysql.h>
#include <string>
#include "MySQLDB.h"

void displayOperations();
int main()
{
	MYSQL* connection;
	MYSQL_RES *res;
	MYSQL_ROW row;

	const char* host = "localhost";
	const char* username = "root";
	const char* password = "root";
	const char* dbName = "atm";
	unsigned int port = 3306;

    std::cout << "Please Enter Your Credit Card Number \n\n";

	std::string creditCard;
	
	std::getline(std::cin, creditCard);

	// checking credit card number

	connection = connect(host, username, password, dbName, port);
	std::string query = "SELECT * FROM atm.credit_card WHERE card_number=";
	query += creditCard;

	int state = mysql_query(connection, query.c_str());

	res = mysql_store_result(connection);
	row = mysql_fetch_row(res);

	// checking credit card pincode

	std::cout << "Please Enter Your PIN Code \n\n";
	std::string pin;
	std::getline(std::cin, pin);

	std::cout << row[1];

	displayOperations();

	std::cin.get();
}


void displayOperations() {
	std::cout << "Please select an operation: \n\n";

	std::cout << "1. Deposit To Your Account.\n";
	std::cout << "2. Withdraw From Your Account.\n";
	std::cout << "3. Transfer To Another Account.\n";
	std::cout << "4. Exit System.\n\n";
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
