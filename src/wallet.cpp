#include "wallet.hpp"
#include "crypto.hpp"
#include "transaction.hpp"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

Wallet::Wallet() {
    keypair = Crypto::generateKey();
    if (!keypair) {
        throw std::runtime_error("Failed to generate keypair");
    }
    address = Crypto::getPublicKeyString(keypair);

}

Wallet::~Wallet() {
    if (keypair) {
        EVP_PKEY_free(keypair);
    }
}

Wallet::Wallet(Wallet&& other) noexcept {
    keypair = other.keypair;
    address = std::move(other.address);
    other.keypair = nullptr;
}

Wallet& Wallet::operator=(Wallet&& other) noexcept {
    if (this != &other) {
        if (keypair) EVP_PKEY_free(keypair);
        keypair = other.keypair;
        address = std::move(other.address);
        other.keypair = nullptr;
    }
    return *this;
}

void Wallet::save(const std::string& path) const {
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) throw std::runtime_error("Could not open file to save wallet");

    PEM_write_PrivateKey(fp, keypair, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(fp);
}

Wallet Wallet::load(const std::string& path) {
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) throw std::runtime_error("Could not open wallet file to load");

    EVP_PKEY* key = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);

    if (!key) throw std::runtime_error("Could not read private key");

    Wallet wallet;
    if (wallet.keypair) EVP_PKEY_free(wallet.keypair); // free the one from constructor
    wallet.keypair = key;
    wallet.address = Crypto::getPublicKeyString(key);
    return wallet;
}

std::string Wallet::getAddress() const {
    return address;
}

std::string Wallet::getPublicKeyString() const {
    return Crypto::getPublicKeyString(keypair);
}

EVP_PKEY* Wallet::getPrivateKey() const 
{
	return keypair;
}

Transaction Wallet::createTransaction(const std::string& to, int amount) {
    Transaction tx(this->address, to, amount);
    tx.signature = Crypto::sign(tx.toString(), keypair);
    return tx;
}


