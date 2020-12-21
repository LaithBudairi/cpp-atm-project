#ifndef ATM_TRANSACTION
#define  ATM_TRANSACTION

#include "mysql.h"

class Transaction {

protected:
	int type = -1;
	int amountType;
	float amount;
	int from = 0;
	int to = 0;
	//time_t result;


public:

	Transaction();

	int getTranType();
	void setTranType(int);

	int getAmountType();
	void setAmountType(int);

	float getAmount();
	void setAmount(float);

	int getFrom();
	void setFrom(int);

	int getTo();
	void setTo(int);

	virtual void commit(MYSQL*&) = 0;

	/*time_t getCreatedTimestamp();*/

};

class Deposit : public Transaction {
	void commit(MYSQL*&);
};

class Withdraw : public Transaction {
	void commit(MYSQL*&);
};

class Transfer : public Transaction {
	void commit(MYSQL*&);
};

enum TransactionType {
	DEPOSIT, WITHDRAW, TRANSFER
};

class TransactionFactory {

public:
	static std::unique_ptr<Transaction> createTransaction(TransactionType);
};

#endif