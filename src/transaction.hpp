#pragma once
#include <string>
#include <string_view>
#include <openssl/evp.h>  
#include <openssl/sha.h>  
#include <iostream>
#include "transaction.hpp"

class Transaction{
	public:
		std::string from;
		std::string to;
		double amount;
		std::string signature;
		std::string timestamp;
		std::string hash; 

	// public:
		// TODO overload constructor
		Transaction(std::string_view fromAdr, std::string_view toAdr, double amnt);
		void signTransaction(EVP_PKEY* privateKey);
		bool isValid() const;
		std::string calculateHash() const;

		// getters 
		std::string getFrom() const {return from;};
		std::string getTo() const {return to; };
		double getAmount() const {return amount; };
		std::string getSignature() const {return signature; };
	       	std::string getTimeStamp() const {return timestamp; };
		std::string getHash() const {return hash;}	
		
		std::string toString() const;
};
