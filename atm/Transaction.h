#pragma once


class Transaction {

private:
	int amountType;
	float amount;
	int from;
	int to;
	//time_t result;

protected:
	virtual void commit() = 0;

public:

	Transaction();

	int getAmountType();
	void setAmountType(int);

	float getAmount();
	void setAmount(float);

	int getFrom();
	void setFrom(int);

	int getTo();
	void setTo(int);

	/*time_t getCreatedTimestamp();*/

};

class Deposit : public Transaction {
	void commit();
};

class Withdraw : public Transaction {
	void commit();
};

class Transfer : public Transaction {
	void commit();
};
