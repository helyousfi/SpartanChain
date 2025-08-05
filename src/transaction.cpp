#include "transaction.hpp"
#include "crypto.hpp"
#include <iostream>

Transaction::Transaction(std::string fromAdr, std::string toAdr, int amnt) : 
	from(std::move(fromAdr)), to(std::move(toAdr)), amount(amnt), signature("") {}

void Transaction::signTransaction(EVP_PKEY* privateKey)
{
	if(from == "SYSTEM") return; // no signature for mining rewards
	std::string data = from + to + std::to_string(amount);
	signature = Crypto::sign(data, privateKey);
}

bool Transaction::isValid() const {
	if(from == "SYSTEM") return true;
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
	return Crypto::verify(data, signature, pubKey);
	
}
