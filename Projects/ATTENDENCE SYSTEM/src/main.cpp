#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

// Student class to store student information
class Student {
private:
    int id;
    string name;
    int totalClasses;
    int attendedClasses;

public:
    // Constructor
    Student(int studentId = 0, string studentName = "", int classes = 0, int attended = 0) {
        id = studentId;
        name = studentName;
        totalClasses = classes;
        attendedClasses = attended;
    }

    // Getter methods
    int getId() const { return id; }
    string getName() const { return name; }
    int getTotalClasses() const { return totalClasses; }
    int getAttendedClasses() const { return attendedClasses; }

    // Setter methods
    void setId(int studentId) { id = studentId; }
    void setName(string studentName) { name = studentName; }
    void setTotalClasses(int classes) { totalClasses = classes; }
    void setAttendedClasses(int attended) { attendedClasses = attended; }

    // Calculate attendance percentage
    double getAttendancePercentage() const {
        if (totalClasses == 0) return 0.0;
        return (static_cast<double>(attendedClasses) / totalClasses) * 100.0;
    }

    // Mark attendance for a class
    void markAttendance(bool present) {
        totalClasses++;
        if (present) {
            attendedClasses++;
        }
    }

    // Display student information
    void display() const {
        cout << left << setw(10) << id 
             << setw(25) << name 
             << setw(15) << totalClasses 
             << setw(15) << attendedClasses 
             << setw(15) << fixed << setprecision(2) << getAttendancePercentage() << "%" << endl;
    }
};

// Attendance Management System class
class AttendanceSystem {
private:
    vector<Student> students;
    int totalClassDays;

public:
    AttendanceSystem() : totalClassDays(0) {
        loadFromFile();
    }

    ~AttendanceSystem() {
        saveToFile();
    }

    // Load data from file
    void loadFromFile() {
        ifstream inFile("attendance_data.txt");
        
        if (inFile.is_open()) {
            int numStudents;
            inFile >> numStudents >> totalClassDays;
            
            for (int i = 0; i < numStudents; i++) {
                int id, totalClasses, attendedClasses;
                string name;
                
                inFile >> id;
                inFile.ignore();
                getline(inFile, name);
                inFile >> totalClasses >> attendedClasses;
                
                students.push_back(Student(id, name, totalClasses, attendedClasses));
            }
            
            inFile.close();
            cout << "Data loaded successfully!\n";
        } else {
            cout << "No previous data found. Starting fresh.\n";
        }
    }

    // Save data to file
    void saveToFile() {
        ofstream outFile("attendance_data.txt");
        
        if (outFile.is_open()) {
            outFile << students.size() << " " << totalClassDays << endl;
            
            for (const auto& student : students) {
                outFile << student.getId() << endl
                       << student.getName() << endl
                       << student.getTotalClasses() << " " 
                       << student.getAttendedClasses() << endl;
            }
            
            outFile.close();
            cout << "Data saved successfully!\n";
        } else {
            cout << "Error: Unable to save data to file.\n";
        }
    }

    // Register a new student
    void registerStudent() {
        int id;
        string name;
        
        cout << "\n--- Register New Student ---\n";
        cout << "Enter Student ID: ";
        cin >> id;
        
        // Check if ID already exists
        for (const auto& student : students) {
            if (student.getId() == id) {
                cout << "Error: Student ID already exists!\n";
                return;
            }
        }
        
        cin.ignore(); // Clear input buffer
        cout << "Enter Student Name: ";
        getline(cin, name);
        
        Student newStudent(id, name, totalClassDays, 0);
        students.push_back(newStudent);
        
        cout << "Student registered successfully!\n";
    }

    // Mark attendance for all students for a new class
    void markAttendanceForClass() {
        if (students.empty()) {
            cout << "No students registered yet!\n";
            return;
        }
        
        cout << "\n--- Mark Attendance for Class Day " << (totalClassDays + 1) << " ---\n";
        cout << "Mark 'P' for Present, 'A' for Absent\n\n";
        
        for (auto& student : students) {
            char attendance;
            cout << student.getName() << " (ID: " << student.getId() << "): ";
            cin >> attendance;
            
            if (toupper(attendance) == 'P') {
                student.markAttendance(true);
            } else {
                student.markAttendance(false);
            }
        }
        
        totalClassDays++;
        cout << "\nAttendance marked for all students!\n";
    }

    // Calculate and display attendance percentage for a student
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
        for (const auto& student : students) {
            if (student.getId() == id) {
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
                break;
            }
        }
        
        if (!found) {
            cout << "Error: Student ID not found!\n";
        }
    }

    // Generate attendance report
    void generateReport() {
        if (students.empty()) {
            cout << "No students registered yet!\n";
            return;
        }
        
        cout << "\n" << string(80, '=') << "\n";
        cout << "                     ATTENDANCE REPORT\n";
        cout << string(80, '=') << "\n";
        
        // Get current date and time
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
        
        // Student data
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
        
        // Summary statistics
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
        
        // Save report to file
        saveReportToFile();
    }

    // Save report to a separate file
    void saveReportToFile() {
        ofstream reportFile("attendance_report.txt");
        
        if (reportFile.is_open()) {
            reportFile << string(80, '=') << "\n";
            reportFile << "                     ATTENDANCE REPORT\n";
            reportFile << string(80, '=') << "\n";
            
            time_t now = time(0);
            char* dt = ctime(&now);
            reportFile << "Report Generated: " << dt;
            reportFile << "Total Class Days: " << totalClassDays << "\n";
            reportFile << "Total Students: " << students.size() << "\n\n";
            
            reportFile << left << setw(10) << "ID"
                      << setw(25) << "Name"
                      << setw(15) << "Total Classes"
                      << setw(15) << "Attended"
                      << setw(15) << "Percentage"
                      << setw(15) << "Status" << endl;
            
            reportFile << string(80, '-') << endl;
            
            for (const auto& student : students) {
                reportFile << left << setw(10) << student.getId()
                          << setw(25) << student.getName()
                          << setw(15) << student.getTotalClasses()
                          << setw(15) << student.getAttendedClasses()
                          << setw(15) << fixed << setprecision(2) << student.getAttendancePercentage() << "%";
                
                if (student.getAttendancePercentage() >= 75) {
                    reportFile << setw(15) << "Good";
                } else if (student.getAttendancePercentage() >= 50) {
                    reportFile << setw(15) << "Low";
                } else {
                    reportFile << setw(15) << "Critical";
                }
                reportFile << endl;
            }
            
            reportFile.close();
            cout << "Report also saved to 'attendance_report.txt'\n";
        }
    }

    // Display all registered students
    void displayAllStudents() {
        if (students.empty()) {
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
        
        for (const auto& student : students) {
            student.display();
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

// Main function
int main() {
    AttendanceSystem system;
    int choice;
    
    cout << "\nWelcome to Attendance Management System Simulation!\n";
    cout << "This program simulates all features without any hardware.\n";
    
    do {
        system.displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1:
                system.registerStudent();
                break;
            case 2:
                system.markAttendanceForClass();
                break;
            case 3:
                system.calculateAttendancePercentage();
                break;
            case 4:
                system.generateReport();
                break;
            case 5:
                system.displayAllStudents();
                break;
            case 6:
                system.saveToFile();
                break;
            case 7:
                system.loadFromFile();
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
    
    return 0;
}