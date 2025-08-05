#include <string>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

namespace Crypto {
	std::string sha256(const  std::string& input);
	EVP_PKEY* generateKey();
	EVP_PKEY* getPublicKeyFromString(const std::string& pubKeyStr);
	std::string getPublicKeyString(EVP_PKEY* key);
	std::string sign(const std::string& message, EVP_PKEY* key);
	bool verify(const std::string& message, const std::string& sig, EVP_PKEY* pubkey);
};
