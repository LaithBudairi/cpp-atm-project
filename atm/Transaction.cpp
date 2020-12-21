#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "TransactionQueries.h"
#include "Transaction.h"
#include <memory>

Transaction::Transaction() {

}

int Transaction::getTranType() {
	return type;
}

void Transaction::setTranType(int Ttype) {
	type = Ttype;
}

int Transaction::getAmountType() {
	return amountType;
}

void Transaction::setAmountType(int amountType) {
	this->amountType = amountType;
}

float Transaction::getAmount() {
	return amount;
}

void Transaction::setAmount(float amount) {
	this->amount = amount;
}

int Transaction::getFrom() {
	return from;
}

void Transaction::setFrom(int from) {
	this->from = from;
}

int Transaction::getTo() {
	return to;
}

void Transaction::setTo(int to) {
	this->to = to;
}

// subclasses

void Deposit::commit(MYSQL*& connection) {
	auto logger = spdlog::get("atm-logger");
	int state = updateBalance(connection, this); 

	logger->info("Deposit Successful");

	createTransaction(connection, this);

	logger->info("Transaction Created");
}

void Withdraw::commit(MYSQL*& connection) {
	auto logger = spdlog::get("atm-logger");
	int state = updateBalance(connection, this);

	if (!state) {
		createTransaction(connection, this);
		logger->info("Withdraw Successful");
		logger->info("Transaction Created");
	}
	else {
		std::cout << "Error: Not Enough Balance.\n\n";
		logger->warn("Withdraw Unsuccessful. Not Enough Balance");
	}
}

void Transfer::commit(MYSQL*& connection) {
	MYSQL_RES* res;
	MYSQL_ROW row;

	int id = findAccountHolderWithId(connection, res, row, to);
	auto logger = spdlog::get("atm-logger");

	if (!id) {
		std::cout << "Account Number Was Not Found...\n";
		logger->warn("Transfer Unsuccessful. Invalid Account Number");
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
			logger->warn("Transfer Unsuccessful. Not Enough Balance");
			std::cout << "Error: Not Enough Balance.\n\n";
		}
	}
}

// transaction factory

std::unique_ptr<Transaction> TransactionFactory::createTransaction(TransactionType type) {
	if (type == DEPOSIT) {
		std::unique_ptr<Deposit> deposit = std::make_unique<Deposit>();
		deposit->setTranType(DEPOSIT);

		return deposit;
	}
	else if (type == WITHDRAW) {
		std::unique_ptr<Withdraw> withdraw = std::make_unique<Withdraw>();
		withdraw->setTranType(WITHDRAW);

		return withdraw;
	}
	else {
		std::unique_ptr<Transfer> transfer = std::make_unique<Transfer>();
		transfer->setTranType(TRANSFER);

		return transfer;
	}
};