class Transaction{
	public:
		std::string from;
		std::string to;
		int amount;
		std::string signature;

		Transaction(std::string fromAdr, std::string toAdr, int amnt);
		void signTransaction(EVP_PKEY* privateKey);
		bool isValid() const;
};
