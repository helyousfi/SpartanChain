#pragma once
#include <string>
#include <ctime>
#include <vector>
#include "transaction.hpp"

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
    explicit Block(int idx, const std::string& prevHash, const std::vector<Transaction>& trx);
    
    void mine(int difficulty);
    std::string toString() const noexcept;
    [[nodiscard]] std::string calculateHash() const;
    [[nodiscard]] bool isLinkedTo(const Block& previousBlock) const;

    // Safe getters, read-only access
    int getIndex() const noexcept;
    int getNonce() const noexcept;
    const std::string& getPreviousHash() const;
    const std::string& getHash() const noexcept;
    const std::vector<Transaction>& getTransactions() const noexcept;
    const std::string& getTimestamp() const noexcept;
    std::time_t getRawTimestamp() const noexcept;
    double getMiningDuration() const noexcept;

    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;
    Block(Block&&) noexcept = default;
    Block& operator=(Block&&) noexcept = default;

};
