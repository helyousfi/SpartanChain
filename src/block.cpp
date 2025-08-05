#include "block.hpp"
#include "crypto.hpp"
#include <sstream>
#include <chrono>
#include <iomanip>

// constructor
Block::Block(int idx, std::string prevHash, std::vector<Transaction> trx) : 
	index(idx), previousHash(prevHash), transactions(trx), nonce(0)
{
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%d %H:%M:%S");

	timestamp = ss.str();
	hash = calculateHash();
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
	std::string prefix(difficulty, "0"); // "0000" if difficulty = 4
	while(hash.substr(0, difficulty) != prefix)
	{
		nonce++;
		hash = calculateHash();
	}
	std::cout << "Block mined : " << hash << std::endl; 
}


