/*
ATTENDANCE MANAGEMENT SYSTEM - C++ CODE WITH DETAILED COMMENTS
This program demonstrates: Classes, Arrays (Vectors), File Handling
*/

// ============================================================
// HEADER FILES (INCLUDE LIBRARIES) - WITH COMMENTS
// ============================================================

#include <iostream>       // For input/output operations (cout, cin)
#include <fstream>        // For file handling (ifstream, ofstream) - FILE HANDLING
#include <string>         // For string data type and string operations
#include <vector>         // For dynamic arrays (vector) - ARRAYS CONCEPT
#include <iomanip>        // For output formatting (setw, setprecision)
#include <ctime>          // For time functions (time, ctime)

using namespace std;      // Standard namespace to avoid std:: prefix

// ============================================================
// CLASSES CONCEPT - Student Class Definition
// ============================================================

// Student class to store student information
// Demonstrates: ENCAPSULATION, DATA HIDING, CLASS METHODS
class Student {
private:
    // PRIVATE DATA MEMBERS - Encapsulation principle
    int id;                // Student ID
    string name;           // Student name
    int totalClasses;      // Total classes held
    int attendedClasses;   // Classes attended by student

public:
    // CONSTRUCTOR - Initializes object when created
    // Demonstrates: CONSTRUCTOR OVERLOADING (with default parameters)
    Student(int studentId = 0, string studentName = "", int classes = 0, int attended = 0) {
        id = studentId;
        name = studentName;
        totalClasses = classes;
        attendedClasses = attended;
    }

    // GETTER METHODS - Provide controlled access to private data
    // Demonstrates: ACCESSOR METHODS (const ensures they don't modify object)
    int getId() const { return id; }
    string getName() const { return name; }
    int getTotalClasses() const { return totalClasses; }
    int getAttendedClasses() const { return attendedClasses; }

    // SETTER METHODS - Provide controlled modification of private data
    // Demonstrates: MUTATOR METHODS
    void setId(int studentId) { id = studentId; }
    void setName(string studentName) { name = studentName; }
    void setTotalClasses(int classes) { totalClasses = classes; }
    void setAttendedClasses(int attended) { attendedClasses = attended; }

    // Calculate attendance percentage
    // Demonstrates: MEMBER FUNCTION, TYPE CASTING
    double getAttendancePercentage() const {
        if (totalClasses == 0) return 0.0;
        return (static_cast<double>(attendedClasses) / totalClasses) * 100.0;
    }

    // Mark attendance for a class
    // Demonstrates: MODIFYING MEMBER DATA
    void markAttendance(bool present) {
        totalClasses++;            // Increment total classes
        if (present) {
            attendedClasses++;     // Increment attended if present
        }
    }

    // Display student information
    // Demonstrates: FORMATTED OUTPUT, MEMBER FUNCTION CALLS
    void display() const {
        cout << left << setw(10) << id 
             << setw(25) << name 
             << setw(15) << totalClasses 
             << setw(15) << attendedClasses 
             << setw(15) << fixed << setprecision(2) << getAttendancePercentage() << "%" << endl;
    }
};

// ============================================================
// CLASSES CONCEPT - AttendanceSystem Class Definition
// ============================================================

// Attendance Management System class
// Demonstrates: COMPOSITION, FILE HANDLING, VECTOR OPERATIONS
class AttendanceSystem {
private:
    // VECTOR (DYNAMIC ARRAY) - ARRAYS CONCEPT
    // Demonstrates: VECTOR as a dynamic array to store Student objects
    vector<Student> students;  // Dynamic array of Student objects
    int totalClassDays;        // Total number of class days

public:
    // CONSTRUCTOR - Initializes system and loads data from file
    // Demonstrates: CONSTRUCTOR, FILE HANDLING INITIALIZATION
    AttendanceSystem() : totalClassDays(0) {
        loadFromFile();  // Load existing data when system starts
    }

    // DESTRUCTOR - Saves data to file when system closes
    // Demonstrates: DESTRUCTOR, AUTOMATIC CLEANUP
    ~AttendanceSystem() {
        saveToFile();  // Save data when system ends
    }

    // ============================================================
    // FILE HANDLING CONCEPT - Load data from file
    // ============================================================
    // Demonstrates: ifstream (input file stream), File reading
    void loadFromFile() {
        ifstream inFile("attendance_data.txt");  // Open file for reading
        
        if (inFile.is_open()) {  // Check if file opened successfully
            int numStudents;
            // Read number of students and total class days from file
            inFile >> numStudents >> totalClassDays;
            
            // Loop to read each student's data
            for (int i = 0; i < numStudents; i++) {
                int id, totalClasses, attendedClasses;
                string name;
                
                // Read student data from file
                inFile >> id;
                inFile.ignore();  // Ignore newline character
                getline(inFile, name);  // Read full name (may contain spaces)
                inFile >> totalClasses >> attendedClasses;
                
                // Create Student object and add to vector (ARRAY OPERATION)
                students.push_back(Student(id, name, totalClasses, attendedClasses));
            }
            
            inFile.close();  // Close the file
            cout << "Data loaded successfully!\n";
        } else {
            // File doesn't exist - start fresh
            cout << "No previous data found. Starting fresh.\n";
        }
    }

    // ============================================================
    // FILE HANDLING CONCEPT - Save data to file
    // ============================================================
    // Demonstrates: ofstream (output file stream), File writing
    void saveToFile() {
        ofstream outFile("attendance_data.txt");  // Open file for writing
        
        if (outFile.is_open()) {
            // Write number of students and total class days
            outFile << students.size() << " " << totalClassDays << endl;
            
            // Loop through all students and write their data to file
            for (const auto& student : students) {
                outFile << student.getId() << endl
                       << student.getName() << endl
                       << student.getTotalClasses() << " " 
                       << student.getAttendedClasses() << endl;
            }
            
            outFile.close();  // Close the file
            cout << "Data saved successfully!\n";
        } else {
            cout << "Error: Unable to save data to file.\n";
        }
    }

    // ============================================================
    // VECTOR (ARRAY) OPERATIONS - Register new student
    // ============================================================
    // Demonstrates: Adding elements to array (vector), Searching in array
    void registerStudent() {
        int id;
        string name;
        
        cout << "\n--- Register New Student ---\n";
        cout << "Enter Student ID: ";
        cin >> id;
        
        // LINEAR SEARCH in array - Check if ID already exists
        // Demonstrates: ARRAY TRAVERSAL
        for (const auto& student : students) {
            if (student.getId() == id) {
                cout << "Error: Student ID already exists!\n";
                return;
            }
        }
        
        cin.ignore();  // Clear input buffer
        cout << "Enter Student Name: ";
        getline(cin, name);  // Read full name with spaces
        
        // Create new Student object
        Student newStudent(id, name, totalClassDays, 0);
        
        // ARRAY OPERATION: Add new student to end of vector
        students.push_back(newStudent);
        
        cout << "Student registered successfully!\n";
    }

    // ============================================================
    // ARRAY TRAVERSAL - Mark attendance for all students
    // ============================================================
    // Demonstrates: Iterating through array, Modifying array elements
    void markAttendanceForClass() {
        if (students.empty()) {  // Check if array is empty
            cout << "No students registered yet!\n";
            return;
        }
        
        cout << "\n--- Mark Attendance for Class Day " << (totalClassDays + 1) << " ---\n";
        cout << "Mark 'P' for Present, 'A' for Absent\n\n";
        
        // Loop through all students in array
        // Demonstrates: ARRAY TRAVERSAL with auto reference
        for (auto& student : students) {
            char attendance;
            cout << student.getName() << " (ID: " << student.getId() << "): ";
            cin >> attendance;
            
            // Call Student's member function to mark attendance
            if (toupper(attendance) == 'P') {
                student.markAttendance(true);
            } else {
                student.markAttendance(false);
            }
        }
        
        totalClassDays++;
        cout << "\nAttendance marked for all students!\n";
    }

    // ============================================================
    // ARRAY SEARCHING - Calculate attendance percentage
    // ============================================================
    // Demonstrates: LINEAR SEARCH in array
    void calculateAttendancePercentage() {
        if (students.empty()) {
            cout << "No students registered yet!\n";
            return;
        }
        
        int id;
        cout << "\n--- Calculate Attendance Percentage ---\n";
        cout << "Enter Student ID: ";
        cin >> id;
        
        bool found = false;
        // LINEAR SEARCH: Iterate through array to find student
        for (const auto& student : students) {
            if (student.getId() == id) {
                // Found student - display attendance details
                cout << "\nStudent: " << student.getName() 
                     << " (ID: " << student.getId() << ")\n";
                cout << "Total Classes: " << student.getTotalClasses() << "\n";
                cout << "Classes Attended: " << student.getAttendedClasses() << "\n";
                cout << "Attendance Percentage: " << fixed << setprecision(2) 
                     << student.getAttendancePercentage() << "%\n";
                
                // Show attendance status
                if (student.getAttendancePercentage() >= 75) {
                    cout << "Status: Good Attendance ✓\n";
                } else if (student.getAttendancePercentage() >= 50) {
                    cout << "Status: Warning! Low Attendance ⚠\n";
                } else {
                    cout << "Status: Critical! Very Low Attendance ✗\n";
                }
                
                found = true;
                break;  // Exit loop once found
            }
        }
        
        if (!found) {
            cout << "Error: Student ID not found!\n";
        }
    }

    // ============================================================
    // FILE HANDLING & ARRAY PROCESSING - Generate report
    // ============================================================
    // Demonstrates: Array processing, File output, Time functions
    void generateReport() {
        if (students.empty()) {
            cout << "No students registered yet!\n";
            return;
        }
        
        cout << "\n" << string(80, '=') << "\n";
        cout << "                     ATTENDANCE REPORT\n";
        cout << string(80, '=') << "\n";
        
        // Get current date and time using ctime library
        time_t now = time(0);
        char* dt = ctime(&now);
        cout << "Report Generated: " << dt;
        cout << "Total Class Days: " << totalClassDays << "\n";
        cout << "Total Students: " << students.size() << "\n\n";
        
        // Table header
        cout << left << setw(10) << "ID"
             << setw(25) << "Name"
             << setw(15) << "Total Classes"
             << setw(15) << "Attended"
             << setw(15) << "Percentage"
             << setw(15) << "Status" << endl;
        
        cout << string(80, '-') << endl;
        
        // ARRAY PROCESSING: Display all students
        // Demonstrates: ARRAY TRAVERSAL for reporting
        for (const auto& student : students) {
            cout << left << setw(10) << student.getId()
                 << setw(25) << student.getName()
                 << setw(15) << student.getTotalClasses()
                 << setw(15) << student.getAttendedClasses()
                 << setw(15) << fixed << setprecision(2) << student.getAttendancePercentage() << "%";
            
            // Status indicator
            if (student.getAttendancePercentage() >= 75) {
                cout << setw(15) << "Good ✓";
            } else if (student.getAttendancePercentage() >= 50) {
                cout << setw(15) << "Low ⚠";
            } else {
                cout << setw(15) << "Critical ✗";
            }
            cout << endl;
        }
        
        // ARRAY PROCESSING: Calculate statistics
        // Demonstrates: ARRAY AGGREGATION
        cout << "\n" << string(80, '=') << "\n";
        cout << "SUMMARY:\n";
        
        int good = 0, low = 0, critical = 0;
        for (const auto& student : students) {
            double percentage = student.getAttendancePercentage();
            if (percentage >= 75) good++;
            else if (percentage >= 50) low++;
            else critical++;
        }
        
        cout << "Students with Good Attendance (≥75%): " << good << "\n";
        cout << "Students with Low Attendance (50-74%): " << low << "\n";
        cout << "Students with Critical Attendance (<50%): " << critical << "\n";
        cout << string(80, '=') << "\n";
        
        // Save report to file (FILE HANDLING)
        saveReportToFile();
    }

    // ============================================================
    // FILE HANDLING CONCEPT - Save report to file
    // ============================================================
    // Demonstrates: File output with formatted data
    void saveReportToFile() {
        ofstream reportFile("attendance_report.txt");  // Open report file
        
        if (reportFile.is_open()) {
            // Write formatted report to file
            reportFile << string(80, '=') << "\n";
            reportFile << "                     ATTENDANCE REPORT\n";
            reportFile << string(80, '=') << "\n";
            
            // Write timestamp
            time_t now = time(0);
            char* dt = ctime(&now);
            reportFile << "Report Generated: " << dt;
            reportFile << "Total Class Days: " << totalClassDays << "\n";
            reportFile << "Total Students: " << students.size() << "\n\n";
            
            // Write table header
            reportFile << left << setw(10) << "ID"
                      << setw(25) << "Name"
                      << setw(15) << "Total Classes"
                      << setw(15) << "Attended"
                      << setw(15) << "Percentage"
                      << setw(15) << "Status" << endl;
            
            reportFile << string(80, '-') << endl;
            
            // Write all student data (ARRAY TRAVERSAL for file output)
            for (const auto& student : students) {
                reportFile << left << setw(10) << student.getId()
                          << setw(25) << student.getName()
                          << setw(15) << student.getTotalClasses()
                          << setw(15) << student.getAttendedClasses()
                          << setw(15) << fixed << setprecision(2) << student.getAttendancePercentage() << "%";
                
                // Write status
                if (student.getAttendancePercentage() >= 75) {
                    reportFile << setw(15) << "Good";
                } else if (student.getAttendancePercentage() >= 50) {
                    reportFile << setw(15) << "Low";
                } else {
                    reportFile << setw(15) << "Critical";
                }
                reportFile << endl;
            }
            
            reportFile.close();  // Close file
            cout << "Report also saved to 'attendance_report.txt'\n";
        }
    }

    // ============================================================
    // ARRAY DISPLAY - Show all students
    // ============================================================
    // Demonstrates: ARRAY TRAVERSAL for display
    void displayAllStudents() {
        if (students.empty()) {  // Check if array is empty
            cout << "No students registered yet!\n";
            return;
        }
        
        cout << "\n--- All Registered Students ---\n";
        cout << left << setw(10) << "ID" 
             << setw(25) << "Name" 
             << setw(15) << "Total Classes" 
             << setw(15) << "Attended" 
             << setw(15) << "Percentage" << endl;
        cout << string(70, '-') << endl;
        
        // Loop through array and call each student's display method
        for (const auto& student : students) {
            student.display();  // Polymorphic behavior through member function
        }
    }

    // Display menu
    void displayMenu() {
        cout << "\n" << string(50, '=') << "\n";
        cout << "    ATTENDANCE MANAGEMENT SYSTEM\n";
        cout << string(50, '=') << "\n";
        cout << "1. Register New Student\n";
        cout << "2. Mark Attendance for a Class\n";
        cout << "3. Calculate Attendance Percentage\n";
        cout << "4. Generate Attendance Report\n";
        cout << "5. Display All Students\n";
        cout << "6. Save Data\n";
        cout << "7. Load Data\n";
        cout << "8. Exit\n";
        cout << string(50, '-') << "\n";
        cout << "Enter your choice (1-8): ";
    }
};

// ============================================================
// MAIN FUNCTION - Program Entry Point
// ============================================================
int main() {
    // CLASS CONCEPT: Creating object of AttendanceSystem class
    AttendanceSystem system;  // Constructor is called here
    
    int choice;
    
    cout << "\nWelcome to Attendance Management System Simulation!\n";
    cout << "This program simulates all features without any hardware.\n";
    
    // Main program loop
    do {
        system.displayMenu();  // Call member function
        cin >> choice;
        
        // Switch statement for menu selection
        switch(choice) {
            case 1:
                system.registerStudent();  // ARRAY: Add to vector
                break;
            case 2:
                system.markAttendanceForClass();  // ARRAY: Traverse and modify
                break;
            case 3:
                system.calculateAttendancePercentage();  // ARRAY: Search
                break;
            case 4:
                system.generateReport();  // FILE HANDLING + ARRAY: Process and save
                break;
            case 5:
                system.displayAllStudents();  // ARRAY: Display all elements
                break;
            case 6:
                system.saveToFile();  // FILE HANDLING: Save data
                break;
            case 7:
                system.loadFromFile();  // FILE HANDLING: Load data
                break;
            case 8:
                cout << "\nThank you for using the Attendance Management System!\n";
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        
    } while(choice != 8);
    
    // Destructor is automatically called here to save data
    return 0;
}