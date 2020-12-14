#include <iostream>
#include <string>
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
	createTransaction(connection, this);
	updateBalance(connection, this);

}

void Withdraw::commit(MYSQL*& connection)
{
	int state = updateBalance(connection, this);
	if (!state) {
		createTransaction(connection, this);
		std::cout << "Withdraw Complete...\n\n";
	}
	else {
		std::cout << "Error: Not Enough Balance, Exiting...\n\n";

	}
}

void Transfer::commit(MYSQL*& connection)
{
	std::cout << "transfer";
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
