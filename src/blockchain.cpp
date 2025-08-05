#include <iostream>
#include "blockchain.hpp"
#include "constants.hpp"

Blockchain::Blockchain()
{
	difficulty = 4;
	miningReward = 5;

	Transaction genesisTx(SYSTEM_ADDRESS, NETWORK_ADDRESS, 0);
	std::vector<Transaction> genesisTxs = {genesisTx};
	Block genesisBlock(0, "0", genesisTxs);
	genesisBlock.mine(difficulty);
	chain.push_back(genesisBlock);
       	
}

void Blockchain::addTransaction(const Transaction& tx)
{
	if(!tx.isValid())
	{
		std::cerr << "Invalid transaction. Rejected. \n";
		return;		
	}
	pendingTransactions.push_back(tx);
}

void Blockchain::minePendingTransaction(const std::string& minerAddress)
{
	Transaction rewardTx(SYSTEM_ADDRESS, minerAddress, miningReward);
	pendingTransactions.push_back(rewardTx);

	Block newBlock(chain.size(), chain.back().hash, pendingTransactions);
	newBlock.mine(difficulty);

	chain.push_back(newBlock);
	pendingTransactions.clear();
}

int Blockchain::getBalanceOf(const std::string& address) const {
	int balance = 0;
	for(const auto& block : chain) {
		for(const auto& tx : block.transactions)
		{
			if(tx.from == address)
				balance -= tx.amount;
			if(tx.to == address)
				balance += tx.amount;
		}
	}
	return balance;
}


bool Blockchain::isChainValid() const {
	for(size_t i = 1; i < chain.size(); ++i)
	{
		const Block& current = chain[i];
		const Block& previous = chain[i-1];

		if(current.hash != current.calculateHash())
		{
			std::cerr << "Block " << i << " hash mismatch! \n";
			return false;
		}
		if(current.previousHash != previous.hash)
		{
			std::cerr << "Block " << i << " previous hash mismatch. \n";
		}
		for(const auto& tx : current.transactions)
		{
			if(!tx.isValid())
			{
				std::cerr << "Invalid transaction " << i << std::endl;
				return false;
			}
		}
	}
	return true;
}

void Blockchain::printChain() const {
	for(const auto& block : chain)
	{
		std::cout << "Block #" << block.index << std::endl; 
		std::cout << "Hash " << block.hash << std::endl;
		std::cout << "Prev " << block.previousHash << std::endl;
		std::cout << "Time " << block.timestamp << std::endl;
		std::cout << "Nonce " << block.nonce << std::endl;
	       	std::cout << "Transactions : " << std::endl;
		for(const auto& tx : block.transactions)
		{
			std::cout << tx.from << " -> " << tx.to << " : " << tx.amount << std::endl;
		}	
		std::cout << "---------------\n"; 
	}
}















