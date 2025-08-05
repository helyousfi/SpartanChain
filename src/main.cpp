#include "blockchain.hpp"
#include "transaction.hpp"
#include <iostream>

int main() {
    // Create a blockchain with difficulty 4 and mining reward of 50 coins
    Blockchain myChain(4, 50);

    // Create a wallet/address for Alice and Bob (just string here)
    std::string alice = "f2ca1bb6c7e907d06dafe4687e579fce9e5251ff453b64d6f6f7898d2f1f0d41";
    std::string bob = "9b74c9897bac770ffc029102a200c5de3a4f5e6f8bde7e67a2f97d36b02f53d7";

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
