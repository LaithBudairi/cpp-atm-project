#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <time.h> 
#include "TransactionQueries.h"
#include "Transaction.h"

Transaction::Transaction()
{

}

int Transaction::getTranType()
{
	return type;
}

void Transaction::setTranType(int Ttype)
{
	type = Ttype;
}


int Transaction::getAmountType()
{
	return amountType;
}

void Transaction::setAmountType(int amountType)
{
	this->amountType = amountType;
}

float Transaction::getAmount()
{
	return amount;
}

void Transaction::setAmount(float amount)
{
	this->amount = amount;
}

int Transaction::getFrom()
{
	return from;
}

void Transaction::setFrom(int from)
{
	this->from = from;
}

int Transaction::getTo()
{
	return to;
}

void Transaction::setTo(int to)
{
	this->to = to;
}

//
//time_t Transaction::getCreatedTimestamp()
//{
//	time_t my_time = time(NULL);
//
//	return ctime(&my_time);
//}


// subclasses


void Deposit::commit(MYSQL*& connection)
{
	auto logger = spdlog::get("atm-logger");

	int state = updateBalance(connection, this);
	logger->info("Deposit Successful");

	createTransaction(connection, this);
	logger->info("Transaction Created");

}

void Withdraw::commit(MYSQL*& connection)
{
	auto logger = spdlog::get("atm-logger");
	int state = updateBalance(connection, this);
	if (!state) {
		createTransaction(connection, this);
		logger->info("Withdraw Successful");

	}
	else {
		std::cout << "Error: Not Enough Balance, Exiting...\n\n";
		logger->info("Withdraw Unsuccessful");

	}
}

void Transfer::commit(MYSQL*& connection)
{
	MYSQL_RES* res;
	MYSQL_ROW row;

	int id = findAccountHolderWithId(connection, res, row, to);
	auto logger = spdlog::get("atm-logger");

	if (!id) {
		std::cout << "Account Number Was Not Found...\n";
		logger->info("Transfer Unsuccessful. Invalid Account Number");
	}
	else {
		int state = updateBalance(connection, this);

		if (!state) {
			createTransaction(connection, this);
			tranferTo(connection, this);
			std::cout << "Transfer Complete...\n\n";
			logger->info("Transfer Successful");

		}
		else {
			logger->info("Transfer Unsuccessful. Not Enough Balance");
			std::cout << "Error: Not Enough Balance, Exiting...\n\n";
		}
	}
}


// transaction factory

Transaction* TransactionFactory::createTransaction(TransactionType type)
{
	if (type == DEPOSIT) {
		Deposit deposit;
		deposit.setTranType(DEPOSIT);
		return &deposit;
	}
	else if (type == WITHDRAW) {
		Withdraw withdraw;
		withdraw.setTranType(WITHDRAW);
		return &withdraw;
	}
	else {
		Transfer transfer;
		transfer.setTranType(TRANSFER);
		return &transfer;
	}
};
