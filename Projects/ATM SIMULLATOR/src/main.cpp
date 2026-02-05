#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <limits>

using namespace std;

// ============================================================
// CLASSES - Demonstrating Object-Oriented Programming
// ============================================================

// Transaction class to store transaction details
class Transaction {
private:
    string type;        // "DEPOSIT", "WITHDRAWAL", "BALANCE_INQUIRY"
    double amount;
    string timestamp;
    double balanceAfter;

public:
    // Constructor
    Transaction(string t, double a, double bal) {
        type = t;
        amount = a;
        balanceAfter = bal;
        
        // Get current time for timestamp
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // Remove newline character
    }

    // Display transaction details
    void display() const {
        cout << left << setw(20) << timestamp 
             << setw(15) << type 
             << setw(12) << fixed << setprecision(2) << "$" << amount
             << setw(12) << "$" << balanceAfter << endl;
    }

    // Getter methods
    string getType() const { return type; }
    double getAmount() const { return amount; }
    string getTimestamp() const { return timestamp; }
    double getBalanceAfter() const { return balanceAfter; }
};

// Bank Account class
class BankAccount {
private:
    int accountNumber;
    int pin;
    string accountHolder;
    double balance;
    vector<Transaction> transactionHistory; // Array of transactions

public:
    // Constructor
    BankAccount(int accNum, int pinNum, string holder, double initialBalance = 0.0) {
        accountNumber = accNum;
        pin = pinNum;
        accountHolder = holder;
        balance = initialBalance;
    }

    // Getter methods
    int getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }

    // PIN verification
    bool verifyPIN(int enteredPIN) const {
        return (pin == enteredPIN);
    }

    // Deposit money
    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            // Add to transaction history
            transactionHistory.push_back(Transaction("DEPOSIT", amount, balance));
            return true;
        }
        return false;
    }

    // Withdraw money with validation
    bool withdraw(double amount) {
        // CONDITIONALS: Multiple validation checks
        if (amount <= 0) {
            cout << "Error: Amount must be positive!\n";
            return false;
        }
        if (amount > balance) {
            cout << "Error: Insufficient funds!\n";
            cout << "Your balance: $" << fixed << setprecision(2) << balance << endl;
            cout << "Requested: $" << amount << endl;
            return false;
        }
        if (amount > 1000) { // Daily withdrawal limit
            cout << "Error: Withdrawal limit exceeded! Maximum $1000 per transaction.\n";
            return false;
        }

        balance -= amount;
        // Add to transaction history
        transactionHistory.push_back(Transaction("WITHDRAWAL", amount, balance));
        return true;
    }

    // Add balance inquiry to transaction history
    void recordBalanceInquiry() {
        transactionHistory.push_back(Transaction("BALANCE_INQUIRY", 0.0, balance));
    }

    // Display transaction history
    void displayTransactionHistory() const {
        if (transactionHistory.empty()) {
            cout << "No transactions yet.\n";
            return;
        }

        cout << "\n" << string(80, '=') << "\n";
        cout << "                    TRANSACTION HISTORY\n";
        cout << string(80, '=') << "\n";
        cout << left << setw(20) << "DATE/TIME" 
             << setw(15) << "TYPE" 
             << setw(12) << "AMOUNT" 
             << setw(12) << "BALANCE" << endl;
        cout << string(80, '-') << endl;

        // LOOPS: Iterating through transaction array
        for (const auto& transaction : transactionHistory) {
            transaction.display();
        }
        cout << string(80, '=') << "\n";
    }

    // Display account summary
    void displayAccountSummary() const {
        cout << "\n" << string(40, '=') << "\n";
        cout << "  ACCOUNT SUMMARY\n";
        cout << string(40, '=') << "\n";
        cout << left << setw(20) << "Account Number:" << accountNumber << endl;
        cout << setw(20) << "Account Holder:" << accountHolder << endl;
        cout << setw(20) << "Current Balance: $" 
             << fixed << setprecision(2) << balance << endl;
        cout << setw(20) << "Total Transactions:" << transactionHistory.size() << endl;
        cout << string(40, '-') << "\n";
    }
};

// ATM class - Main system
class ATM {
private:
    vector<BankAccount> accounts; // Array of bank accounts
    BankAccount* currentAccount;  // Pointer to current logged-in account

public:
    // Constructor - Initialize with some sample accounts
    ATM() : currentAccount(nullptr) {
        // Create sample accounts
        accounts.push_back(BankAccount(1001, 1234, "John Doe", 1500.00));
        accounts.push_back(BankAccount(1002, 5678, "Jane Smith", 2500.00));
        accounts.push_back(BankAccount(1003, 1111, "Bob Johnson", 500.00));
        accounts.push_back(BankAccount(1004, 9999, "Alice Williams", 3500.00));
    }

    // ============================================================
    // FUNCTIONS - Modularizing ATM operations
    // ============================================================

    // Function to find account by account number
    BankAccount* findAccount(int accountNumber) {
        // LOOPS & CONDITIONALS: Searching through accounts
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr; // Account not found
    }

    // Function for PIN verification
    bool verifyAccount(int accountNumber, int pin) {
        BankAccount* account = findAccount(accountNumber);
        if (account != nullptr && account->verifyPIN(pin)) {
            currentAccount = account;
            return true;
        }
        return false;
    }

    // Function to display main menu
    void displayMainMenu() {
        cout << "\n" << string(50, '=') << "\n";
        cout << "         WELCOME TO SIMPLE ATM\n";
        cout << string(50, '=') << "\n";
        cout << "Account Holder: " << currentAccount->getAccountHolder() << endl;
        cout << "Account Number: " << currentAccount->getAccountNumber() << endl;
        cout << string(50, '-') << "\n";
        cout << "1. Balance Inquiry\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transaction History\n";
        cout << "5. Account Summary\n";
        cout << "6. Logout\n";
        cout << string(50, '-') << "\n";
        cout << "Enter your choice (1-6): ";
    }

    // Function for balance inquiry
    void balanceInquiry() {
        cout << "\n" << string(40, '=') << "\n";
        cout << "   BALANCE INQUIRY\n";
        cout << string(40, '=') << "\n";
        cout << "Account Holder: " << currentAccount->getAccountHolder() << endl;
        cout << "Account Number: " << currentAccount->getAccountNumber() << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) 
             << currentAccount->getBalance() << endl;
        cout << string(40, '=') << "\n";
        
        // Record this inquiry in transaction history
        currentAccount->recordBalanceInquiry();
    }

    // Function for deposit operation
    void depositMoney() {
        double amount;
        
        cout << "\n" << string(40, '=') << "\n";
        cout << "      DEPOSIT MONEY\n";
        cout << string(40, '=') << "\n";
        cout << "Current Balance: $" << fixed << setprecision(2) 
             << currentAccount->getBalance() << endl;
        cout << "Enter amount to deposit: $";
        
        // Input validation loop
        while (!(cin >> amount) || amount <= 0) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid amount! Please enter a positive number: $";
        }
        cin.ignore(); // Clear newline character
        
        // CONDITIONALS: Validate deposit
        if (amount > 10000) { // Deposit limit check
            cout << "Error: Deposit limit is $10,000 per transaction!\n";
            return;
        }
        
        if (currentAccount->deposit(amount)) {
            cout << "\n✓ Deposit successful!\n";
            cout << "Amount deposited: $" << fixed << setprecision(2) << amount << endl;
            cout << "New balance: $" << currentAccount->getBalance() << endl;
        } else {
            cout << "\n✗ Deposit failed!\n";
        }
    }

    // Function for withdrawal operation
    void withdrawMoney() {
        double amount;
        
        cout << "\n" << string(40, '=') << "\n";
        cout << "     WITHDRAW MONEY\n";
        cout << string(40, '=') << "\n";
        cout << "Current Balance: $" << fixed << setprecision(2) 
             << currentAccount->getBalance() << endl;
        cout << "Enter amount to withdraw: $";
        
        // Input validation loop
        while (!(cin >> amount) || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount! Please enter a positive number: $";
        }
        cin.ignore();
        
        // Try to withdraw
        if (currentAccount->withdraw(amount)) {
            cout << "\n✓ Withdrawal successful!\n";
            cout << "Amount withdrawn: $" << fixed << setprecision(2) << amount << endl;
            cout << "New balance: $" << currentAccount->getBalance() << endl;
            
            // Check for low balance warning
            if (currentAccount->getBalance() < 100) {
                cout << "\n⚠ WARNING: Low balance! ($" 
                     << currentAccount->getBalance() << ")\n";
            }
        }
    }

    // Function to run the ATM
    void run() {
        int choice;
        bool running = true;
        
        cout << "\n" << string(60, '=') << "\n";
        cout << "        SIMPLE ATM SIMULATION\n";
        cout << "   (No Hardware Required - Pure Software)\n";
        cout << string(60, '=') << "\n";
        
        // Main program loop
        while (running) {
            // Display login menu
            cout << "\n" << string(40, '=') << "\n";
            cout << "           ATM LOGIN\n";
            cout << string(40, '=') << "\n";
            
            // Available accounts information
            cout << "Sample Accounts Available:\n";
            cout << "--------------------------\n";
            for (const auto& account : accounts) {
                cout << "Account: " << account.getAccountNumber() 
                     << " | PIN: " 
                     << (account.getAccountNumber() == 1001 ? "1234" : 
                         account.getAccountNumber() == 1002 ? "5678" :
                         account.getAccountNumber() == 1003 ? "1111" : "9999")
                     << " | Holder: " << account.getAccountHolder() << endl;
            }
            cout << string(40, '-') << "\n";
            
            int accountNumber, pin;
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter PIN: ";
            cin >> pin;
            
            // CONDITIONALS: PIN verification
            if (verifyAccount(accountNumber, pin)) {
                cout << "\n✓ Login successful! Welcome, " 
                     << currentAccount->getAccountHolder() << "!\n";
                
                // Session loop
                bool sessionActive = true;
                while (sessionActive) {
                    displayMainMenu();
                    cin >> choice;
                    cin.ignore(); // Clear newline
                    
                    // Switch statement for menu choices
                    switch (choice) {
                        case 1:
                            balanceInquiry();
                            break;
                        case 2:
                            depositMoney();
                            break;
                        case 3:
                            withdrawMoney();
                            break;
                        case 4:
                            currentAccount->displayTransactionHistory();
                            break;
                        case 5:
                            currentAccount->displayAccountSummary();
                            break;
                        case 6:
                            cout << "\n✓ Logout successful!\n";
                            cout << "Thank you for using Simple ATM, " 
                                 << currentAccount->getAccountHolder() << "!\n";
                            currentAccount = nullptr;
                            sessionActive = false;
                            break;
                        default:
                            cout << "\n✗ Invalid choice! Please try again.\n";
                    }
                    
                    if (sessionActive) {
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                }
            } else {
                cout << "\n✗ Login failed! Invalid account number or PIN.\n";
                cout << "\nPress Enter to try again...";
                cin.ignore();
                cin.get();
            }
            
            // Ask if user wants to exit completely
            cout << "\nDo you want to exit the ATM? (y/n): ";
            char exitChoice;
            cin >> exitChoice;
            if (tolower(exitChoice) == 'y') {
                running = false;
                cout << "\n" << string(60, '=') << "\n";
                cout << "  Thank you for using Simple ATM Simulation!\n";
                cout << "           Have a nice day!\n";
                cout << string(60, '=') << "\n";
            }
        }
    }
};

// ============================================================
// MAIN FUNCTION
// ============================================================
int main() {
    // Create ATM object and run the simulation
    ATM atm;
    atm.run();
    
    return 0;
}