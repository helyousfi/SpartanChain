#pragma once
#include "transaction.hpp"
#include <string>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>

/**
 * @class Wallet
 * @brief Represents a blockchain wallet containing a cryptographic keypair and address.
 *
 * A Wallet stores a private/public keypair used to sign transactions
 * and an associated blockchain address derived from the public key.
 *
 * Responsibilities:
 * - Generate and store a secure asymmetric keypair (private + public).
 * - Provide the blockchain address for receiving funds.
 * - Create and sign transactions.
 * - Save/load wallet data to/from disk for persistence.
 *
 * The wallet ensures that private keys never leave its control except
 * when explicitly saved to a file. Copy operations are disabled to
 * prevent accidental duplication of sensitive key material, but move
 * operations are allowed for transfer of ownership.
 *
 * Uses OpenSSL for cryptographic operations.
 */
class Wallet{
	public:
		EVP_PKEY* keypair;
		std::string address;

		/**
 		* @brief Constructs a new wallet with a freshly generated keypair.
 		*
 		* The wallet will generate a secure elliptic curve keypair and derive
 		* a blockchain address from the public key.
 		*/
		Wallet();
		/**
 		* @brief Destroys the wallet and securely frees the keypair.
 		*/
		~Wallet();
		/**
 		* @brief Saves the wallet's private key to a file.
 		* @param path Path to the file where the key will be stored.
 		*/
		void save(const std::string& path) const;
	    /**
 		* @brief Loads a wallet from a private key file.
 		* @param path Path to the file containing the wallet's private key.
 		* @return A new Wallet instance containing the loaded keypair.
 		*/
	    static Wallet load(const std::string& path);
		/**
 		* @brief Returns the wallet's blockchain address.
 		* @return Address as a string.
 		*/
		std::string getAddress() const;
		/**
 		* @brief Creates and signs a transaction from this wallet.
 		* @param to Recipient's blockchain address.
 		* @param amount Amount to transfer.
 		* @return A signed Transaction object ready to be added to the blockchain.
 		*/
		Transaction createTransaction(const std::string& to, int amount); 
		/**
 		* @brief Returns the public key as a string.
 		* @return Public key in string (PEM) format.
 		*/
		std::string getPublicKeyString() const;
		/**
 		* @brief Returns a pointer to the wallet's private key.
 		* @return EVP_PKEY pointer (do not free externally).
 		*/

		Wallet(const Wallet&) = delete;
		Wallet& operator=(const Wallet&) = delete;
		Wallet(Wallet&& other) noexcept;
		Wallet& operator=(Wallet&& other) noexcept;
};
