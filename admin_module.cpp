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
int hashCount[TABLE_SIZE] = {0};
Student students[MAX_STUDENTS];
int studentCount = 0;
Student* sortedArray[MAX_STUDENTS];
int arraySize = 0;
bool loggedIn = false;

// Function prototypes
void convertToArray();
int loadStudentsFromFile(const string& filename);
void displayStudents(const Student* student = 0);
void registerNewAdmin();
void logout();
void editStudentRecord();
void deleteStudentRecord();
void searchByStudentName();
void saveSummaryToFile();
void loadSummaryFromFile();
string verifyAdminLogin(bool initial = false);

// Utility functions
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

long long stringToLongLong(const string& str) {
    long long result = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result;
}

float stringToFloat(const string& str) {
    return atof(str.c_str());
}

// Load students from file
int loadStudentsFromFile(const string& filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Error: Unable to open file '" << filename << "'." << endl;
        return 0;
    }

    string line;
    studentCount = 0; // Reset student count, overwriting in-memory data
    int lineNumber = 0;

    cout << "Loading from '" << filename << "'... (This will overwrite any unsaved in-memory data)" << endl;
    if (!getline(file, line)) { file.close(); return 0; }
    lineNumber++;
    if (!getline(file, line)) { file.close(); return 0; }
    lineNumber++;

    while (getline(file, line) && studentCount < MAX_STUDENTS) {
        lineNumber++;
        if (line.empty()) continue;

        stringstream ss(line);
        string fields[7];
        int fieldCount = 0;
        while (getline(ss, fields[fieldCount], '|') && fieldCount < 7) {
            fields[fieldCount] = trim(fields[fieldCount]);
            fieldCount++;
        }

        if (fieldCount != 7) continue;

        try {
            strcpy(students[studentCount].name, fields[0].c_str());
            students[studentCount].studentId = stringToLongLong(fields[1]);
            strcpy(students[studentCount].email, fields[2].c_str());
            students[studentCount].cgpa = stringToFloat(fields[3]);
            strcpy(students[studentCount].diploma, fields[4].c_str());
            strcpy(students[studentCount].skills, fields[5].c_str());
            strcpy(students[studentCount].appliedJob, fields[6].c_str());

            if (students[studentCount].studentId == 0 || students[studentCount].cgpa < 0.0 || students[studentCount].cgpa > 4.0) continue;

            strcpy(students[studentCount].status, "Active");
            int index = students[studentCount].studentId % TABLE_SIZE;
            if (hashCount[index] < MAX_STUDENTS) hashTable[index][hashCount[index]++] = studentCount;
            studentCount++;
        } catch (...) { continue; }
    }

    file.close();
    cout << "Loaded " << studentCount << " students." << endl;
    return studentCount;
}

// Display students
void displayStudents(const Student* student) {
    const int NAME_WIDTH = 20;
    const int EMAIL_WIDTH = 30;
    const int DIPLOMA_WIDTH = 15;
    const int SKILLS_WIDTH = 40;
    const int JOB_WIDTH = 25;
    const int STATUS_WIDTH = 10;

    if (student == 0) {
        cout << "\n--- Student Records ---" << endl;
        cout << left << setw(NAME_WIDTH) << "Name"
             << setw(12) << "ID"
             << setw(EMAIL_WIDTH) << "Email"
             << setw(8) << "CGPA"
             << setw(DIPLOMA_WIDTH) << "Diploma"
             << setw(SKILLS_WIDTH) << "Skills"
             << setw(JOB_WIDTH) << "Applied Job"
             << setw(STATUS_WIDTH) << "Status" << endl;
        cout << string(NAME_WIDTH + 12 + EMAIL_WIDTH + 8 + DIPLOMA_WIDTH + SKILLS_WIDTH + JOB_WIDTH + STATUS_WIDTH, '-') << endl;
    }

    if (student != 0) {
        cout << left << setw(NAME_WIDTH) << student->name
             << setw(12) << student->studentId
             << setw(EMAIL_WIDTH) << student->email
             << setw(8) << fixed << setprecision(2) << student->cgpa
             << setw(DIPLOMA_WIDTH) << student->diploma
             << setw(SKILLS_WIDTH) << student->skills
             << setw(JOB_WIDTH) << student->appliedJob
             << setw(STATUS_WIDTH) << student->status << endl;
    } else if (studentCount == 0) {
        cout << "No records loaded." << endl;
        return;
    } else {
        for (int i = 0; i < studentCount; i++) displayStudents(&students[i]);
    }
}

// Register new admin
void registerNewAdmin() {
    string username, password, confirmPassword;
    cout << "Enter new admin username: "; cin >> username;
    cout << "Enter new admin password: "; cin >> password;
    cout << "Confirm new admin password: "; cin >> confirmPassword;

    if (password != confirmPassword) {
        cout << "Error: Passwords do not match." << endl;
        return;
    }

    ofstream file("login_admin.txt", ios::app);
    if (file.is_open()) {
        file << username << "," << password << endl;
        file.close();
        cout << "Admin registered successfully." << endl;
    } else {
        cout << "Error: Could not open login_admin.txt." << endl;
    }
}

// Logout
void logout() {
    loggedIn = false;
    cout << "Logged out successfully." << endl;
}

// Edit student record
void editStudentRecord() {
    if (studentCount == 0) {
        cout << "No records to edit. Load data first." << endl;
        return;
    }
    long long id;
    cout << "Enter Student ID to edit: "; cin >> id;
    int studentIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentId == id) {
            studentIndex = i;
            break;
        }
    }
    if (studentIndex == -1) {
        cout << "Student not found." << endl;
        return;
    }

    int choice;
    do {
        cout << "\n--- Edit Student Record ---" << endl;
        cout << "Current Record:" << endl;
        displayStudents(&students[studentIndex]);
        cout << "\nSelect field to edit:" << endl;
        cout << "1. Name" << endl;
        cout << "2. Email" << endl;
        cout << "3. CGPA" << endl;
        cout << "4. Diploma" << endl;
        cout << "5. Skills" << endl;
        cout << "6. Applied Job" << endl;
        cout << "0. Finish Editing" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        string input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
        switch (choice) {
            case 1:
                cout << "Enter new Name: "; getline(cin, input); strcpy(students[studentIndex].name, trim(input).c_str());
                break;
            case 2:
                cout << "Enter new Email: "; getline(cin, input); strcpy(students[studentIndex].email, trim(input).c_str());
                break;
            case 3:
                cout << "Enter new CGPA: "; cin >> students[studentIndex].cgpa;
                if (cin.fail() || students[studentIndex].cgpa < 0.0 || students[studentIndex].cgpa > 4.0) {
                    cout << "Invalid CGPA. Must be between 0.0 and 4.0." << endl;
                    students[studentIndex].cgpa = 0.0; // Reset on invalid input
                }
                cin.ignore();
                break;
            case 4:
                cout << "Enter new Diploma: "; getline(cin, input); strcpy(students[studentIndex].diploma, trim(input).c_str());
                break;
            case 5:
                cout << "Enter new Skills: "; getline(cin, input); strcpy(students[studentIndex].skills, trim(input).c_str());
                break;
            case 6:
                cout << "Enter new Applied Job: "; getline(cin, input); strcpy(students[studentIndex].appliedJob, trim(input).c_str());
                break;
            case 0:
                cout << "Student updated." << endl;
                break;
            default:
                cout << "Invalid option." << endl;
        }
    } while (choice != 0);
}

// Delete student record
void deleteStudentRecord() {
    if (studentCount == 0) {
        cout << "No records to delete. Load data first." << endl;
        return;
    }
    long long id;
    cout << "Enter Student ID to delete: "; cin >> id;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentId == id) {
            for (int j = i; j < studentCount - 1; j++) students[j] = students[j + 1];
            studentCount--;
            cout << "Student deleted." << endl;
            return;
        }
    }
    cout << "Student not found." << endl;
}

// Search by student name
void searchByStudentName() {
    if (studentCount == 0) {
        cout << "No records to search. Load data first." << endl;
        return;
    }
    string name;
    cin.ignore();
    cout << "Enter Student Name: "; getline(cin, name);
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].name, trim(name).c_str()) == 0) {
            displayStudents(&students[i]);
            found = true;
        }
    }
    if (!found) cout << "Student not found." << endl;
}

// Save summary to file
void saveSummaryToFile() {
    ofstream file("summary.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open summary.txt." << endl;
        return;
    }
    file << "Total Students: " << studentCount << endl;
    for (int i = 0; i < studentCount; i++) {
        file << students[i].name << " | ID: " << students[i].studentId << " | CGPA: " << fixed << setprecision(2) << students[i].cgpa << endl;
    }
    file.close();
    cout << "Summary saved to summary.txt." << endl;
}

// Load summary from file
void loadSummaryFromFile() {
    ifstream file("summary.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open summary.txt." << endl;
        return;
    }
    string line;
    cout << "Loading summary..." << endl;
    while (getline(file, line)) cout << line << endl;
    file.close();
}

// Convert to array
void convertToArray() {
    arraySize = 0;
    for (int i = 0; i < studentCount; i++) sortedArray[arraySize++] = &students[i];
    cout << "Converted " << arraySize << " students." << endl;
}

// Selection sort by CGPA
void selectionSortByCGPA(Student* arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) if (arr[j]->cgpa > arr[maxIdx]->cgpa) maxIdx = j;
        if (maxIdx != i) swap(arr[i], arr[maxIdx]);
    }
}

// Selection sort by ID
void selectionSortById(Student* arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) if (arr[j]->studentId < arr[minIdx]->studentId) minIdx = j;
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
}

// Binary search by ID
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

// Search by student ID
void searchByStudentId() {
    if (studentCount == 0) {
        cout << "No records to search. Load data first." << endl;
        return;
    }
    long long id;
    cout << "Enter Student ID: "; cin >> id;
    convertToArray();
    selectionSortById(sortedArray, arraySize);
    int result = binarySearchById(sortedArray, arraySize, id);
    if (result != -1) displayStudents(sortedArray[result]);
    else cout << "Student not found." << endl;
}

// Add new student
void addNewStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Maximum capacity reached." << endl;
        return;
    }
    Student newStudent;
    string input;
    cin.ignore();
    cout << "Enter Name: "; getline(cin, input); strcpy(newStudent.name, trim(input).c_str());
    cout << "Enter ID: "; getline(cin, input); newStudent.studentId = stringToLongLong(trim(input));
    cout << "Enter Email: "; getline(cin, input); strcpy(newStudent.email, trim(input).c_str());
    cout << "Enter CGPA: "; cin >> newStudent.cgpa;
    cin.ignore();
    cout << "Enter Diploma: "; getline(cin, input); strcpy(newStudent.diploma, trim(input).c_str());
    cout << "Enter Skills: "; getline(cin, input); strcpy(newStudent.skills, trim(input).c_str());
    cout << "Enter Job: "; getline(cin, input); strcpy(newStudent.appliedJob, trim(input).c_str());
    strcpy(newStudent.status, "Active");

    // Add to in-memory array
    students[studentCount] = newStudent;
    int index = newStudent.studentId % TABLE_SIZE;
    if (hashCount[index] < MAX_STUDENTS) hashTable[index][hashCount[index]++] = studentCount;
    studentCount++;

    // Save to file
    ofstream file("C:\\Users\\USER\\OneDrive\\Documents\\TDS4223-Project-GR1\\raw data.txt", ios::app);
    if (file.is_open()) {
        file << newStudent.name << "|" << newStudent.studentId << "|"
             << newStudent.email << "|" << fixed << setprecision(2) << newStudent.cgpa << "|"
             << newStudent.diploma << "|" << newStudent.skills << "|"
             << newStudent.appliedJob << endl;
        file.close();
        cout << "Student added and saved to raw data.txt." << endl;
    } else {
        cout << "Error: Could not save to raw data.txt. Data added in memory only." << endl;
    }
}

// Display sorted records
void displaySortedData() {
    if (arraySize == 0) {
        cout << "No sorted records. Convert first." << endl;
        return;
    }
    for (int i = 0; i < arraySize; i++) displayStudents(sortedArray[i]);
}

// Save sorted data
void saveSortedData() {
    ofstream out("sorted_information.txt");
    if (!out.is_open()) {
        cout << "Error: Could not open sorted_information.txt." << endl;
        return;
    }
    for (int i = 0; i < arraySize; i++) {
        out << sortedArray[i]->name << "|" << sortedArray[i]->studentId << "|"
            << sortedArray[i]->email << "|" << fixed << setprecision(2) << sortedArray[i]->cgpa << "|"
            << sortedArray[i]->diploma << "|" << sortedArray[i]->skills << "|"
            << sortedArray[i]->appliedJob << endl;
    }
    out.close();
    cout << "Saved to sorted_information.txt." << endl;
}

// Show summary report
void showSummary(int count) {
    cout << "Total records: " << count << endl;
}
void showSummary(float minCgpa) {
    if (arraySize == 0) {
        cout << "No records. Convert first." << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < arraySize; i++) {
        if (sortedArray[i]->cgpa >= minCgpa) {
            displayStudents(sortedArray[i]);
            found = true;
        }
    }
    if (!found) cout << "No students with CGPA >= " << minCgpa << endl;
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
string verifyAdminLogin(bool initial) {
    string username, password;
    cout << "==== Admin Login ====" << endl;
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;

    ifstream file("login_admin.txt");
    if (!file.is_open()) {
        return "Error: Could not open login_admin.txt.";
    }

    string line;
    while (getline(file, line)) {
        size_t delim = line.find(',');
        if (delim != string::npos) {
            string storedUser = trim(line.substr(0, delim));
            string storedPass = trim(line.substr(delim + 1));
            cout << "Checking: " << storedUser << "," << storedPass << endl;
            if (username == storedUser && password == storedPass) {
                file.close();
                loggedIn = true;
                return "Login successful.";
            }
        }
    }
    file.close();
    cout << "No match found for " << username << "." << endl;
    return "Login failed.";
}

// Admin menu
void adminMenu() {
    int choice;
    do {
        clearScreen();
        cout << "==== Internship System - Admin Panel ====" << endl;
        cout << "\n1. Register New Admin" << endl;
        cout << "2. Login/Re-login" << endl;
        cout << "3. Logout" << endl;
        cout << "--------------------------" << endl;
        cout << "4. Load Student Data" << endl;
        cout << "5. Add New Student Record" << endl;
        cout << "6. Edit/Update Student Record" << endl;
        cout << "7. Delete Student Record" << endl;
        cout << "8. Display All Records" << endl;
        cout << "9. Convert Records to Array" << endl;
        cout << "10. Display Sorted Records" << endl;
        cout << "11. Search by Student ID" << endl;
        cout << "12. Search by Student Name" << endl;
        cout << "13. Sort by CGPA" << endl;
        cout << "14. Sort by Student ID" << endl;
        cout << "15. Show Summary Report" << endl;
        cout << "16. Save Summary to File" << endl;
        cout << "17. Load Summary from File" << endl;
        cout << "0. Exit" << endl;
        cout << "Select an option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: registerNewAdmin(); pauseScreen(); break;
            case 2: {
                logout();
                return; // Exit to re-enter login loop in main
            }
            case 3: logout(); pauseScreen(); break;
            case 4: {
                studentCount = loadStudentsFromFile("C:\\Users\\USER\\OneDrive\\Documents\\TDS4223-Project-GR1\\raw data.txt");
                pauseScreen();
                break;
            }
            case 5: addNewStudent(); pauseScreen(); break;
            case 6: editStudentRecord(); pauseScreen(); break;
            case 7: deleteStudentRecord(); pauseScreen(); break;
            case 8: displayStudents(); pauseScreen(); break;
            case 9: convertToArray(); pauseScreen(); break;
            case 10: displaySortedData(); pauseScreen(); break;
            case 11: searchByStudentId(); pauseScreen(); break;
            case 12: searchByStudentName(); pauseScreen(); break;
            case 13: {
                convertToArray();
                selectionSortByCGPA(sortedArray, arraySize);
                displaySortedData();
                pauseScreen();
                break;
            }
            case 14: {
                convertToArray();
                selectionSortById(sortedArray, arraySize);
                displaySortedData();
                pauseScreen();
                break;
            }
            case 15: {
                float minCgpa;
                cout << "Enter minimum CGPA: "; cin >> minCgpa;
                if (cin.fail()) cout << "Invalid input." << endl;
                else showSummary(minCgpa);
                pauseScreen();
                break;
            }
            case 16: saveSummaryToFile(); pauseScreen(); break;
            case 17: loadSummaryFromFile(); pauseScreen(); break;
            case 0: cout << "Exiting..." << endl; break;
            default: cout << "Invalid option." << endl; pauseScreen();
        }
    } while (choice != 0 && loggedIn);
}

// Main function
int main() {
    while (true) {
        string result = verifyAdminLogin(true);
        cout << result << endl;
        if (loggedIn) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseScreen();
            adminMenu();
        } else {
            cout << "Access denied." << endl;
            break; // Exit if login fails initially
        }
    }
    return 0;
}
