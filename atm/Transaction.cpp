#include "Transaction.h"
#include <iostream>

#include <time.h> 

Transaction::Transaction()
{

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

void Deposit::commit()
{
	//commit changes
}

void Withdraw::commit()
{
	//commit changes
}

void Transfer::commit()
{
	//commit changes
}
