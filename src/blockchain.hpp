#pragma once

#include "block.hpp"
#include "transaction.hpp"
#include <vector>
#include <string_view>

class Blockchain final {
private:
    std::vector<Block> chain;
    std::vector<Transaction> pendingTransactions;
    int difficulty;
    int miningReward;

public:
    explicit Blockchain();

    void addTransaction(const Transaction& tx);
    void minePendingTransaction(std::string_view minerAddress);

    [[nodiscard]] bool isChainValid() const;
    [[nodiscard]] double getBalanceOf(std::string_view address) const;

    void printChain() const;

    Blockchain(const Blockchain&) = delete;                  // Prevent copy constructor
    Blockchain& operator=(const Blockchain&) = delete;       // Prevent copy assignment
    Blockchain(Blockchain&&) = default;                      // Allow move constructor
    Blockchain& operator=(Blockchain&&) = default;           // Allow move assignment
};
