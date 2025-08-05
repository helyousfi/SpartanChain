class Block{
public:
	int index;
	std::string previousHash;
	std::string timestamp;
	std::time_t rawTimestamp; // for serialization, sorting ...
	std::vector<Transaction> transactions;
	int nonce;
	std::string hash;
	double miningDuration; // duration in seconds
	
	Block(int idx, std::string prevHash, std::vector<Transaction> trx);
	std::string calculateHash() const;
	void mine(int difficulty);

	std::string Block::toString() const; // For debug purpose
	bool isLinkedTo(const Block& previousBlock) const;
};
