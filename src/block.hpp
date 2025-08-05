class Block {
private:
    int index;
    std::string previousHash;
    std::string timestamp;
    std::time_t rawTimestamp;
    std::vector<Transaction> transactions;
    int nonce;
    std::string hash;
    double miningDuration;

public:
    Block(int idx, std::string prevHash, std::vector<Transaction> trx);
    
    std::string calculateHash() const;
    void mine(int difficulty);
    std::string toString() const;
    bool isLinkedTo(const Block& previousBlock) const;

    // Safe getters, read-only access
    int getIndex() const;
    const std::string& getPreviousHash() const;
    const std::string& getHash() const;
    const std::vector<Transaction>& getTransactions() const;
    const std::string& getTimestamp() const;
    std::time_t getRawTimestamp() const;
    int getNonce() const;
    double getMiningDuration() const;
};
