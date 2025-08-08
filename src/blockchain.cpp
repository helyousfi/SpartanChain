#include <iostream>
#include "blockchain.hpp"
#include "constants.hpp"
#include <nlohmann/json.hpp>
#include "transaction.hpp"

using json = nlohmann::json;

Blockchain::Blockchain(int difficulty, int miningReward) : 
	difficulty(difficulty), miningReward(miningReward)
{
	Transaction genesisTx(SYSTEM_ADDRESS, NETWORK_ADDRESS, 0.0);
	std::vector<Transaction> genesisTxs = {genesisTx};
	totalCoinsIssued += 0; // no reward after genesis transaction
	Block genesisBlock(0, "0", genesisTxs);
	genesisBlock.mine(difficulty);
	if(verboseLoggingEnabled)
	{
		std::cout << "[DEBUG] Genesis Block Mined!" << std::endl;
	}
	chain.push_back(std::move(genesisBlock));
}

void Blockchain::addTransaction(const Transaction& tx)
{ 
	if (tx.getAmount() <= 0) {
	    std::cerr << "[ERROR] Transaction amount must be positive. Rejected.\n";
	    return;
	}
	if(tx.getFrom() != SYSTEM_ADDRESS && !tx.isValid()) // No verification for mining rewards
	{
		std::cerr << "[ERROR] Invalid transaction. Rejected. \n";
		return;		
	}
	pendingTransactions.push_back(std::move(tx));
}

void Blockchain::addTransaction(Transaction&& tx)
{ 
	if (tx.getAmount() <= 0) {
	    std::cerr << "[DEBUG] Transaction amount must be positive. Rejected.\n";
	    return;
	}
	if(tx.getFrom() != SYSTEM_ADDRESS && !tx.isValid()) // No verification for mining rewards
	{
		std::cerr << "[ERROR] Invalid transaction. Rejected. \n";
		return;		
	}
	// Verify recipient address
	auto toAddr = tx.getTo();
	pendingTransactions.push_back(std::move(tx));
}

void Blockchain::minePendingTransaction(std::string_view minerAddress)
{
	if (pendingTransactions.empty()) {
        	if (verboseLoggingEnabled) {
            		std::cout << "[DEBUG] No pending transactions to mine.\n";
        	}
        	return;
    	}
	if (totalCoinsIssued + miningReward <= MAX_SUPPLY) {
        	Transaction rewardTx(SYSTEM_ADDRESS, minerAddress, miningReward);
        	pendingTransactions.push_back(rewardTx);
        	totalCoinsIssued += miningReward;
    	} else {
        	std::cout << "[INFO] Max coin supply reached. No mining reward issued.\n";
    	}
	if (verboseLoggingEnabled) {
        	std::cout << "[DEBUG] Creating new block with pending transactions...\n";
    	}
	Block newBlock(chain.size(), chain.back().getHash(), pendingTransactions);
	newBlock.mine(difficulty);
	if (verboseLoggingEnabled) {
        	std::cout << "[DEBUG] New block mined with hash: " << newBlock.getHash() << "\n";
    	}
	chain.push_back(std::move(newBlock));
	pendingTransactions.clear();
}

double Blockchain::getBalanceOf(std::string_view address) const {
	double balance = 0;
	for(const auto& block : chain) {
		for(const auto& tx : block.getTransactions())
		{
			if(tx.getFrom() == address)
				balance -= tx.getAmount();
			if(tx.getTo() == address)
				balance += tx.getAmount();
		}
	}
	return balance;
}


bool Blockchain::isChainValid() const {
	for(size_t i = 1; i < chain.size(); ++i)
	{
		const Block& current = chain[i];
		const Block& previous = chain[i-1];

		if(current.getHash() != current.calculateHash())
		{
			std::cerr << "Block " << i << " hash mismatch! \n";
			return false;
		}
		if(current.getPreviousHash() != previous.getHash())
		{
			std::cerr << "Block " << i << " previous hash mismatch. \n";
			return false;
		}
		for(const auto& tx : current.getTransactions())
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

void Blockchain::printChain() const noexcept {
	for(const auto& block : chain)
	{
		std::cout << "Block #" << block.getIndex() << std::endl; 
		std::cout << "Hash " << block.getHash() << std::endl;
		std::cout << "Prev " << block.getPreviousHash() << std::endl;
		std::cout << "Time " << block.getTimestamp() << std::endl;
		std::cout << "Nonce " << block.getNonce() << std::endl;
	       	std::cout << "Transactions : " << std::endl;
		for(const auto& tx : block.getTransactions())
		{
			std::cout << "Transaction: " << tx.getFrom() << " -> " << tx.getTo()
          					     << " | Amount: " << tx.getAmount() << std::endl;
		}
		std::cout << "Total Coins Issued: " << totalCoinsIssued << "/" << MAX_SUPPLY << "\n";
		std::cout << "---------------\n"; 
	}
}

const Block& Blockchain::getLatestBlock() const noexcept {
	return chain.back();
}

void Blockchain::reset() noexcept {
	chain.clear();
	pendingTransactions.clear();

	Transaction genesisTx(SYSTEM_ADDRESS, NETWORK_ADDRESS, 0);
	std::vector<Transaction> genesisTxs = {genesisTx};
	Block genesisBlock(0, "0", genesisTxs);
	genesisBlock.mine(difficulty);
	chain.push_back(std::move(genesisBlock));
}

std::string Blockchain::serialize() const {
    json j;
    j["difficulty"] = difficulty;
    j["miningReward"] = miningReward;
    j["chain"] = json::array();

    for (const auto& block : chain) {
        // j["chain"].push_back(block.serialize()); // TODO add serialize() to block
    }

    j["pendingTransactions"] = json::array();
    for (const auto& tx : pendingTransactions) {
        // j["pendingTransactions"].push_back(tx.serialize()); // assume Transaction has serialize() returning json
    }

    return j.dump();
}

Blockchain Blockchain::deserialize(const std::string& data) {
    auto j = json::parse(data);
    Blockchain bc(j["difficulty"], j["miningReward"]);

    bc.chain.clear();
    for (const auto& b : j["chain"]) {
        // bc.chain.push_back(Block::deserialize(b)); // assume Block::deserialize(json)
    }

    bc.pendingTransactions.clear();
    for (const auto& tx : j["pendingTransactions"]) {
        // bc.pendingTransactions.push_back(Transaction::deserialize(tx)); // assume Transaction::deserialize(json)
    }

    return bc;
}

bool Blockchain::isVerboseLoggingEnabled() const noexcept {
    return verboseLoggingEnabled;
}

void Blockchain::enableVerboseLogging(bool enable) noexcept {
    verboseLoggingEnabled = enable;
}













