/*
    Bank Management Application
    ----------------------------
    A C++ based Bank Management Application that simulates core banking
    operations using Object-Oriented Programming and file handling.

    Features:
      - Create a new account
      - Deposit money
      - Withdraw money
      - Check balance
      - View all accounts
      - Delete an account
      - Persistent storage using file handling (accounts.dat)
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <vector>

using namespace std;

const string DATA_FILE = "accounts.dat";

// ----------------------- Account Class -----------------------
class Account {
private:
    int accountNumber;
    char name[50];
    char accountType[20]; // "Savings" or "Current"
    double balance;

public:
    Account() : accountNumber(0), balance(0.0) {
        name[0] = '\0';
        accountType[0] = '\0';
    }

    // ---- Create a new account (called only when opening account) ----
    void createAccount(int accNo) {
        accountNumber = accNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);

        cout << "Enter Account Type (Savings/Current): ";
        cin.getline(accountType, 20);

        cout << "Enter Initial Deposit Amount: ";
        cin >> balance;
        while (cin.fail() || balance < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount. Enter a valid initial deposit: ";
            cin >> balance;
        }
    }

    // ---- Getters ----
    int getAccountNumber() const { return accountNumber; }
    string getName() const { return string(name); }
    string getAccountType() const { return string(accountType); }
    double getBalance() const { return balance; }

    // ---- Deposit money ----
    void deposit(double amount) { balance += amount; }

    // ---- Withdraw money (returns false if insufficient funds) ----
    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    // ---- Display account details ----
    void display() const {
        cout << left
             << setw(15) << accountNumber
             << setw(20) << name
             << setw(12) << accountType
             << fixed << setprecision(2) << setw(12) << balance
             << endl;
    }

    // ---- Binary file read/write support (works for ofstream/ifstream/fstream) ----
    friend ostream& operator<<(ostream& os, const Account& acc);
    friend istream& operator>>(istream& is, Account& acc);
};

ostream& operator<<(ostream& os, const Account& acc) {
    os.write(reinterpret_cast<const char*>(&acc), sizeof(Account));
    return os;
}

istream& operator>>(istream& is, Account& acc) {
    is.read(reinterpret_cast<char*>(&acc), sizeof(Account));
    return is;
}

// ----------------------- Bank Class -----------------------
class Bank {
public:
    void createAccount();
    void deposit();
    void withdraw();
    void checkBalance();
    void displayAll();
    void deleteAccount();

private:
    bool findAccount(int accNo, Account& acc, streampos& pos);
    int generateAccountNumber();
};

// ---- Generate the next available account number ----
int Bank::generateAccountNumber() {
    ifstream infile(DATA_FILE, ios::binary);
    int maxAccNo = 1000; // starting account number
    Account acc;
    while (infile >> acc) {
        if (acc.getAccountNumber() > maxAccNo)
            maxAccNo = acc.getAccountNumber();
    }
    infile.close();
    return maxAccNo + 1;
}

// ---- Create a new account and append it to the file ----
void Bank::createAccount() {
    Account acc;
    int newAccNo = generateAccountNumber();
    acc.createAccount(newAccNo);

    ofstream outfile(DATA_FILE, ios::binary | ios::app);
    if (!outfile) {
        cout << "Error opening file!\n";
        return;
    }
    outfile << acc;
    outfile.close();

    cout << "\nAccount created successfully! Your Account Number is: "
         << newAccNo << "\n";
}

// ---- Locate an account by number; returns true if found ----
bool Bank::findAccount(int accNo, Account& acc, streampos& pos) {
    ifstream infile(DATA_FILE, ios::binary);
    if (!infile) return false;

    while (infile >> acc) {
        if (acc.getAccountNumber() == accNo) {
            pos = infile.tellg();
            pos -= sizeof(Account);
            infile.close();
            return true;
        }
    }
    infile.close();
    return false;
}

// ---- Deposit money into an account ----
void Bank::deposit() {
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;

    Account acc;
    streampos pos;
    if (!findAccount(accNo, acc, pos)) {
        cout << "Account not found!\n";
        return;
    }

    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;
    while (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid amount. Enter a valid deposit amount: ";
        cin >> amount;
    }

    acc.deposit(amount);

    fstream file(DATA_FILE, ios::binary | ios::in | ios::out);
    file.seekp(pos);
    file << acc;
    file.close();

    cout << "Deposit successful! New Balance: " << fixed
         << setprecision(2) << acc.getBalance() << "\n";
}

// ---- Withdraw money from an account ----
void Bank::withdraw() {
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;

    Account acc;
    streampos pos;
    if (!findAccount(accNo, acc, pos)) {
        cout << "Account not found!\n";
        return;
    }

    double amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;
    while (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid amount. Enter a valid withdrawal amount: ";
        cin >> amount;
    }

    if (!acc.withdraw(amount)) {
        cout << "Insufficient balance! Current Balance: "
             << fixed << setprecision(2) << acc.getBalance() << "\n";
        return;
    }

    fstream file(DATA_FILE, ios::binary | ios::in | ios::out);
    file.seekp(pos);
    file << acc;
    file.close();

    cout << "Withdrawal successful! New Balance: " << fixed
         << setprecision(2) << acc.getBalance() << "\n";
}

// ---- Check balance of an account ----
void Bank::checkBalance() {
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;

    Account acc;
    streampos pos;
    if (!findAccount(accNo, acc, pos)) {
        cout << "Account not found!\n";
        return;
    }

    cout << "\n--- Account Details ---\n";
    cout << "Account Number : " << acc.getAccountNumber() << "\n";
    cout << "Name           : " << acc.getName() << "\n";
    cout << "Account Type   : " << acc.getAccountType() << "\n";
    cout << "Balance        : " << fixed << setprecision(2)
         << acc.getBalance() << "\n";
}

// ---- Display all accounts stored in the file ----
void Bank::displayAll() {
    ifstream infile(DATA_FILE, ios::binary);
    if (!infile) {
        cout << "No records found!\n";
        return;
    }

    Account acc;
    bool any = false;
    cout << "\n" << left
         << setw(15) << "AccNo"
         << setw(20) << "Name"
         << setw(12) << "Type"
         << setw(12) << "Balance" << "\n";
    cout << string(59, '-') << "\n";

    while (infile >> acc) {
        acc.display();
        any = true;
    }
    infile.close();

    if (!any) cout << "No records found!\n";
}

// ---- Delete an account ----
void Bank::deleteAccount() {
    int accNo;
    cout << "Enter Account Number to delete: ";
    cin >> accNo;

    ifstream infile(DATA_FILE, ios::binary);
    if (!infile) {
        cout << "No records found!\n";
        return;
    }

    vector<Account> accounts;
    Account acc;
    bool found = false;

    while (infile >> acc) {
        if (acc.getAccountNumber() == accNo)
            found = true;
        else
            accounts.push_back(acc);
    }
    infile.close();

    if (!found) {
        cout << "Account not found!\n";
        return;
    }

    ofstream outfile(DATA_FILE, ios::binary | ios::trunc);
    for (const auto& a : accounts)
        outfile << a;
    outfile.close();

    cout << "Account deleted successfully!\n";
}

// ----------------------- Menu / Driver -----------------------
void showMenu() {
    cout << "\n===== Bank Management System =====\n";
    cout << "1. Create Account\n";
    cout << "2. Deposit\n";
    cout << "3. Withdraw\n";
    cout << "4. Check Balance\n";
    cout << "5. View All Accounts\n";
    cout << "6. Delete Account\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Bank bank;
    int choice;

    cout << "Welcome to the Bank Management Application\n";

    do {
        showMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 7.\n";
            continue;
        }

        switch (choice) {
            case 1: bank.createAccount(); break;
            case 2: bank.deposit();       break;
            case 3: bank.withdraw();      break;
            case 4: bank.checkBalance();  break;
            case 5: bank.displayAll();    break;
            case 6: bank.deleteAccount(); break;
            case 7: cout << "Thank you for using the Bank Management Application!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
