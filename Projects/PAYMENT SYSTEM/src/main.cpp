/*
PAYROLL MANAGEMENT SYSTEM - C++ SIMULATION
Runs in VS Code without any hardware
*/

// ============================================================
// HEADER FILES (LIBRARIES) - WITH COMMENTS
// ============================================================

#include <iostream>       // For input/output operations (cin, cout)
#include <iomanip>        // For output formatting (setw, fixed, setprecision)
#include <string>         // For string data type and operations
#include <vector>         // For dynamic arrays (ARRAY CONCEPT - vector)
#include <fstream>        // For FILE HANDLING operations (ifstream, ofstream)
#include <algorithm>      // For sorting and algorithms
#include <ctime>          // For time and date functions
#include <limits>         // For numeric_limits (FIXED: Added this missing include)

using namespace std;

// ============================================================
// CLASSES - Object-Oriented Programming (OOP CONCEPT)
// ============================================================

/*
CLASS: Employee
PURPOSE: Represents an employee with personal and salary details
OOP CONCEPTS: Encapsulation, Data Hiding, Member Functions
*/
class Employee {
private:
    // PRIVATE DATA MEMBERS - Encapsulation principle
    int empID;           // Employee ID
    string empName;      // Employee name
    double basicSalary;  // Basic salary
    double allowances;   // Total allowances
    double deductions;   // Total deductions
    double grossSalary;  // Gross salary
    double netSalary;    // Net salary (take-home)

public:
    // CONSTRUCTOR - Initializes employee object
    // Demonstrates: Constructor with parameters
    Employee(int id = 0, string name = "", double basic = 0.0) {
        empID = id;
        empName = name;
        basicSalary = basic;
        allowances = 0.0;
        deductions = 0.0;
        grossSalary = 0.0;
        netSalary = 0.0;
    }

    // ============================================================
    // GETTER METHODS - Accessor functions
    // Demonstrates: Controlled access to private data
    // ============================================================
    int getID() const { return empID; }
    string getName() const { return empName; }
    double getBasicSalary() const { return basicSalary; }
    double getGrossSalary() const { return grossSalary; }
    double getNetSalary() const { return netSalary; }

    // ============================================================
    // SETTER METHODS - Mutator functions
    // Demonstrates: Controlled modification of private data
    // ============================================================
    void setID(int id) { empID = id; }
    void setName(string name) { empName = name; }
    void setBasicSalary(double basic) { basicSalary = basic; }

    // ============================================================
    // SALARY CALCULATION FUNCTIONS
    // Demonstrates: Member functions performing calculations
    // ============================================================
    
    // Calculate allowances (percentage of basic salary)
    void calculateAllowances() {
        double hra = basicSalary * 0.25;      // House Rent Allowance: 25%
        double da = basicSalary * 0.15;       // Dearness Allowance: 15%
        double ta = basicSalary * 0.10;       // Travel Allowance: 10%
        double medical = basicSalary * 0.05;  // Medical Allowance: 5%
        
        allowances = hra + da + ta + medical;
    }

    // Calculate deductions (percentage of basic salary)
    void calculateDeductions() {
        double pf = basicSalary * 0.12;       // Provident Fund: 12%
        double tax = basicSalary * 0.10;      // Income Tax: 10%
        double insurance = basicSalary * 0.05; // Insurance: 5%
        
        deductions = pf + tax + insurance;
    }

    // Calculate gross salary (basic + allowances)
    void calculateGrossSalary() {
        grossSalary = basicSalary + allowances;
    }

    // Calculate net salary (gross - deductions)
    void calculateNetSalary() {
        netSalary = grossSalary - deductions;
    }

    // Complete salary calculation
    void calculateSalary() {
        calculateAllowances();
        calculateDeductions();
        calculateGrossSalary();
        calculateNetSalary();
    }

    // ============================================================
    // DISPLAY FUNCTIONS
    // Demonstrates: Formatted output using iomanip
    // ============================================================
    
    // Display pay slip with detailed breakdown
    void displayPaySlip() const {
        cout << "\n" << string(60, '=') << endl;
        cout << "                 PAY SLIP\n";
        cout << string(60, '=') << endl;
        
        // Get current date - FIXED: Correct time syntax
        time_t now = time(0);
        char* date = ctime(&now);
        
        cout << left << setw(20) << "Pay Slip Date:" << date;
        cout << left << setw(20) << "Employee ID:" << empID << endl;
        cout << left << setw(20) << "Employee Name:" << empName << endl;
        cout << string(60, '-') << endl;
        
        // Earnings section
        cout << "EARNINGS:" << endl;
        cout << left << setw(30) << "  Basic Salary" 
             << right << setw(20) << fixed << setprecision(2) << basicSalary << endl;
        cout << left << setw(30) << "  Allowances (Total)" 
             << right << setw(20) << allowances << endl;
        cout << left << setw(30) << "  Gross Salary" 
             << right << setw(20) << grossSalary << endl;
        cout << string(60, '-') << endl;
        
        // Deductions section
        cout << "DEDUCTIONS:" << endl;
        cout << left << setw(30) << "  Deductions (Total)" 
             << right << setw(20) << deductions << endl;
        cout << string(60, '-') << endl;
        
        // Net salary
        cout << left << setw(30) << "NET SALARY (Take Home)" 
             << right << setw(20) << netSalary << endl;
        cout << string(60, '=') << endl;
    }

    // Display brief employee info
    void display() const {
        cout << left << setw(10) << empID 
             << setw(25) << empName 
             << setw(15) << fixed << setprecision(2) << basicSalary
             << setw(15) << grossSalary 
             << setw(15) << netSalary << endl;
    }
};

// ============================================================
// PAYROLL SYSTEM CLASS - Manages all employees
// Demonstrates: Composition, File Handling, Array Management
// ============================================================
class PayrollSystem {
private:
    // VECTOR (DYNAMIC ARRAY) - ARRAYS CONCEPT
    // Stores multiple Employee objects dynamically
    vector<Employee> employees;

public:
    // ============================================================
    // FUNCTIONS - Modular programming
    // ============================================================

    // Function to add new employee
    void addEmployee() {
        cout << "\n--- ADD NEW EMPLOYEE ---\n";
        
        int id;
        string name;
        double basicSalary;
        
        cout << "Enter Employee ID: ";
        cin >> id;
        
        // Check if ID already exists (ARRAY TRAVERSAL)
        for (const auto& emp : employees) {
            if (emp.getID() == id) {
                cout << "Error: Employee ID already exists!\n";
                return;
            }
        }
        
        cin.ignore(); // Clear input buffer
        cout << "Enter Employee Name: ";
        getline(cin, name);
        
        cout << "Enter Basic Salary: $";
        cin >> basicSalary;
        
        // Create new Employee object
        Employee newEmp(id, name, basicSalary);
        newEmp.calculateSalary(); // Calculate all salary components
        
        // Add to vector (ARRAY OPERATION)
        employees.push_back(newEmp);
        
        cout << "\n✅ Employee added successfully!\n";
        newEmp.displayPaySlip();
    }

    // Function to display all employees
    void displayAllEmployees() const {
        if (employees.empty()) {
            cout << "\nNo employees in the system.\n";
            return;
        }
        
        cout << "\n" << string(80, '=') << endl;
        cout << "                      ALL EMPLOYEES\n";
        cout << string(80, '=') << endl;
        cout << left << setw(10) << "ID" 
             << setw(25) << "Name" 
             << setw(15) << "Basic Salary"
             << setw(15) << "Gross Salary" 
             << setw(15) << "Net Salary" << endl;
        cout << string(80, '-') << endl;
        
        // ARRAY TRAVERSAL - Loop through all employees
        for (const auto& emp : employees) {
            emp.display();
        }
        cout << string(80, '=') << endl;
        
        // Display statistics
        displayStatistics();
    }

    // Function to display pay slip for specific employee
    void displayEmployeePaySlip() const {
        if (employees.empty()) {
            cout << "\nNo employees in the system.\n";
            return;
        }
        
        int id;
        cout << "\nEnter Employee ID to view pay slip: ";
        cin >> id;
        
        bool found = false;
        // ARRAY SEARCH - Linear search for employee
        for (const auto& emp : employees) {
            if (emp.getID() == id) {
                emp.displayPaySlip();
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "❌ Employee ID not found!\n";
        }
    }

    // Function to calculate and display statistics
    void displayStatistics() const {
        if (employees.empty()) {
            cout << "No employees for statistics.\n";
            return;
        }
        
        double totalBasic = 0, totalGross = 0, totalNet = 0;
        double highestNet = 0, lowestNet = employees[0].getNetSalary();
        string highestPaid, lowestPaid;
        
        // ARRAY PROCESSING - Calculate statistics
        for (const auto& emp : employees) {
            double net = emp.getNetSalary();
            totalBasic += emp.getBasicSalary();
            totalGross += emp.getGrossSalary();
            totalNet += net;
            
            // Find highest paid
            if (net > highestNet) {
                highestNet = net;
                highestPaid = emp.getName();
            }
            
            // Find lowest paid
            if (net < lowestNet) {
                lowestNet = net;
                lowestPaid = emp.getName();
            }
        }
        
        double avgNet = totalNet / employees.size();
        
        cout << "\n" << string(50, '=') << endl;
        cout << "          PAYROLL STATISTICS\n";
        cout << string(50, '=') << endl;
        cout << left << setw(30) << "Total Employees:" << employees.size() << endl;
        cout << setw(30) << "Total Basic Salary:" << "$" << totalBasic << endl;
        cout << setw(30) << "Total Gross Salary:" << "$" << totalGross << endl;
        cout << setw(30) << "Total Net Salary:" << "$" << totalNet << endl;
        cout << setw(30) << "Average Net Salary:" << "$" << fixed << setprecision(2) << avgNet << endl;
        cout << setw(30) << "Highest Paid:" << highestPaid << " ($" << highestNet << ")" << endl;
        cout << setw(30) << "Lowest Paid:" << lowestPaid << " ($" << lowestNet << ")" << endl;
        cout << string(50, '=') << endl;
    }

    // ============================================================
    // FILE HANDLING FUNCTIONS - Save/Load data
    // Demonstrates: File Input/Output operations
    // ============================================================

    // Function to save employee records to file
    void saveToFile() const {
        // FILE HANDLING: Create output file stream
        ofstream outFile("payroll_data.txt");
        
        if (outFile.is_open()) {
            // Write number of employees first
            outFile << employees.size() << endl;
            
            // Write each employee's data
            for (const auto& emp : employees) {
                outFile << emp.getID() << endl;
                outFile << emp.getName() << endl;
                outFile << emp.getBasicSalary() << endl;
                outFile << emp.getGrossSalary() << endl;
                outFile << emp.getNetSalary() << endl;
            }
            
            outFile.close();
            cout << "\n✅ Data saved to 'payroll_data.txt' successfully!\n";
        } else {
            cout << "\n❌ Error: Unable to save data to file.\n";
        }
    }

    // Function to load employee records from file
    void loadFromFile() {
        // FILE HANDLING: Create input file stream
        ifstream inFile("payroll_data.txt");
        
        if (inFile.is_open()) {
            // Clear existing data
            employees.clear();
            
            int numEmployees;
            inFile >> numEmployees;
            inFile.ignore(); // Skip newline
            
            for (int i = 0; i < numEmployees; i++) {
                int id;
                string name;
                double basic, gross, net;
                
                inFile >> id;
                inFile.ignore();
                getline(inFile, name);
                inFile >> basic >> gross >> net;
                
                // Create employee with loaded data
                Employee emp(id, name, basic);
                employees.push_back(emp);
            }
            
            inFile.close();
            cout << "\n✅ Data loaded from 'payroll_data.txt' successfully!\n";
            cout << "Loaded " << employees.size() << " employee records.\n";
        } else {
            cout << "\nℹ️  No existing data file found. Starting with empty system.\n";
        }
    }

    // Function to export pay slips to file - FIXED: Correct time syntax
    void exportPaySlipsToFile() const {
        if (employees.empty()) {
            cout << "\nNo employees to export.\n";
            return;
        }
        
        // FILE HANDLING: Create output file for pay slips
        ofstream outFile("pay_slips.txt");
        
        if (outFile.is_open()) {
            // Get current time - FIXED: Correct syntax
            time_t now = time(0);
            char* timeStr = ctime(&now);
            
            // Write header
            outFile << string(60, '=') << endl;
            outFile << "             PAYROLL REPORT - ALL EMPLOYEES\n";
            outFile << string(60, '=') << endl;
            outFile << "Generated: " << timeStr << endl;
            
            // Write each employee's pay slip
            for (const auto& emp : employees) {
                outFile << "\nEmployee ID: " << emp.getID() << endl;
                outFile << "Name: " << emp.getName() << endl;
                outFile << "Net Salary: $" << fixed << setprecision(2) << emp.getNetSalary() << endl;
                outFile << string(40, '-') << endl;
            }
            
            outFile.close();
            cout << "\n✅ All pay slips exported to 'pay_slips.txt'\n";
        } else {
            cout << "\n❌ Error creating export file.\n";
        }
    }

    // ============================================================
    // MENU FUNCTIONS
    // ============================================================

    // Function to display main menu
    void displayMenu() const {
        cout << "\n" << string(50, '=') << endl;
        cout << "     PAYROLL MANAGEMENT SYSTEM\n";
        cout << string(50, '=') << endl;
        cout << "1. Add New Employee\n";
        cout << "2. Display All Employees\n";
        cout << "3. Display Employee Pay Slip\n";
        cout << "4. Display Statistics\n";
        cout << "5. Save Data to File\n";
        cout << "6. Load Data from File\n";
        cout << "7. Export All Pay Slips\n";
        cout << "8. Exit System\n";
        cout << string(50, '-') << endl;
        cout << "Enter your choice (1-8): ";
    }

    // Function to clear input buffer - FIXED: Using correct syntax
    void clearInputBuffer() {
        cin.clear();
        // Use the correct way to ignore remaining characters
        cin.ignore(1000, '\n');  // Simplified version that works everywhere
    }
};

// ============================================================
// MAIN FUNCTION - Program Entry Point
// Demonstrates: Program flow control, Object creation
// ============================================================
int main() {
    PayrollSystem payroll;  // Create PayrollSystem object
    int choice;
    
    // Load existing data when program starts
    payroll.loadFromFile();
    
    cout << "\n" << string(60, '=') << endl;
    cout << "  PAYROLL MANAGEMENT SYSTEM SIMULATION\n";
    cout << "   (Runs in VS Code - No Hardware Needed)\n";
    cout << string(60, '=') << endl;
    
    // Main program loop
    do {
        payroll.displayMenu();
        cin >> choice;
        payroll.clearInputBuffer();
        
        // Switch statement for menu choices
        switch (choice) {
            case 1:
                payroll.addEmployee();  // FUNCTION CALL
                break;
            case 2:
                payroll.displayAllEmployees();  // FUNCTION CALL
                break;
            case 3:
                payroll.displayEmployeePaySlip();  // FUNCTION CALL
                break;
            case 4:
                payroll.displayStatistics();  // FUNCTION CALL
                break;
            case 5:
                payroll.saveToFile();  // FILE HANDLING function
                break;
            case 6:
                payroll.loadFromFile();  // FILE HANDLING function
                break;
            case 7:
                payroll.exportPaySlipsToFile();  // FILE HANDLING function
                break;
            case 8:
                cout << "\n✅ Saving data before exit...\n";
                payroll.saveToFile();
                cout << "\nThank you for using Payroll Management System!\n";
                cout << "Goodbye!\n";
                break;
            default:
                cout << "\n❌ Invalid choice! Please enter 1-8.\n";
        }
        
        if (choice != 8) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        
    } while (choice != 8);
    
    return 0;
}