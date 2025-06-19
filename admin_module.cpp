#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <limits>
using namespace std;

// Constants
const int MAX_STUDENTS = 100;
const int TABLE_SIZE = 10;

// Student structure
struct Student {
    long long studentId;
    char name[50];
    char email[100];
    float cgpa;
    char diploma[20];
    char skills[200];
    char appliedJob[100];
    char status[20];
};

// Hash table for student indices
int hashTable[TABLE_SIZE][MAX_STUDENTS];
int hashCount[TABLE_SIZE] = {0}; // Initialize to 0
Student students[MAX_STUDENTS];
int studentCount = 0;
Student* sortedArray[MAX_STUDENTS];
int arraySize = 0;

// Function prototypes
void convertToArray();
int loadStudentsFromFile(const string& filename);

// Utility functions
string trim(const string& str) {
    size_t start = str.find_first_not_of(" ");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" ");
    return str.substr(start, end - start + 1);
}

long long stringToLongLong(const string& str) {
    long long result = 0;
    for (char c : str) {
        if (c >= '0' && c <= '9') {
            result = result * 10 + (c - '0');
        }
    }
    return result;
}

float stringToFloat(const string& str) {
    return atof(str.c_str());
}

// Load students from file with debug output
int loadStudentsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file '" << filename << "'. Please check the path and ensure the file exists." << endl;
        return 0;
    }

    string line;
    studentCount = 0;
    int lineNumber = 0;

    cout << "Attempting to load students from '" << filename << "'..." << endl;

    // Skip header lines
    if (!getline(file, line)) {
        cout << "Error: File is empty." << endl;
        file.close();
        return 0;
    }
    lineNumber++;
    if (!getline(file, line)) {
        cout << "Error: Missing second header line." << endl;
        file.close();
        return 0;
    }
    lineNumber++;

    while (getline(file, line) && studentCount < MAX_STUDENTS) {
        lineNumber++;
        if (line.empty()) {
            cout << "Warning: Skipping empty line " << lineNumber << endl;
            continue;
        }

        cout << "Processing line " << lineNumber << ": '" << line << "'" << endl;
        stringstream ss(line);
        string fields[7];
        int fieldCount = 0;

        // Parse fields
        while (getline(ss, fields[fieldCount], '|') && fieldCount < 7) {
            fields[fieldCount] = trim(fields[fieldCount]);
            fieldCount++;
        }

        if (fieldCount != 7) {
            cout << "Error: Line " << lineNumber << " has " << fieldCount << " fields, expected 7. Skipping." << endl;
            continue;
        }

        // Validate and copy fields
        try {
            strcpy(students[studentCount].name, fields[0].c_str());
            students[studentCount].studentId = stringToLongLong(fields[1]);
            strcpy(students[studentCount].email, fields[2].c_str());
            students[studentCount].cgpa = stringToFloat(fields[3]);
            strcpy(students[studentCount].diploma, fields[4].c_str());
            strcpy(students[studentCount].skills, fields[5].c_str());
            strcpy(students[studentCount].appliedJob, fields[6].c_str());

            // Basic validation
            if (students[studentCount].studentId == 0 || students[studentCount].cgpa < 0.0 || students[studentCount].cgpa > 4.0) {
                cout << "Error: Invalid data on line " << lineNumber << ". Skipping." << endl;
                continue;
            }

            strcpy(students[studentCount].status, "Active");

            // Add to hash table
            int index = students[studentCount].studentId % TABLE_SIZE;
            if (hashCount[index] < MAX_STUDENTS) {
                hashTable[index][hashCount[index]++] = studentCount;
            } else {
                cout << "Warning: Hash table bucket " << index << " is full." << endl;
            }

            studentCount++;
            cout << "Loaded student: " << students[studentCount - 1].name << " (ID: " << students[studentCount - 1].studentId << ")" << endl;
        } catch (const exception& e) {
            cout << "Error parsing line " << lineNumber << ": " << e.what() << ". Skipping." << endl;
            continue;
        }
    }

    file.close();
    cout << "Loading complete. Total students loaded: " << studentCount << endl;
    return studentCount;
}

// Display all students
void displayStudents() {
    cout << "\n--- Student Records ---" << endl;
    if (studentCount == 0) {
        cout << "No student records loaded. Please load data first." << endl;
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        cout << i + 1 << ". ";
        cout << students[i].name << " | ID: " << students[i].studentId << " | Email: " << students[i].email
             << " | CGPA: " << fixed << setprecision(2) << students[i].cgpa
             << " | Diploma: " << students[i].diploma << " | Skills: " << students[i].skills
             << " | Job: " << students[i].appliedJob << " | Status: " << students[i].status << endl;
    }
}

// Selection sort by CGPA (highest to lowest)
void selectionSortByCGPA(Student* arr[], int n) {
    cout << "Sorting by CGPA..." << endl;
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j]->cgpa > arr[maxIdx]->cgpa) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            swap(arr[i], arr[maxIdx]);
        }
    }
    cout << "Sorting by CGPA complete!" << endl;
}

// Selection sort by Student ID (lowest to highest)
void selectionSortById(Student* arr[], int n) {
    cout << "Sorting by Student ID..." << endl;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j]->studentId < arr[minIdx]->studentId) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
        }
    }
    cout << "Sorting by ID complete!" << endl;
}

// Binary search by Student ID
int binarySearchById(Student* arr[], int n, long long targetId) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid]->studentId == targetId) return mid;
        if (arr[mid]->studentId < targetId) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Search student by ID
void searchByStudentId() {
    long long targetId;
    cout << "\n=== Search Student by ID ===" << endl;
    cout << "Enter Student ID: ";
    cin >> targetId;

    convertToArray();
    selectionSortById(sortedArray, arraySize);
    int result = binarySearchById(sortedArray, arraySize, targetId);

    if (result != -1) {
        cout << "\n--- STUDENT FOUND ---" << endl;
        displayStudents(&sortedArray[result], 1);
    } else {
        cout << "Student not found!" << endl;
    }
}

// Add new student
void addNewStudent() {
    try {
        if (studentCount >= MAX_STUDENTS) {
            throw runtime_error("Maximum student capacity reached!");
        }

        Student newStudent;
        string input;

        cin.ignore();
        cout << "Enter Name: "; getline(cin, input);
        strcpy(newStudent.name, trim(input).c_str());

        cout << "Enter Student ID: "; getline(cin, input);
        newStudent.studentId = stringToLongLong(trim(input));

        cout << "Enter Email: "; getline(cin, input);
        strcpy(newStudent.email, trim(input).c_str());

        cout << "Enter CGPA: "; cin >> newStudent.cgpa;
        if (cin.fail() || newStudent.cgpa < 0.0 || newStudent.cgpa > 4.0) throw invalid_argument("Invalid CGPA input.");

        cin.ignore();
        cout << "Enter Diploma: "; getline(cin, input);
        strcpy(newStudent.diploma, trim(input).c_str());

        cout << "Enter Skills: "; getline(cin, input);
        strcpy(newStudent.skills, trim(input).c_str());

        cout << "Enter Applied Job: "; getline(cin, input);
        strcpy(newStudent.appliedJob, trim(input).c_str());

        strcpy(newStudent.status, "Active");

        students[studentCount] = newStudent;
        int index = newStudent.studentId % TABLE_SIZE;
        if (hashCount[index] < MAX_STUDENTS) {
            hashTable[index][hashCount[index]++] = studentCount;
        }
        studentCount++;

        cout << "Student added successfully: " << newStudent.name << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Convert to array for sorting
void convertToArray() {
    arraySize = 0;
    for (int i = 0; i < studentCount; i++) {
        sortedArray[arraySize++] = &students[i];
    }
    cout << "Converted " << arraySize << " students to array." << endl;
}

// Display sorted data
void displaySortedData() {
    cout << "\n--- Sorted Student Records ---" << endl;
    if (arraySize == 0) {
        cout << "No sorted records available. Please convert or load data first." << endl;
        return;
    }
    for (int i = 0; i < arraySize; i++) {
        cout << i + 1 << ". ";
        displayStudents(sortedArray[i], 1);
    }
}

// Save sorted data
void saveSortedData() {
    ofstream out("sorted_information.txt");
    if (!out.is_open()) {
        cout << "Error: Unable to open 'sorted_information.txt' for writing." << endl;
        return;
    }
    for (int i = 0; i < arraySize; i++) {
        out << sortedArray[i]->name << "|" << sortedArray[i]->studentId << "|"
            << sortedArray[i]->email << "|" << fixed << setprecision(2) << sortedArray[i]->cgpa << "|"
            << sortedArray[i]->diploma << "|" << sortedArray[i]->skills << "|"
            << sortedArray[i]->appliedJob << endl;
    }
    out.close();
    cout << "Saved " << arraySize << " students to 'sorted_information.txt'." << endl;
}

// Show summary
void showSummary(int count) {
    cout << "Total records in system: " << count << endl;
}
void showSummary(float minCgpa) {
    cout << "Listing students with CGPA >= " << minCgpa << ":" << endl;
    if (arraySize == 0) {
        cout << "No records available. Please convert or load data first." << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < arraySize; i++) {
        if (sortedArray[i]->cgpa >= minCgpa) {
            displayStudents(sortedArray[i], 1);
            found = true;
        }
    }
    if (!found) cout << "No students found with CGPA >= " << minCgpa << "." << endl;
}

// Clear screen
void clearScreen() {
    system("cls");
}

// Pause screen
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Verify admin login
bool verifyAdminLogin() {
    string username, password;
    cout << "==== Admin Login ====" << endl;
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;

    ifstream file("login_admin.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open 'login_admin.txt'!" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        size_t delim = line.find(',');
        if (delim != string::npos) {
            string storedUser = trim(line.substr(0, delim));
            string storedPass = trim(line.substr(delim + 1));
            if (username == storedUser && password == storedPass) {
                file.close();
                cout << "Login successful." << endl;
                return true;
            }
        }
    }
    file.close();
    cout << "Invalid username or password." << endl;
    return false;
}

// Admin menu
void adminMenu() {
    string filePath;
    cout << "Enter the full path to 'raw data.txt' (e.g., C:\\path\\to\\raw data.txt): ";
    cin >> filePath;

    int choice;
    do {
        clearScreen();
        cout << "==== Internship System - Admin Panel ====" << endl;
        cout << "1. Load Student Data" << endl;
        cout << "2. Display All Records" << endl;
        cout << "3. Add New Student" << endl;
        cout << "4. Convert to Array" << endl;
        cout << "5. Display Sorted Data" << endl;
        cout << "6. Save Sorted Data" << endl;
        cout << "7. Show Summary (Total Records)" << endl;
        cout << "8. Show Summary (CGPA >= X)" << endl;
        cout << "9. Sort by CGPA" << endl;
        cout << "10. Sort by Student ID" << endl;
        cout << "11. Search by Student ID" << endl;
        cout << "0. Exit" << endl;
        cout << "Select an option: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1:
                // Disable clearScreen to see debug output
                studentCount = loadStudentsFromFile(filePath);
                pauseScreen();
                break;
            case 2:
                clearScreen();
                displayStudents();
                pauseScreen();
                break;
            case 3:
                clearScreen();
                addNewStudent();
                pauseScreen();
                break;
            case 4:
                clearScreen();
                convertToArray();
                pauseScreen();
                break;
            case 5:
                clearScreen();
                displaySortedData();
                pauseScreen();
                break;
            case 6:
                clearScreen();
                saveSortedData();
                pauseScreen();
                break;
            case 7:
                clearScreen();
                showSummary(studentCount);
                pauseScreen();
                break;
            case 8: {
                clearScreen();
                float minCgpa;
                cout << "Enter minimum CGPA: ";
                cin >> minCgpa;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid CGPA input." << endl;
                } else {
                    convertToArray();
                    showSummary(minCgpa);
                }
                pauseScreen();
                break;
            }
            case 9:
                clearScreen();
                convertToArray();
                selectionSortByCGPA(sortedArray, arraySize);
                displaySortedData();
                pauseScreen();
                break;
            case 10:
                clearScreen();
                convertToArray();
                selectionSortById(sortedArray, arraySize);
                displaySortedData();
                pauseScreen();
                break;
            case 11:
                clearScreen();
                searchByStudentId();
                pauseScreen();
                break;
            case 0:
                clearScreen();
                cout << "Exiting admin module..." << endl;
                break;
            default:
                clearScreen();
                cout << "Invalid option!" << endl;
                pauseScreen();
        }
    } while (choice != 0);
}

// Main function
int main() {
    if (verifyAdminLogin()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        pauseScreen();
        adminMenu();
    } else {
        cout << "Access denied." << endl;
    }
    return 0;
}
