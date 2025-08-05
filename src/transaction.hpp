class Transaction{
	private:
		std::string from;
		std::string to;
		double amount;
		std::string signature;
	public:
		Transaction(const std::string& fromAdr, const std::string& toAdr, double amnt);
		void signTransaction(EVP_PKEY* privateKey);
		bool isValid() const;
};
