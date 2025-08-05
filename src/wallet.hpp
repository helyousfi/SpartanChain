class Wallet{
	public:
		EVP_PKEY* keypair;
		std::string address;

		Wallet();
		~Wallet();

		void save(const std::string& path);
	       	static Wallet load(const std::string& path);
		
		std::string getAddress() const;
		Transaction createTransaction(const std::string& to, int amount); 
		std::string getPublicKeyString() const;

		Wallet(const Wallet&) = delete;
		Wallet& operator=(const Wallet&) = delete;
		Wallet(Wallet&& other) noexcept;
		Wallet& operator=(Wallet&& other) noexcept;
};
