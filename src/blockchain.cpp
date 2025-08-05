#include <iostream>
#include "blockchain.hpp"
#include "constants.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Blockchain::Blockchain(int difficulty, int miningReward) : 
	difficulty(difficulty), miningReward(miningReward)
{
	Transaction genesisTx(SYSTEM_ADDRESS, NETWORK_ADDRESS, 0);
	std::vector<Transaction> genesisTxs = {genesisTx};
	Block genesisBlock(0, "0", genesisTxs);
	genesisBlock.mine(difficulty);
	chain.push_back(genesisBlock);
}

void Blockchain::addTransaction(const Transaction& tx)
{ 
	if(tx.from != SYSTEM_ADDRESS && !tx.isValid()) // No verification for mining rewards
	{
		std::cerr << "Invalid transaction. Rejected. \n";
		return;		
	}
	pendingTransactions.push_back(tx);
}

void Blockchain::addTransaction(Transaction&& tx)
{ 
	if(tx.from != SYSTEM_ADDRESS && !tx.isValid()) // No verification for mining rewards
	{
		std::cerr << "Invalid transaction. Rejected. \n";
		return;		
	}
	pendingTransactions.push_back(std::move(tx));
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

double Blockchain::getBalanceOf(const std::string& address) const {
	double balance = 0;
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
			return false;
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
			std::cout << "Transaction: " << tx.from << " -> " << tx.to
          					     << " | Amount: " << tx.amount << std::endl;
		}	
		std::cout << "---------------\n"; 
	}
}

const Block& Blockchain::getLatestBlock() const noexcept {
	return chain.back();
}

void Blockchain::reset() {
	chain.clear();
	pendingTransactions.clear();

	Transaction genesisTx(SYSTEM_ADDRESS, NETWORK_ADDRESS, 0);
	std::vector<Transaction> genesisTxs = {genesisTx};
	Block genesisBlock(0, "0", genesisTxs);
	genesisBlock.mine(difficulty);
	chain.push_back(genesisBlock);
}

std::string Blockchain::serialize() const {
    json j;
    j["difficulty"] = difficulty;
    j["miningReward"] = miningReward;
    j["chain"] = json::array();

    for (const auto& block : chain) {
        j["chain"].push_back(block.serialize()); // assume Block has serialize() returning json
    }

    j["pendingTransactions"] = json::array();
    for (const auto& tx : pendingTransactions) {
        j["pendingTransactions"].push_back(tx.serialize()); // assume Transaction has serialize() returning json
    }

    return j.dump();
}

Blockchain Blockchain::deserialize(const std::string& data) {
    auto j = json::parse(data);
    Blockchain bc(j["difficulty"], j["miningReward"]);

    bc.chain.clear();
    for (const auto& b : j["chain"]) {
        bc.chain.push_back(Block::deserialize(b)); // assume Block::deserialize(json)
    }

    bc.pendingTransactions.clear();
    for (const auto& tx : j["pendingTransactions"]) {
        bc.pendingTransactions.push_back(Transaction::deserialize(tx)); // assume Transaction::deserialize(json)
    }

    return bc;
}













