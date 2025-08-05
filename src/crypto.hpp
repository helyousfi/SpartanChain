namespace Crypto {
	std::string sha256(const  std::string& input);
	EVP_PKEY* generateKey();
	std::string getPublicKeyString(EVP_PKEY* key);
	std::string sign(const std::string& message, EVP_PKEY* key);
	bool verify(const std::string& message, const std::string& sig, EVP_PKEY* pubkey);
};
