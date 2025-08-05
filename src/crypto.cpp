#include "crypto.hpp"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstring>

namespace Crypto {

    std::string sha256(const std::string& input) { // Secure Hash Algorithm 256-bit.
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash); 
        std::ostringstream result;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return result.str();
    }

    EVP_PKEY* generateKey() {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        if (!ctx) throw std::runtime_error("Failed to create context");

        if (EVP_PKEY_keygen_init(ctx) <= 0 ||
            EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize key generation");
        }

        EVP_PKEY* pkey = nullptr;
        if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Key generation failed");
        }

        EVP_PKEY_CTX_free(ctx);
        return pkey;
    }

    std::string getPublicKeyString(EVP_PKEY* key) {
        BIO* bio = BIO_new(BIO_s_mem());
        if (!bio) throw std::runtime_error("Failed to create BIO");

        if (!PEM_write_bio_PUBKEY(bio, key)) {
            BIO_free(bio);
            throw std::runtime_error("Failed to write public key");
        }

        char* data;
        long len = BIO_get_mem_data(bio, &data);
        std::string pubKey(data, len);

        BIO_free(bio);
        return pubKey;
    }

    std::string sign(const std::string& message, EVP_PKEY* privateKey) {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) throw std::runtime_error("Failed to create digest context");

        if (EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, privateKey) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("DigestSignInit failed");
        }

        if (EVP_DigestSignUpdate(ctx, message.c_str(), message.size()) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("DigestSignUpdate failed");
        }

        size_t sigLen = 0;
        if (EVP_DigestSignFinal(ctx, nullptr, &sigLen) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("DigestSignFinal (size) failed");
        }

        std::vector<unsigned char> sig(sigLen);
        if (EVP_DigestSignFinal(ctx, sig.data(), &sigLen) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("DigestSignFinal failed");
        }

        EVP_MD_CTX_free(ctx);
        return std::string(reinterpret_cast<char*>(sig.data()), sigLen);
    }

    bool verify(const std::string& message, const std::string& sig, EVP_PKEY* pubkey) {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) throw std::runtime_error("Failed to create digest context");

        if (EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pubkey) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("DigestVerifyInit failed");
        }

        if (EVP_DigestVerifyUpdate(ctx, message.c_str(), message.size()) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("DigestVerifyUpdate failed");
        }

        int ret = EVP_DigestVerifyFinal(ctx,
            reinterpret_cast<const unsigned char*>(sig.c_str()), sig.size());

        EVP_MD_CTX_free(ctx);
        return ret == 1;
    }

}

