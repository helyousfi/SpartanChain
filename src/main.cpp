#include "block.hpp"
#include "transaction.hpp" // Assuming you have a Transaction class
#include <iostream>
#include <vector>

int main() {
    std::vector<Transaction> genesisTransactions = {
        Transaction("Ahmad", "Bob", 10.0),
        Transaction("Charlie", "Dave", 5.0)
    };

    // Create the genesis block
    Block genesisBlock(0, "0", genesisTransactions);
    genesisBlock.mine(4); // difficulty = 4 leading zeroes

    std::cout << genesisBlock.toString() << std::endl;

    std::vector<Transaction> block1Transactions = {
        Transaction("Eve", "Frank", 2.5),
        Transaction("Bob", "Alice", 3.2)
    };

    Block block1(1, genesisBlock.getHash(), block1Transactions);
    block1.mine(4);

    std::cout << block1.toString() << std::endl;

    if (block1.isLinkedTo(genesisBlock)) {
        std::cout << "Block 1 is correctly linked to the genesis block.\n";
    } else {
        std::cout << "Block linkage error!\n";
    }

    return 0;
}
