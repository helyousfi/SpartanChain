#pragma once

#include "block.hpp"
#pragma once
#include "transaction.hpp"

#include <vector>
#include <string_view>

/**
 * @class Blockchain
 * @brief Represents the blockchain ledger and its core operations.
 *
 * This class manages the sequence of blocks that form the blockchain.
 * It is responsible for:
 * - Storing and maintaining the chain of blocks.
 * - Handling pending transactions waiting to be mined.
 * - Mining new blocks with proof-of-work.
 * - Validating the integrity of the chain.
 * - Tracking balances and coin issuance.
 *
 * A `Blockchain` instance starts with a configurable difficulty level
 * and mining reward. New transactions are collected into a pending pool
 * until they are mined into a block. The blockchain can be serialized
 * and deserialized for storage or network transmission.
 *
 * Copying is disabled to avoid accidental duplication of the blockchain state,
 * but moving is allowed for efficiency.
 *
 * Thread safety is not guaranteed — external synchronization is required
 * for concurrent access.
 */
class Blockchain final {
private:
    static constexpr int DEFAULT_DIFFICULTY = 4;
    static constexpr int DEFAULT_REWARD = 5;
    
    alignas(64) std::vector<Block> chain;
    std::vector<Transaction> pendingTransactions;

    int difficulty;
    int miningReward;

    uint64_t totalCoinsIssued = 0;

    [[nodiscard]] bool isVerboseLoggingEnabled() const noexcept;
    
    bool verboseLoggingEnabled{true}; // verbosity of debug messages

public:
    explicit Blockchain(int difficulty = DEFAULT_DIFFICULTY, int miningReward = DEFAULT_REWARD);

    void enableVerboseLogging(bool enable = true) noexcept;

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
