class Wallet{
	public:
		EVP_PKEY* keypair;
		std::string address;

		Wallet();
		void save(const std::string& path);
	       	static Wallet load(const std::string& path);
		std::string getAddress() const;
		Transaction createTransaction(const std::string& to, int amount); 	
};
