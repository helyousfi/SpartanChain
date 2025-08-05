#include "transaction.hpp"
#include "constants.hpp"
#include "crypto.hpp"
#include <iostream>

Transaction::Transaction(std::string fromAdr, std::string toAdr, int amnt) : 
	from(std::move(fromAdr)), to(std::move(toAdr)), amount(amnt), signature("") {}

void Transaction::signTransaction(EVP_PKEY* privateKey)
{
	if(from == SYSTEM_ADDRESS) return; // no signature for mining rewards
	std::string data = from + to + std::to_string(amount);
	signature = Crypto::sign(data, privateKey);
}
std::string Transaction::calculateHash() const{
	std::stringsteam ss;
	ss << from << to << amount << signature << timestamp;
	return Crypto::sha256(ss.str());
}
bool Transaction::isValid() const {
	if(from == SYSTEM_ADDRESS) return true;
	if(signature.empty())
	{
		std::cerr << "Transaction missing signature!" << std::endl;
		return false;
	}

	EVP_PKEY* pubKey = Crypto::loadpublickey(from);
	if(!pubKey)
	{
		std::cerr << "Invalid public key" << std::endl;
		return false;
	}
	std::string data = from + to + std::to_string(amount);
	bool result = Crypto::verify(data, signature, pubKey);
	EVP_PKEY_free(pubKey);
	return result;
}
