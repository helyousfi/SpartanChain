#include "transaction.hpp"
#include "constants.hpp"
#include "crypto.hpp"
#include <iostream>
#include <string_view>
#include <chrono>

Transaction::Transaction(std::string_view fromAdr, std::string_view toAdr, double amnt) : 
	from(std::move(fromAdr)), to(std::move(toAdr)), amount(amnt), signature("") 
{
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%d %H:%M:%S");

	timestamp = ss.str();
	hash = calculateHash();
}

void Transaction::signTransaction(EVP_PKEY* privateKey)
{
	if(from == SYSTEM_ADDRESS) return; // no signature for mining rewards
	signature = Crypto::sign(hash, privateKey);
}
std::string Transaction::calculateHash() const{
	std::stringstream ss;
	ss << from << to << amount << timestamp;
	return Crypto::sha256(ss.str());
}
bool Transaction::isValid() const {
	if(from == SYSTEM_ADDRESS) return true;
	if(signature.empty())
	{
		std::cerr << "Transaction missing signature!" << std::endl;
		return false;
	}

	EVP_PKEY* pubKey = Crypto::getPublicKeyFromString(from);
	if(!pubKey)
	{
		std::cerr << "Invalid public key" << std::endl;
		return false;
	}
	bool result = Crypto::verify(hash, signature, pubKey);
	EVP_PKEY_free(pubKey);
	return result;
}
std::string Transaction::toString() const noexcept
{
        std::stringstream ss;
	ss << "From "<< from << " to " << to << " : " << amount << std::endl;
	return ss.str();
}
