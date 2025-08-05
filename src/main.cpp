#include "blockchain.hpp"
#include "transaction.hpp"
#include <iostream>

int main() {
    // Create a blockchain with difficulty 4 and mining reward of 50 coins
    Blockchain myChain(4, 50);

    // Create a wallet/address for Alice and Bob (just string here)
    std::string alice = "AliceAddress";
    std::string bob = "BobAddress";

    // Create some transactions
    Transaction tx1(alice, bob, 10);
    tx1.signTransaction(/* Alice's private key here, if implemented */);

    myChain.addTransaction(tx1);

    // Mine pending transactions, reward goes to Alice
    myChain.minePendingTransaction(alice);

    std::cout << "Balance of Alice: " << myChain.getBalanceOf(alice) << std::endl;
    std::cout << "Balance of Bob: " << myChain.getBalanceOf(bob) << std::endl;

    // Print the blockchain
    myChain.printChain();

    return 0;
}
