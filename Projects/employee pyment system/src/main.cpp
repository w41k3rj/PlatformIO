#include <Arduino.h>

// Use Arduino String class instead of std::string
#define MAX_EMPLOYEES 10  // Limited by Arduino memory
#define MAX_NAME_LENGTH 20
#define FILENAME "/employees.txt"

// Enable simulation mode
#define SIMULATION_MODE

struct Employee {
  int id;
  char name[MAX_NAME_LENGTH + 1];
  float basicSalary;
  float allowances;
  float deductions;
  float grossSalary;
  float netSalary;
};

// Global variables
Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;
bool sdCardReady = false;

// ========== FUNCTION PROTOTYPES ==========
void mainMenu();
void addEmployee();
void calculateSalaries();
void displayAllEmployees();
void displayPaySlip();
int findEmployee(int id);
void saveToSD();
void loadFromSD();
void printMenuDivider();
void printSectionTitle(const char* title);
void processMenuChoice(char choice);
void quickTest();
void initializeSampleData();
// =========================================

void printMenuDivider() {
  Serial.println("-------------------------------------");
}

void printSectionTitle(const char* title) {
  Serial.println("\n=====================================");
  Serial.print("  ");
  Serial.println(title);
  Serial.println("=====================================");
}

void initializeSampleData() {
  // Clear any existing data
  employeeCount = 0;
  
  // Employee 1
  employees[0].id = 1001;
  strncpy(employees[0].name, "John Doe", MAX_NAME_LENGTH);
  employees[0].basicSalary = 5000.00;
  employees[0].allowances = 500.00;
  employees[0].deductions = 200.00;
  employees[0].grossSalary = 5500.00;
  employees[0].netSalary = 5300.00;
  
  // Employee 2
  employees[1].id = 1002;
  strncpy(employees[1].name, "Jane Smith", MAX_NAME_LENGTH);
  employees[1].basicSalary = 6000.00;
  employees[1].allowances = 600.00;
  employees[1].deductions = 300.00;
  employees[1].grossSalary = 6600.00;
  employees[1].netSalary = 6300.00;
  
  // Employee 3
  employees[2].id = 1003;
  strncpy(employees[2].name, "Bob Johnson", MAX_NAME_LENGTH);
  employees[2].basicSalary = 4500.00;
  employees[2].allowances = 400.00;
  employees[2].deductions = 150.00;
  employees[2].grossSalary = 4900.00;
  employees[2].netSalary = 4750.00;
  
  employeeCount = 3;
}

void quickTest() {
  if (employeeCount >= MAX_EMPLOYEES) {
    Serial.println("ERROR: Maximum employees reached!");
    return;
  }
  
  // Add a test employee
  Employee emp;
  emp.id = 2000 + employeeCount;
  snprintf(emp.name, MAX_NAME_LENGTH, "Test Employee %d", employeeCount + 1);
  emp.basicSalary = 3000.00 + (employeeCount * 500);
  emp.allowances = 300.00;
  emp.deductions = 100.00;
  emp.grossSalary = emp.basicSalary + emp.allowances;
  emp.netSalary = emp.grossSalary - emp.deductions;
  
  employees[employeeCount++] = emp;
  Serial.print("Added test employee: ");
  Serial.println(emp.name);
  Serial.print("ID: ");
  Serial.println(emp.id);
  Serial.print("Net Salary: $");
  Serial.println(emp.netSalary, 2);
}

void processMenuChoice(char choice) {
  switch(choice) {
    case '1':
      addEmployee();
      break;
    case '2':
      calculateSalaries();
      break;
    case '3':
      displayAllEmployees();
      break;
    case '4':
      displayPaySlip();
      break;
    case '5':
      saveToSD();
      break;
    case '6':
      loadFromSD();
      break;
    case '7':
      Serial.println("System resetting... Goodbye!");
      delay(2000);
      initializeSampleData();
      Serial.println("\n\nSystem reset complete. Starting fresh...\n");
      break;
    case '8':
      quickTest();
      break;
    case '9':
      Serial.println("Initializing sample data...");
      initializeSampleData();
      Serial.println("Sample data loaded!");
      break;
    default:
      Serial.println("Invalid choice! Please enter 1-9");
  }
}

void setup() {
  Serial.begin(115200);
  
  // In simulation mode, don't wait for Serial connection
  #ifndef SIMULATION_MODE
  while (!Serial) {
    delay(10);
  }
  #endif
  
  delay(2000); // Give time for serial monitor to connect
  
  Serial.println("\n\n=====================================");
  Serial.println("   ESP32-S3 PAYROLL SYSTEM");
  Serial.println("=====================================");
  
  #ifdef SIMULATION_MODE
  Serial.println("Mode: SIMULATION (Running without hardware)");
  Serial.println("No ESP32 required - Pure software simulation");
  #else
  Serial.println("Mode: HARDWARE (Connected to ESP32-S3)");
  #endif
  
  Serial.println("\nInitializing system...");
  
  // Initialize with sample data
  initializeSampleData();
  
  Serial.println("3 sample employees loaded.");
  Serial.println("\nSystem Ready!");
  Serial.println("=====================================\n");
  
  // Small delay before showing menu
  delay(500);
}

void loop() {
  mainMenu();
}

void mainMenu() {
  static bool firstRun = true;
  
  if (firstRun) {
    // Auto-run test sequence on first run
    Serial.println("\n=== AUTO-DEMO SEQUENCE STARTING ===\n");
    delay(1000);
    
    // Demo sequence
    char demoSequence[] = {'3', '4', '2', '3', '8', '3', '5', '6'};
    
    for (int i = 0; i < sizeof(demoSequence); i++) {
      Serial.print("\n[Demo Step ");
      Serial.print(i + 1);
      Serial.print(" of ");
      Serial.print(sizeof(demoSequence));
      Serial.println("]");
      delay(500);
      
      processMenuChoice(demoSequence[i]);
      delay(1500);
    }
    
    Serial.println("\n=== AUTO-DEMO COMPLETE ===");
    Serial.println("Now entering interactive mode...\n");
    delay(1000);
    firstRun = false;
  }
  
  while (true) {
    printSectionTitle("MAIN MENU");
    Serial.println("1. Add New Employee");
    Serial.println("2. Calculate All Salaries");
    Serial.println("3. Display All Employees");
    Serial.println("4. Display Pay Slip");
    Serial.println("5. Save to SD (Simulated)");
    Serial.println("6. Load from SD (Simulated)");
    Serial.println("7. Reset System");
    Serial.println("8. Quick Test (Add Sample)");
    Serial.println("9. Reload Sample Data");
    printMenuDivider();
    Serial.print("Enter choice (1-9): ");
    
    // For simulation, allow manual input
    unsigned long startTime = millis();
    bool inputReceived = false;
    
    while (millis() - startTime < 30000) { // 30 second timeout
      if (Serial.available() > 0) {
        char choice = Serial.read();
        // Echo the choice
        Serial.println(choice);
        processMenuChoice(choice);
        inputReceived = true;
        break;
      }
      delay(10);
    }
    
    if (!inputReceived) {
      Serial.println("\nNo input received. Displaying employee list...");
      displayAllEmployees();
      delay(2000);
    }
    
    delay(500); // Small pause before showing menu again
  }
}

void addEmployee() {
  if (employeeCount >= MAX_EMPLOYEES) {
    Serial.println("ERROR: Maximum employees reached!");
    Serial.print("Current limit: ");
    Serial.println(MAX_EMPLOYEES);
    return;
  }
  
  printSectionTitle("ADD EMPLOYEE");
  
  #ifdef SIMULATION_MODE
  // Auto-generate employee for simulation
  Employee emp;
  emp.id = 1000 + employeeCount + 4; // Start after sample data
  snprintf(emp.name, MAX_NAME_LENGTH, "Employee %d", employeeCount + 1);
  emp.basicSalary = 4000.00 + (employeeCount * 200);
  emp.allowances = emp.basicSalary * 0.1;  // 10%
  emp.deductions = emp.basicSalary * 0.05; // 5%
  emp.grossSalary = emp.basicSalary + emp.allowances;
  emp.netSalary = emp.grossSalary - emp.deductions;
  
  employees[employeeCount++] = emp;
  
  Serial.println("=== EMPLOYEE ADDED SUCCESSFULLY ===");
  Serial.print("ID: ");
  Serial.println(emp.id);
  Serial.print("Name: ");
  Serial.println(emp.name);
  Serial.print("Basic Salary: $");
  Serial.println(emp.basicSalary, 2);
  Serial.print("Net Salary: $");
  Serial.println(emp.netSalary, 2);
  Serial.println("=====================================");
  #else
  // Original manual input code for hardware
  Serial.print("Enter Employee ID: ");
  while (!Serial.available());
  int id = Serial.parseInt();
  Serial.println(id);
  
  if (findEmployee(id) != -1) {
    Serial.println("ERROR: ID already exists!");
    return;
  }
  
  while (Serial.read() != '\n');
  
  Serial.print("Enter Name: ");
  char name[MAX_NAME_LENGTH + 1];
  int i = 0;
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        name[i] = '\0';
        break;
      }
      if (i < MAX_NAME_LENGTH) {
        name[i++] = c;
      }
    }
  }
  Serial.println(name);
  
  Serial.print("Enter Basic Salary: $");
  while (!Serial.available());
  float salary = Serial.parseFloat();
  Serial.println(salary, 2);
  
  Employee emp;
  emp.id = id;
  strncpy(emp.name, name, MAX_NAME_LENGTH);
  emp.basicSalary = salary;
  emp.allowances = 0;
  emp.deductions = 0;
  emp.grossSalary = 0;
  emp.netSalary = 0;
  
  employees[employeeCount++] = emp;
  Serial.println("Employee added successfully!");
  #endif
}

void calculateSalaries() {
  if (employeeCount == 0) {
    Serial.println("No employees to calculate!");
    return;
  }
  
  printSectionTitle("CALCULATE SALARIES");
  Serial.print("Calculating salaries for ");
  Serial.print(employeeCount);
  Serial.println(" employees...\n");
  
  for (int i = 0; i < employeeCount; i++) {
    Serial.print("Employee ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(employees[i].name);
    Serial.print(" (ID: ");
    Serial.print(employees[i].id);
    Serial.println(")");
    
    #ifdef SIMULATION_MODE
    // Auto-calculate for simulation
    if (employees[i].allowances == 0 && employees[i].deductions == 0) {
      employees[i].allowances = employees[i].basicSalary * 0.1;  // 10% of basic
      employees[i].deductions = employees[i].basicSalary * 0.05; // 5% of basic
    }
    #else
    // Original manual input
    Serial.print("Enter allowances for ");
    Serial.print(employees[i].name);
    Serial.print(": $");
    while (!Serial.available());
    float allowances = Serial.parseFloat();
    Serial.println(allowances, 2);
    employees[i].allowances = allowances;
    
    Serial.print("Enter deductions for ");
    Serial.print(employees[i].name);
    Serial.print(": $");
    while (!Serial.available());
    float deductions = Serial.parseFloat();
    Serial.println(deductions, 2);
    employees[i].deductions = deductions;
    #endif
    
    // Calculate
    employees[i].grossSalary = employees[i].basicSalary + employees[i].allowances;
    employees[i].netSalary = employees[i].grossSalary - employees[i].deductions;
    
    Serial.print("  Basic: $");
    Serial.print(employees[i].basicSalary, 2);
    Serial.print(" + Allowances: $");
    Serial.print(employees[i].allowances, 2);
    Serial.print(" - Deductions: $");
    Serial.print(employees[i].deductions, 2);
    Serial.print(" = Net: $");
    Serial.println(employees[i].netSalary, 2);
    printMenuDivider();
  }
  
  Serial.println("\n✅ All salaries calculated successfully!");
  
  // Show summary
  float totalNet = 0;
  for (int i = 0; i < employeeCount; i++) {
    totalNet += employees[i].netSalary;
  }
  Serial.print("Total monthly payroll: $");
  Serial.println(totalNet, 2);
}

void displayAllEmployees() {
  if (employeeCount == 0) {
    Serial.println("No employees in system!");
    return;
  }
  
  printSectionTitle("EMPLOYEE LIST");
  Serial.print("Total Employees: ");
  Serial.println(employeeCount);
  
  // Table header
  Serial.println("+--------+----------------------+--------------+--------------+");
  Serial.println("|   ID   |        Name          | Basic Salary |  Net Salary  |");
  Serial.println("+--------+----------------------+--------------+--------------+");
  
  for (int i = 0; i < employeeCount; i++) {
    Serial.print("| ");
    Serial.print(employees[i].id);
    
    // Adjust spacing for ID
    if (employees[i].id < 1000) Serial.print("    ");
    else if (employees[i].id < 10000) Serial.print("   ");
    
    Serial.print(" | ");
    
    // Name with padding
    Serial.print(employees[i].name);
    int nameLen = strlen(employees[i].name);
    for (int j = nameLen; j < 20; j++) {
      Serial.print(" ");
    }
    
    Serial.print(" | $");
    
    // Basic salary with formatting
    String basicStr = String(employees[i].basicSalary, 2);
    Serial.print(basicStr);
    int spaces = 11 - basicStr.length();
    for (int j = 0; j < spaces; j++) {
      Serial.print(" ");
    }
    
    Serial.print(" | $");
    
    // Net salary
    String netStr = String(employees[i].netSalary, 2);
    Serial.print(netStr);
    spaces = 11 - netStr.length();
    for (int j = 0; j < spaces; j++) {
      Serial.print(" ");
    }
    
    Serial.println(" |");
  }
  
  Serial.println("+--------+----------------------+--------------+--------------+");
  
  // Summary
  float totalBasic = 0;
  float totalNet = 0;
  for (int i = 0; i < employeeCount; i++) {
    totalBasic += employees[i].basicSalary;
    totalNet += employees[i].netSalary;
  }
  
  Serial.println("\nSUMMARY:");
  Serial.print("Total Basic Salary: $");
  Serial.println(totalBasic, 2);
  Serial.print("Total Allowances: $");
  float totalAllowances = 0;
  for (int i = 0; i < employeeCount; i++) totalAllowances += employees[i].allowances;
  Serial.println(totalAllowances, 2);
  Serial.print("Total Deductions: $");
  float totalDeductions = 0;
  for (int i = 0; i < employeeCount; i++) totalDeductions += employees[i].deductions;
  Serial.println(totalDeductions, 2);
  Serial.print("Total Net Payroll: $");
  Serial.println(totalNet, 2);
}

void displayPaySlip() {
  if (employeeCount == 0) {
    Serial.println("No employees in system!");
    return;
  }
  
  printSectionTitle("PAY SLIP GENERATOR");
  
  #ifdef SIMULATION_MODE
  // Auto-select employee for simulation
  int selectedIndex = 0; // Default to first employee
  Serial.print("Displaying payslip for: ");
  Serial.println(employees[selectedIndex].name);
  #else
  Serial.print("Enter Employee ID: ");
  while (!Serial.available());
  int id = Serial.parseInt();
  Serial.println(id);
  
  int selectedIndex = findEmployee(id);
  if (selectedIndex == -1) {
    Serial.println("Employee not found!");
    return;
  }
  #endif
  
  Employee emp = employees[selectedIndex];
  
  Serial.println("\n══════════════════════════════════════════");
  Serial.println("               PAY SLIP                   ");
  Serial.println("══════════════════════════════════════════");
  Serial.print("Employee ID     : ");
  Serial.println(emp.id);
  Serial.print("Name            : ");
  Serial.println(emp.name);
  Serial.print("Pay Period      : ");
  Serial.println("Monthly");
  Serial.println("══════════════════════════════════════════");
  
  // Earnings
  Serial.println("EARNINGS:");
  Serial.print("  Basic Salary  : $");
  Serial.println(emp.basicSalary, 2);
  Serial.print("  Allowances    : $");
  Serial.println(emp.allowances, 2);
  Serial.print("  ──────────────────────────────\n");
  Serial.print("  GROSS SALARY  : $");
  Serial.println(emp.grossSalary, 2);
  
  // Deductions
  Serial.println("\nDEDUCTIONS:");
  Serial.print("  Total Deduct. : $");
  Serial.println(emp.deductions, 2);
  
  Serial.println("══════════════════════════════════════════");
  
  // Final Net
  Serial.print("NET SALARY      : $");
  Serial.println(emp.netSalary, 2);
  
  // Breakdown
  Serial.println("══════════════════════════════════════════");
  Serial.println("BREAKDOWN:");
  Serial.print("  Hourly Rate   : $");
  Serial.println(emp.netSalary / 160, 2);
  Serial.print("  Daily Rate    : $");
  Serial.println(emp.netSalary / 20, 2);
  Serial.print("  Weekly Rate   : $");
  Serial.println(emp.netSalary / 4, 2);
  
  Serial.println("══════════════════════════════════════════");
  Serial.println("           *** END OF PAY SLIP ***        ");
  Serial.println("══════════════════════════════════════════\n");
}

int findEmployee(int id) {
  for (int i = 0; i < employeeCount; i++) {
    if (employees[i].id == id) {
      return i;
    }
  }
  return -1;
}

void saveToSD() {
  #ifdef SIMULATION_MODE
  printSectionTitle("SAVE TO SD CARD (SIMULATED)");
  Serial.println("Simulating SD card save operation...");
  delay(500);
  
  Serial.println("\n📁 Creating file: /employees.txt");
  Serial.println("📝 Writing data in CSV format...");
  Serial.println("\nFile Contents Preview:");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("ID,Name,BasicSalary,Allowances,Deductions,GrossSalary,NetSalary");
  
  for (int i = 0; i < employeeCount; i++) {
    Serial.print(employees[i].id);
    Serial.print(",");
    Serial.print(employees[i].name);
    Serial.print(",");
    Serial.print(employees[i].basicSalary, 2);
    Serial.print(",");
    Serial.print(employees[i].allowances, 2);
    Serial.print(",");
    Serial.print(employees[i].deductions, 2);
    Serial.print(",");
    Serial.print(employees[i].grossSalary, 2);
    Serial.print(",");
    Serial.println(employees[i].netSalary, 2);
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.print("\n✅ Successfully saved ");
  Serial.print(employeeCount);
  Serial.println(" employee records.");
  Serial.println("File saved to simulated SD card.");
  #else
  Serial.println("SD Card Save - Hardware mode not implemented in simulation");
  #endif
}

void loadFromSD() {
  #ifdef SIMULATION_MODE
  printSectionTitle("LOAD FROM SD CARD (SIMULATED)");
  Serial.println("Simulating SD card load operation...");
  delay(500);
  
  Serial.println("\n📁 Opening file: /employees.txt");
  Serial.println("✅ File found!");
  Serial.println("📖 Reading CSV data...");
  
  // Simulate loading data
  int previousCount = employeeCount;
  
  if (employeeCount < MAX_EMPLOYEES - 2) {
    // Add some simulated loaded employees
    Employee emp1;
    emp1.id = 3001;
    strncpy(emp1.name, "Loaded Employee 1", MAX_NAME_LENGTH);
    emp1.basicSalary = 3500.00;
    emp1.allowances = 350.00;
    emp1.deductions = 175.00;
    emp1.grossSalary = 3850.00;
    emp1.netSalary = 3675.00;
    
    Employee emp2;
    emp2.id = 3002;
    strncpy(emp2.name, "Loaded Employee 2", MAX_NAME_LENGTH);
    emp2.basicSalary = 4200.00;
    emp2.allowances = 420.00;
    emp2.deductions = 210.00;
    emp2.grossSalary = 4620.00;
    emp2.netSalary = 4410.00;
    
    employees[employeeCount++] = emp1;
    employees[employeeCount++] = emp2;
    
    Serial.print("\n✅ Loaded ");
    Serial.print(employeeCount - previousCount);
    Serial.println(" new employees from SD card.");
    Serial.print("Total employees now: ");
    Serial.println(employeeCount);
  } else {
    Serial.println("\n⚠️  Cannot load more employees - maximum limit reached!");
  }
  #else
  Serial.println("SD Card Load - Hardware mode not implemented in simulation");
  #endif
}