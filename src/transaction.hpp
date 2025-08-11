#pragma once
#include <string>
#include <string_view>
#include <openssl/evp.h>  
#include <openssl/sha.h>  
#include <iostream>
#include "transaction.hpp"

/**
 * @class Transaction
 * @brief Represents a monetary transfer between two blockchain addresses.
 *
 * A Transaction records the sender address, recipient address, amount,
 * timestamp, and cryptographic signature. Transactions are hashed to
 * ensure immutability, and can be signed and validated using public-key
 * cryptography.
 *
 * This class provides:
 * - Secure transaction signing with a private key.
 * - Validation to ensure data integrity and authenticity.
 * - Hash calculation for use in block mining and chain verification.
 * - Read-only access to all transaction details.
 *
 * Transactions are intended to be immutable after creation and signing.
 * Any modification would invalidate the signature and hash.
 */
class Transaction{
	private:
		std::string from;
		std::string to;
		double amount;
		std::string signature;
		std::string timestamp; // time when transaction added
		std::string hash; 

	public:
		/**
 		* @brief Creates a new transaction.
 		* @param fromAdr Address of the sender.
 		* @param toAdr Address of the recipient.
 		* @param amnt Amount to transfer.
 		*/
		explicit Transaction(std::string_view fromAdr, std::string_view toAdr, double amnt);
		/**
 		* @brief Signs the transaction using the sender's private key.
 		* @param privateKey Pointer to the sender's private key.
 		*
 		* The signature ensures that only the owner of the corresponding
 		* public key could have created this transaction.
 		*/
		void signTransaction(EVP_PKEY* privateKey);
		/**
 		* @brief Checks if the transaction is valid.
 		* @return True if the transaction's signature and hash are valid, false otherwise.
 		*/
		[[nodiscard]] bool isValid() const;
		/**
 		* @brief Calculates the SHA-256 hash of the transaction's contents.
 		* @return Hexadecimal string of the transaction hash.
 		*/
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
