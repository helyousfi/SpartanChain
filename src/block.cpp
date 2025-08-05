#include "block.hpp"
#include "crypto.hpp"
#include <sstream>
#include <chrono>
#include <iomanip>
#include <iostream>

// constructor
Block::Block(int idx, const std::string& prevHash, const std::vector<Transaction>& trx) : 
	index(idx), previousHash(prevHash), transactions(trx), nonce(0)
{
	// Calculate the timestamp
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
	timestamp = ss.str();
}


std::string Block::calculateHash() const{
	std::stringstream ss;
	ss << index << previousHash << timestamp << nonce;
	for(const auto& tx:transactions)
	{
		ss << tx.getHash();
	}
	return Crypto::sha256(ss.str());
}

void Block::mine(int difficulty)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::string prefix(difficulty, '0'); // "0000" if difficulty = 4
	while(hash.substr(0, difficulty) != prefix)
	{
		nonce++;
		hash = calculateHash();
	}
	auto end = std::chrono::high_resolution_clock::now();
	miningDuration = std::chrono::duration<double>(end - start).count();
	std::cout << "Block mined : " << hash << " in " << miningDuration << " seconds." << std::endl; 
}

std::string Block::toString() const {
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
int Block::getIndex() const { return index; }
const std::string& Block::getPreviousHash() const { return previousHash; }
const std::string& Block::getHash() const { return hash; }
const std::vector<Transaction>& Block::getTransactions() const { return transactions;}
const std::string& Block::getTimestamp() const { return timestamp;}
std::time_t Block::getRawTimestamp() const { return rawTimestamp; }
int Block::getNonce() const { return nonce; }
double Block::getMiningDuration() const { return miningDuration; }
