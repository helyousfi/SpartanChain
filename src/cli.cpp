#include "cli.hpp"
#include <iostream>
#include "constants.hpp"

CLI::CLI() : blockchain(6, 50) {
}

void CLI::run() {
    std::string command;
    help();
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        if (command == "help") help();
        else if (command == "createwallet") createWallet();
        else if (command == "send") sendCoins();
        else if (command == "mine") mine();
        else if (command == "balance") checkBalance();
        else if (command == "showchain") showChain();
        else if (command == "validate") validate();
        else if (command == "exit") break;
        else std::cout << "Unknown command. Type 'help' to see available commands.\n";
    }
}

void CLI::help() {
    std::cout << "Available commands:\n"
              << "  createwallet - Create a new wallet\n"
              << "  send         - Send coins to another address\n"
              << "  mine         - Mine pending transactions\n"
              << "  balance      - Check your wallet balance\n"
              << "  showchain    - Print the blockchain\n"
              << "  validate     - Validate blockchain integrity\n"
              << "  help         - Show this help message\n"
              << "  exit         - Exit the program\n";
}

void CLI::createWallet() {
	wallet = Wallet(); // generates new keypair and address
    	std::cout << "New wallet created!\nYour address: " << wallet.getAddress() << std::endl;
   	blockchain.addTransaction(Transaction(SYSTEM_ADDRESS, wallet.getAddress(), 100.00));
}

void CLI::sendCoins() {
    std::string to;
    double amount;
    std::cout << "Enter recipient address: ";
    std::getline(std::cin, to);
    std::cout << "Enter amount to send: ";
    std::cin >> amount;
    std::cin.ignore(); // remove leftover newline

    if (amount <= 0) {
        std::cout << "Amount must be positive!\n";
        return;
    }
    if (blockchain.getBalanceOf(wallet.getAddress()) < amount) {
        std::cout << "Insufficient balance!\n";
        return;
    }

    auto tx = wallet.createTransaction(to, amount);
    tx.signTransaction(wallet.getPrivateKey());  // You need to implement getPrivateKey() in Wallet
    blockchain.addTransaction(std::move(tx));
    std::cout << "Transaction added to pending transactions.\n";
}

void CLI::mine() {
    blockchain.minePendingTransaction(wallet.getAddress());
}

void CLI::checkBalance() {
    double balance = blockchain.getBalanceOf(wallet.getAddress());
    std::cout << "Balance for " << wallet.getAddress() << ": " << balance << " coins\n";
}

void CLI::showChain() {
    blockchain.printChain();
}

void CLI::validate() {
    if (blockchain.isChainValid())
        std::cout << "Blockchain is valid.\n";
    else
        std::cout << "Blockchain is INVALID!\n";
}

