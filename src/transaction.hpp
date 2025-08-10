#pragma once
#include <string>
#include <string_view>
#include <openssl/evp.h>  
#include <openssl/sha.h>  
#include <iostream>
#include "transaction.hpp"

class Transaction{
	private:
		std::string from;
		std::string to;
		double amount;
		std::string signature;
		std::string timestamp; // time when transaction added
		std::string hash; 

	public:
		explicit Transaction(std::string_view fromAdr, std::string_view toAdr, double amnt);
		void signTransaction(EVP_PKEY* privateKey);
		[[nodiscard]] bool isValid() const;
		[[nodiscard]] std::string calculateHash() const;

		// getters 
		[[nodiscard]] std::string getFrom() const noexcept {return from;};
		[[nodiscard]] std::string getTo() const noexcept {return to; };
		[[nodiscard]] double getAmount() const noexcept {return amount; };
		[[nodiscard]] std::string getSignature() const noexcept {return signature; };
	       	[[nodiscard]] std::string getTimeStamp() const noexcept {return timestamp; };
		[[nodiscard]] std::string getHash() const noexcept {return hash;}	
		
		[[nodiscard]] std::string toString() const noexcept;
};
