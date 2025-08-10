#pragma once
#include "transaction.hpp"
#include <string>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>

class Wallet{
	public:
		EVP_PKEY* keypair;
		std::string address;

		Wallet();
		~Wallet();

		void save(const std::string& path) const;
	        static Wallet load(const std::string& path);
		
		std::string getAddress() const;
		Transaction createTransaction(const std::string& to, int amount); 
		std::string getPublicKeyString() const;
		EVP_PKEY* getPrivateKey() const;

		Wallet(const Wallet&) = delete;
		Wallet& operator=(const Wallet&) = delete;
		Wallet(Wallet&& other) noexcept;
		Wallet& operator=(Wallet&& other) noexcept;
};
