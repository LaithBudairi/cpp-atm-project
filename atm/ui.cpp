#include "ui.h"
#include <mysql.h>
#include <string>
#include <sstream>

int validateCreditCard(MYSQL*& connection, MYSQL_RES*& res, MYSQL_ROW& row, std::string& creditCard)
{
	std::string query = "SELECT * FROM atm.credit_card WHERE card_number='";
	query += creditCard;
	query += "'";

	int state = mysql_query(connection, query.c_str());

	res = mysql_store_result(connection);
	row = mysql_fetch_row(res);

	if (row == NULL) {
		std::cout << "Invalid Card Number\n\n";
		return 0;
	}

	// checking credit card pincode

	std::cout << "Please Enter Your PIN Code \n\n";
	std::string pin;
	std::getline(std::cin, pin);

	if (pin.compare(row[1])) {
		std::cout << "Invalid PIN Code, Aborting...\n";
		return 0;
	}
	else {
		return std::stoi(row[2]);
	}

	//mysql_free_result;

}