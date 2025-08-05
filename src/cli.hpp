#ifndef CLI_HPP
#define CLI_HPP

#include <string>
#include "blockchain.hpp"
#include "wallet.hpp"

class CLI {
private:
    Blockchain blockchain;
    Wallet wallet;

public:
    CLI();

    void run();
    void help();
    void createWallet();
    void sendCoins();
    void mine();
    void checkBalance();
    void showChain();
    void validate();
};

#endif // CLI_HPP

