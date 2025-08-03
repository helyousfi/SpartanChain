class Block{
public:
	int index;
	std::string previousHash;
	std::string timestamp;
	std::vector<Transaction> transactions;
	int nonce;
	std::string hash;
	
	Block(int idx, std::string prevHash, std::vector<Transaction> trx);
	std::string calculateHash() const;
	void mine(int difficulty);
};
