# Bank Management Application

A C++ based Bank Management Application that simulates core banking operations
using Object-Oriented Programming and file handling.

## Objective
To design and implement a C++ based Bank Management Application that
simulates core banking operations using object-oriented programming and file
handling.

## Key Features
- Create a new account (auto-generated account number)
- Deposit money
- Withdraw money (with insufficient-balance check)
- Check balance / view full account details
- View all accounts
- Delete an account
- Customer data stored securely and persistently in a binary file
  (`accounts.dat`) using file handling — records survive between program runs

## Design
- **`Account` class** — encapsulates account number, holder name, account
  type, and balance. Provides controlled access via getters and
  deposit/withdraw methods.
- **`Bank` class** — manages the collection of accounts: create, deposit,
  withdraw, search, display, and delete, all backed by binary file I/O.
- **File handling** — accounts are stored as fixed-size binary records in
  `accounts.dat`, enabling fast lookups (`seekg`/`seekp`) and in-place updates.

## Build

Requires a C++ compiler that supports C++17 (e.g. g++).

```bash
g++ -std=c++17 -Wall -o bank main.cpp
```

## Run

```bash
./bank
```

On Windows (after building with MinGW or MSVC):
```bash
bank.exe
```

## Usage
Follow the on-screen menu:
1. Create Account
2. Deposit
3. Withdraw
4. Check Balance
5. View All Accounts
6. Delete Account
7. Exit

Account data is automatically saved to `accounts.dat` in the same folder and
will persist the next time you run the program.

## Uploading to GitHub
```bash
git init
git add .
git commit -m "Initial commit: Bank Management Application"
git branch -M main
git remote add origin https://github.com/<your-username>/<repo-name>.git
git push -u origin main
```
