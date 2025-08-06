sudo apt install libssl-dev

# SpartanChain

SpartanChain is a lightweight, secure cryptocurrency and blockchain implementation written in modern C++. It includes a cryptographic wallet system, transaction management, and a simple CLI for interacting with the chain.

---

## Features

- Wallet generation using OpenSSL (RSA or EC keys)
- Transaction creation and signing
- Block mining with basic proof-of-work
- Command-line interface (CLI)
- Blockchain explorer (basic output)
- Save/load wallets and blockchain data

---

## Requirements

- C++17 or later
- [OpenSSL](https://www.openssl.org/) (for cryptography)
- CMake (for building)

---

## Build Instructions

```bash
git clone https://github.com/yourusername/spartanchain.git
cd spartanchain
mkdir build && cd build
cmake ..
make
```

## Usage
 ```bash
./spartanchain
```

CLI Commands:
- create_wallet - Generate a new wallet
- load_wallet path/to/file - Load existing wallet
- send <address> <amount> - Create a transaction
- mine - Mine a new block and receive reward
- chain - View the current blockchain
- balance - Show wallet balance
- exit - Exit the CLI
