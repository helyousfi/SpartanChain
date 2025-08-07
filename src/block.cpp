#include "block.hpp"
#include "crypto.hpp"
#include <sstream>
#include <chrono>
#include <iomanip>
#include <iostream>

// Block Constructor
Block::Block(int idx, const std::string& prevHash, const std::vector<Transaction>& trx) : 
	index(idx), previousHash(prevHash), transactions(trx), nonce(0)
{
	// Calculate the timestamp (when the block was added to the blockchain)
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
	timestamp = ss.str();
}


// Calculate the hash from index, previousHash and timestamp
std::string Block::calculateHash() const{
	std::stringstream ss;
	ss << index << previousHash << timestamp << nonce;
	for(const auto& tx:transactions)
	{
		ss << tx.getHash();
	}
	return Crypto::sha256(ss.str());
}

// mining, finding the nonce until hash starts with difficulty "0"
void Block::mine(int difficulty)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::string prefix(difficulty, '0'); // "0000" if difficulty = 4
	while(hash.substr(0, difficulty) != prefix)
	{
		nonce++;
		hash = calculateHash();
		if(nonce % 1000000 == 0)
			std::cout << "[DEBUG_BLOCK_" + hash + "] Mining... nonce : " << nonce << std::endl;
	}
	auto end = std::chrono::high_resolution_clock::now();
	miningDuration = std::chrono::duration<double>(end - start).count();
	std::cout << "Block mined : " << hash << " in " << miningDuration << " seconds." << std::endl; 
}

std::string Block::toString() const noexcept {
	std::stringstream ss;
	ss << "Block " << index << " [Hash: " << hash << "]\n"
	   << "Prev: " << previousHash << "\n"
	   << "Time: " << timestamp << "\n"
	   << "Nonce: " << nonce << "\n"
	   << "Transactions: " << transactions.size() << "\n";
	return ss.str();
}

bool Block::isLinkedTo(const Block& previousBlock) const {
    return previousHash == previousBlock.hash && index == previousBlock.index + 1;
}

// === Getters ===
int Block::getIndex() const noexcept { return index; }
const std::string& Block::getPreviousHash() const { return previousHash; }
const std::string& Block::getHash() const noexcept { return hash; }
const std::vector<Transaction>& Block::getTransactions() const noexcept { return transactions;}
const std::string& Block::getTimestamp() const noexcept { return timestamp;}
std::time_t Block::getRawTimestamp() const noexcept { return rawTimestamp; }
int Block::getNonce() const noexcept { return nonce; }
double Block::getMiningDuration() const noexcept { return miningDuration; }
