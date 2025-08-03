#pragma once

#include "block.hpp"
#include "transaction.hpp"
#include <vector>
#include <string> 

class Blockchain{
	private:
		std::vector<Block> chain;
		int difficulty;
		std::vector<Transaction> pendingTransactions;
		int reward;
		int miningReward;

	public:
		Blockchain();
		void addTransaction(const Transaction& tsx);
		void minePendingTransaction(const std::string& minerAddress);
		bool isChainValid() const;
		int getBalanceOf(const std::string& address) const;
		void printChain() const;
};
