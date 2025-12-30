#include <Arduino.h>

// ========== ATM SYSTEM CONFIGURATION ==========
#define MAX_ACCOUNTS 5           // Limited by Arduino memory
#define MAX_PIN_ATTEMPTS 3       // Security limit
#define INITIAL_BALANCE 1000.00  // Default starting balance

// ========== ACCOUNT CLASS (Basic OOP) ==========
class Account {
private:
    int accountNumber;
    int pin;
    float balance;
    bool isActive;

public:
    // Constructor
    Account() {
        accountNumber = 0;
        pin = 0;
        balance = 0;
        isActive = false;
    }

    // Initialize account
    void initialize(int accNum, int accPin, float initialBalance = INITIAL_BALANCE) {
        accountNumber = accNum;
        pin = accPin;
        balance = initialBalance;
        isActive = true;
    }

    // Verify PIN
    bool verifyPin(int inputPin) {
        return (pin == inputPin && isActive);
    }

    // Get balance
    float getBalance() {
        return balance;
    }

    // Deposit money
    bool deposit(float amount) {
        if (amount > 0 && isActive) {
            balance += amount;
            return true;
        }
        return false;
    }

    // Withdraw money
    bool withdraw(float amount) {
        if (amount > 0 && amount <= balance && isActive) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // Check if account is active
    bool isAccountActive() {
        return isActive;
    }

    // Get account number
    int getAccountNumber() {
        return accountNumber;
    }
};

// ========== ATM CLASS ==========
class ATM {
private:
    Account accounts[MAX_ACCOUNTS];
    int accountCount;
    Account* currentAccount;
    bool isAuthenticated;

public:
    // Constructor
    ATM() {
        accountCount = 0;
        currentAccount = nullptr;
        isAuthenticated = false;
        initializeDemoAccounts();
    }

    // Initialize demo accounts for testing
    void initializeDemoAccounts() {
        // Create some demo accounts
        createAccount(1001, 1234, 1500.50);
        createAccount(1002, 5678, 2500.75);
        createAccount(1003, 9012, 500.25);
    }

    // Create new account
    bool createAccount(int accNum, int pin, float initialBalance = INITIAL_BALANCE) {
        if (accountCount >= MAX_ACCOUNTS) {
            return false;
        }
        
        // Check if account number already exists
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].getAccountNumber() == accNum) {
                return false;
            }
        }
        
        accounts[accountCount].initialize(accNum, pin, initialBalance);
        accountCount++;
        return true;
    }

    // PIN verification
    bool authenticate(int accNum, int pin) {
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].getAccountNumber() == accNum) {
                if (accounts[i].verifyPin(pin)) {
                    currentAccount = &accounts[i];
                    isAuthenticated = true;
                    return true;
                }
            }
        }
        return false;
    }

    // Logout
    void logout() {
        currentAccount = nullptr;
        isAuthenticated = false;
    }

    // Check if authenticated
    bool isUserAuthenticated() {
        return isAuthenticated;
    }

    // Balance inquiry
    float checkBalance() {
        if (isAuthenticated && currentAccount) {
            return currentAccount->getBalance();
        }
        return -1; // Error code
    }

    // Deposit money
    bool makeDeposit(float amount) {
        if (isAuthenticated && currentAccount) {
            return currentAccount->deposit(amount);
        }
        return false;
    }

    // Withdraw money
    bool makeWithdrawal(float amount) {
        if (isAuthenticated && currentAccount) {
            return currentAccount->withdraw(amount);
        }
        return false;
    }

    // Get current account number
    int getCurrentAccountNumber() {
        if (currentAccount) {
            return currentAccount->getAccountNumber();
        }
        return -1;
    }
};

// ========== GLOBAL ATM INSTANCE ==========
ATM atm;

// ========== FUNCTION PROTOTYPES ==========
void mainMenu();
void loginScreen();
void atmOperations();
void balanceInquiry();
void depositMoney();
void withdrawMoney();
void adminMenu();
float getFloatInput(const char* prompt);
int getIntInput(const char* prompt);
void clearInputBuffer();

// ========== SETUP FUNCTION ==========
void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // Wait for serial port
    }
    
    Serial.println(F("========================================"));
    Serial.println(F("       SIMPLE ATM SIMULATION"));
    Serial.println(F("     For Arduino Uno - PlatformIO"));
    Serial.println(F("========================================"));
    Serial.println(F("\nDEMO ACCOUNTS:"));
    Serial.println(F("  1001 | PIN: 1234 | Balance: $1500.50"));
    Serial.println(F("  1002 | PIN: 5678 | Balance: $2500.75"));
    Serial.println(F("  1003 | PIN: 9012 | Balance: $500.25"));
    Serial.println(F("\nType 'admin' for admin menu"));
    Serial.println(F("========================================"));
    
    mainMenu();
}

// ========== LOOP FUNCTION ==========
void loop() {
    // Keep Arduino running
    delay(1000);
}

// ========== MAIN MENU ==========
void mainMenu() {
    while (true) {
        if (!atm.isUserAuthenticated()) {
            loginScreen();
        } else {
            atmOperations();
        }
    }
}

// ========== LOGIN SCREEN ==========
void loginScreen() {
    Serial.println(F("\n=== ATM LOGIN ==="));
    Serial.println(F("Enter '0' to exit"));
    Serial.println(F("Type 'admin' for admin menu"));
    
    // Get account number
    int accNum = getIntInput("Enter Account Number: ");
    
    if (accNum == 0) {
        Serial.println(F("\nThank you for using our ATM!"));
        Serial.println(F("Goodbye!"));
        while (true) { delay(1000); } // Stop program
    }
    
    // Admin access
    if (accNum == 9999) {  // Special admin code
        adminMenu();
        return;
    }
    
    // PIN verification with attempts
    int attempts = 0;
    bool authenticated = false;
    
    while (attempts < MAX_PIN_ATTEMPTS && !authenticated) {
        int pin = getIntInput("Enter PIN: ");
        
        if (atm.authenticate(accNum, pin)) {
            authenticated = true;
            Serial.println(F("\n✓ Login successful!"));
            Serial.print(F("Welcome, Account #"));
            Serial.println(accNum);
            break;
        } else {
            attempts++;
            Serial.print(F("✗ Invalid PIN. Attempts left: "));
            Serial.println(MAX_PIN_ATTEMPTS - attempts);
            
            if (attempts >= MAX_PIN_ATTEMPTS) {
                Serial.println(F("\n⚠️  Maximum attempts reached!"));
                Serial.println(F("Please contact customer service."));
                delay(3000);
            }
        }
    }
}

// ========== ATM OPERATIONS ==========
void atmOperations() {
    while (atm.isUserAuthenticated()) {
        Serial.println(F("\n=== ATM MAIN MENU ==="));
        Serial.println(F("1. Balance Inquiry"));
        Serial.println(F("2. Deposit Money"));
        Serial.println(F("3. Withdraw Money"));
        Serial.println(F("4. Logout"));
        Serial.print(F("Enter choice (1-4): "));
        
        while (!Serial.available());
        char choice = Serial.read();
        Serial.println(choice);
        clearInputBuffer();
        
        switch(choice) {
            case '1':
                balanceInquiry();
                break;
            case '2':
                depositMoney();
                break;
            case '3':
                withdrawMoney();
                break;
            case '4':
                Serial.println(F("\nLogging out..."));
                Serial.println(F("Thank you for banking with us!"));
                atm.logout();
                return;
            default:
                Serial.println(F("Invalid choice!"));
        }
        
        delay(1000); // Pause for readability
    }
}

// ========== BALANCE INQUIRY ==========
void balanceInquiry() {
    Serial.println(F("\n--- BALANCE INQUIRY ---"));
    float balance = atm.checkBalance();
    
    if (balance >= 0) {
        Serial.print(F("Account #"));
        Serial.println(atm.getCurrentAccountNumber());
        Serial.print(F("Available Balance: $"));
        Serial.println(balance, 2);
    } else {
        Serial.println(F("Error checking balance!"));
    }
}

// ========== DEPOSIT MONEY ==========
void depositMoney() {
    Serial.println(F("\n--- DEPOSIT MONEY ---"));
    float amount = getFloatInput("Enter deposit amount: $");
    
    if (amount <= 0) {
        Serial.println(F("Invalid amount!"));
        return;
    }
    
    // Validate amount
    if (amount > 10000) {
        Serial.println(F("Maximum deposit limit is $10,000!"));
        return;
    }
    
    Serial.print(F("Depositing $"));
    Serial.print(amount, 2);
    Serial.println(F("..."));
    
    if (atm.makeDeposit(amount)) {
        Serial.println(F("✓ Deposit successful!"));
        Serial.print(F("New Balance: $"));
        Serial.println(atm.checkBalance(), 2);
    } else {
        Serial.println(F("✗ Deposit failed!"));
    }
}

// ========== WITHDRAW MONEY ==========
void withdrawMoney() {
    Serial.println(F("\n--- WITHDRAW MONEY ---"));
    
    // Check available balance first
    float balance = atm.checkBalance();
    if (balance <= 0) {
        Serial.println(F("Insufficient funds!"));
        return;
    }
    
    Serial.print(F("Available: $"));
    Serial.println(balance, 2);
    
    float amount = getFloatInput("Enter withdrawal amount: $");
    
    if (amount <= 0) {
        Serial.println(F("Invalid amount!"));
        return;
    }
    
    // Validate amount
    if (amount > balance) {
        Serial.println(F("Insufficient funds!"));
        return;
    }
    
    if (amount > 1000) {
        Serial.println(F("Maximum withdrawal limit is $1,000!"));
        return;
    }
    
    // Check for multiples of 10 (simulating real ATM)
    if (fmod(amount, 10) != 0) {
        Serial.println(F("Amount must be in multiples of $10!"));
        return;
    }
    
    Serial.print(F("Withdrawing $"));
    Serial.print(amount, 2);
    Serial.println(F("..."));
    
    if (atm.makeWithdrawal(amount)) {
        Serial.println(F("✓ Withdrawal successful!"));
        Serial.println(F("Please take your cash."));
        Serial.print(F("Remaining Balance: $"));
        Serial.println(atm.checkBalance(), 2);
    } else {
        Serial.println(F("✗ Withdrawal failed!"));
    }
}

// ========== ADMIN MENU ==========
void adminMenu() {
    Serial.println(F("\n=== ADMIN MENU ==="));
    Serial.println(F("1. View All Accounts"));
    Serial.println(F("2. Create New Account"));
    Serial.println(F("3. Back to Main Menu"));
    Serial.print(F("Enter choice (1-3): "));
    
    while (!Serial.available());
    char choice = Serial.read();
    Serial.println(choice);
    clearInputBuffer();
    
    switch(choice) {
        case '1':
            Serial.println(F("\n--- ALL ACCOUNTS ---"));
            // In a real system, this would list all accounts
            Serial.println(F("Account #1001: $1500.50"));
            Serial.println(F("Account #1002: $2500.75"));
            Serial.println(F("Account #1003: $500.25"));
            break;
        case '2':
            Serial.println(F("\nFeature not implemented in demo."));
            Serial.println(F("Maximum accounts: 5 (Uno memory limit)"));
            break;
        case '3':
            Serial.println(F("Returning to main menu..."));
            return;
        default:
            Serial.println(F("Invalid choice!"));
    }
    
    adminMenu(); // Show menu again
}

// ========== HELPER FUNCTIONS ==========
float getFloatInput(const char* prompt) {
    Serial.print(prompt);
    
    while (!Serial.available());
    String input = Serial.readStringUntil('\n');
    
    // Remove carriage return if present
    input.trim();
    
    // Check for empty input
    if (input.length() == 0) {
        return 0;
    }
    
    // Convert to float
    char buffer[20];
    input.toCharArray(buffer, sizeof(buffer));
    return atof(buffer);
}

int getIntInput(const char* prompt) {
    Serial.print(prompt);
    
    while (!Serial.available());
    String input = Serial.readStringUntil('\n');
    
    // Remove carriage return if present
    input.trim();
    
    // Check for admin keyword
    if (input.equalsIgnoreCase("admin")) {
        return 9999; // Admin code
    }
    
    // Check for empty input
    if (input.length() == 0) {
        return 0;
    }
    
    // Convert to int
    char buffer[20];
    input.toCharArray(buffer, sizeof(buffer));
    return atoi(buffer);
}

void clearInputBuffer() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}

// ========== PLATFORMIO.INI CONFIGURATION ==========
/*
[env:uno]
platform = atmelavr
board = uno
framework = arduino

monitor_speed = 9600
monitor_filters = direct
monitor_flags = 
    --echo
    --eol CRLF
*/