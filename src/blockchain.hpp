#pragma once

#include "block.hpp"
#include "transaction.hpp"
#include <vector>
#include <string_view>

class Blockchain final {
private:
    static constexpr int DEFAULT_DIFFICULTY = 4;
    static constexpr int DEFAULT_REWARD = 5;
    
    bool verboseLoggingEnabled{false}; // verbosity of debug messages

    alignas(64) std::vector<Block> chain;
    std::vector<Transaction> pendingTransactions;
    int difficulty;
    int miningReward;

    const uint64_t MAX_SUPPLY = 10'000; // maximum supply
    uint64_t totalCoinsIssued = 0;

    [[nodiscard]] bool isVerboseLoggingEnabled() const noexcept;
    void enableVerboseLogging(bool enable = true) noexcept;

public:
    explicit Blockchain(int difficulty = DEFAULT_DIFFICULTY, int miningReward = DEFAULT_REWARD);

    void addTransaction(const Transaction& tx);
    void addTransaction(Transaction&& tx); // rvalue overload, useful if callers are creating transactions temporarily
    void minePendingTransaction(std::string_view minerAddress);

    [[nodiscard]] bool isChainValid() const;
    [[nodiscard]] double getBalanceOf(std::string_view address) const;

    void printChain() const noexcept;

    [[nodiscard]] const Block& getLatestBlock() const noexcept;
    [[nodiscard]] size_t getChainSize() const noexcept { return chain.size(); }
    [[nodiscard]] const std::vector<Block>& getChain() const noexcept { return chain; }

    [[nodiscard]] std::string serialize() const;
    static Blockchain deserialize(const std::string& data);

    Blockchain(const Blockchain&) = delete;                  // Prevent copy constructor
    Blockchain& operator=(const Blockchain&) = delete;       // Prevent copy assignment
    Blockchain(Blockchain&&) = default;                      // Allow move constructor
    Blockchain& operator=(Blockchain&&) = default;           // Allow move assignment

    void reset() noexcept; // Clear chain and start from genesis again
};
