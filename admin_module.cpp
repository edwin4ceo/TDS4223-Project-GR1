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

// Student struct definition
struct Student {
    long long studentID;
    char name[50];
    char email[100];
    float cgpa;
    char diploma[20];
    char skills[200];
    char appliedJob[100];
    char status[20];
};

// Hash table to store indices of students
int hashTable[TABLE_SIZE][MAX_STUDENTS];
int hashCount[TABLE_SIZE]; // Number of students at each index
Student students[MAX_STUDENTS]; // Main student array
int studentCount = 0; // Number of students loaded
Student* sortedArray[MAX_STUDENTS]; // Pointers for sorting
int arraySize = 0;

// Function prototypes
void convertToArray();

// Remove spaces from string
string trim(string str) {
    while (str.length() > 0 && str[0] == ' ') {
        str = str.substr(1);
    }
    while (str.length() > 0 && str[str.length()-1] == ' ') {
        str = str.substr(0, str.length()-1);
    }
    return str;
}

// Convert string to long long
long long stringToLongLong(string str) {
    long long result = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result;
}

// Convert string to float
float stringToFloat(string str) {
    return atof(str.c_str());
}

// Load students from raw data.txt
int loadStudentsFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file '" << filename << "'! Check if file exists in the correct directory." << endl;
        return 0;
    }
    
    string line;
    studentCount = 0;
    
    // Skip header lines
    if (!getline(file, line)) {
        cout << "Error: File is empty or cannot read header." << endl;
        file.close();
        return 0;
    }
    if (!getline(file, line)) {
        cout << "Error: Cannot read second header line." << endl;
        file.close();
        return 0;
    }
    
    cout << "Loading students from '" << filename << "'..." << endl;
    
    int lineNumber = 2; // Track line number for debugging
    while (getline(file, line) && studentCount < MAX_STUDENTS) {
        lineNumber++;
        if (line.empty()) {
            cout << "Skipping empty line at line " << lineNumber << endl;
            continue;
        }
        
        stringstream ss(line);
        string token;
        string fields[7]; // Store name, ID, email, CGPA, diploma, skills, job
        int fieldCount = 0;
        
        // Parse all fields
        while (getline(ss, token, '|') && fieldCount < 7) {
            fields[fieldCount++] = trim(token);
        }
        
        // Check if all required fields were parsed
        if (fieldCount != 7) {
            cout << "Error: Line " << lineNumber << " has " << fieldCount << " fields, expected 7. Skipping: " << line << endl;
            continue;
        }
        
        // Validate fields
        try {
            // Name
            if (fields[0].empty()) {
                cout << "Error: Line " << lineNumber << " has empty name. Skipping." << endl;
                continue;
            }
            strcpy(students[studentCount].name, fields[0].c_str());
            
            // Student ID
            if (fields[1].empty()) {
                cout << "Error: Line " << lineNumber << " has empty student ID. Skipping." << endl;
                continue;
            }
            students[studentCount].studentID = stringToLongLong(fields[1]);
            if (students[studentCount].studentID == 0) {
                cout << "Error: Line " << lineNumber << " has invalid student ID. Skipping." << endl;
                continue;
            }
            
            // Email
            if (fields[2].empty()) {
                cout << "Error: Line " << lineNumber << " has empty email. Skipping." << endl;
                continue;
            }
            strcpy(students[studentCount].email, fields[2].c_str());
            
            // CGPA
            students[studentCount].cgpa = stringToFloat(fields[3]);
            if (fields[3].empty() || students[studentCount].cgpa < 0.0 || students[studentCount].cgpa > 4.0) {
                cout << "Error: Line " << lineNumber << " has invalid CGPA: " << fields[3] << ". Skipping." << endl;
                continue;
            }
            
            // Diploma
            if (fields[4].empty()) {
                cout << "Error: Line " << lineNumber << " has empty diploma. Skipping." << endl;
                continue;
            }
            strcpy(students[studentCount].diploma, fields[4].c_str());
            
            // Skills
            if (fields[5].empty()) {
                cout << "Error: Line " << lineNumber << " has empty skills. Skipping." << endl;
                continue;
            }
            strcpy(students[studentCount].skills, fields[5].c_str());
            
            // Applied Job
            if (fields[6].empty()) {
                cout << "Error: Line " << lineNumber << " has empty applied job. Skipping." << endl;
                continue;
            }
            strcpy(students[studentCount].appliedJob, fields[6].c_str());
            
            // Set default status
            strcpy(students[studentCount].status, "Active");
            
            // Insert into hash table
            int index = students[studentCount].studentID % TABLE_SIZE;
            if (hashCount[index] < MAX_STUDENTS) {
                hashTable[index][hashCount[index]++] = studentCount;
            } else {
                cout << "Warning: Hash table bucket " << index << " is full. Student ID " << students[studentCount].studentID << " not added to hash table." << endl;
            }
            
            studentCount++;
            cout << "Successfully loaded student at line " << lineNumber << ": " << students[studentCount-1].name << endl;
            
        } catch (exception& e) {
            cout << "Error parsing line " << lineNumber << ": " << e.what() << ". Skipping." << endl;
            continue;
        }
    }
    
    file.close();
    cout << "Loaded " << studentCount << " students successfully from '" << filename << "'!" << endl;
    return studentCount;
}

// Print single student
void printStudent(const Student* s) {
    cout << s->name << " | ID: " << s->studentID << " | Email: " << s->email 
         << " | CGPA: " << fixed << setprecision(2) << s->cgpa 
         << " | Diploma: " << s->diploma << " | Skills: " << s->skills 
         << " | Job: " << s->appliedJob << " | Status: " << s->status << endl;
}

// Display all students
void displayStudents() {
    cout << "\n--- Student Records ---" << endl;
    if (studentCount == 0) {
        cout << "No student records loaded. Please load data first." << endl;
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        cout << i+1 << ". ";
        printStudent(&students[i]);
    }
}

// Selection Sort by CGPA (Highest to Lowest)
void selectionSortByCGPA(Student* arr[], int n) {
    cout << "Sorting by CGPA..." << endl;
    for (int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j]->cgpa > arr[maxIndex]->cgpa) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            Student* temp = arr[i];
            arr[i] = arr[maxIndex];
            arr[maxIndex] = temp;
        }
    }
    cout << "Sorted by CGPA complete!" << endl;
}

// Selection Sort by Student ID (Lowest to Highest)
void selectionSortByID(Student* arr[], int n) {
    cout << "Sorting by Student ID..." << endl;
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j]->studentID < arr[minIndex]->studentID) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Student* temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
    cout << "Sorted by ID complete!" << endl;
}

// Binary Search by Student ID
int binarySearchByID(Student* arr[], int n, long long targetID) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid]->studentID == targetID) {
            return mid;
        }
        if (arr[mid]->studentID < targetID) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Search Student by ID
void searchByStudentID() {
    long long targetID;
    cout << "\n=== Search Student by ID ===" << endl;
    cout << "Enter Student ID: ";
    cin >> targetID;
    
    // Convert to array and sort by ID
    convertToArray();
    selectionSortByID(sortedArray, arraySize);
    
    // Search using binary search
    int result = binarySearchByID(sortedArray, arraySize, targetID);
    
    if (result != -1) {
        cout << "\n--- STUDENT FOUND ---" << endl;
        printStudent(sortedArray[result]);
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
        input = trim(input);
        strcpy(newStudent.name, input.c_str());
        
        cout << "Enter Student ID: "; getline(cin, input);
        input = trim(input);
        newStudent.studentID = stringToLongLong(input);
        
        cout << "Enter Email: "; getline(cin, input);
        input = trim(input);
        strcpy(newStudent.email, input.c_str());
        
        cout << "Enter CGPA: "; cin >> newStudent.cgpa;
        if (cin.fail()) throw invalid_argument("Invalid CGPA input.");
        
        cin.ignore();
        cout << "Enter Diploma: "; getline(cin, input);
        input = trim(input);
        strcpy(newStudent.diploma, input.c_str());
        
        cout << "Enter Skills: "; getline(cin, input);
        input = trim(input);
        strcpy(newStudent.skills, input.c_str());
        
        cout << "Enter Applied Job: "; getline(cin, input);
        input = trim(input);
        strcpy(newStudent.appliedJob, input.c_str());
        
        strcpy(newStudent.status, "Active");
        
        // Add to array
        students[studentCount] = newStudent;
        
        // Add to hash table
        int index = newStudent.studentID % TABLE_SIZE;
        if (hashCount[index] < MAX_STUDENTS) {
            hashTable[index][hashCount[index]++] = studentCount;
        }
        
        studentCount++;
        
        cout << "Student added successfully." << endl;
    } catch (exception& e) {
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
    cout << "Data converted to array for sorting/searching." << endl;
}

// Display sorted data
void displaySortedData() {
    cout << "\n--- Sorted Student Records ---" << endl;
    if (arraySize == 0) {
        cout << "No sorted records available. Please convert or sort data first." << endl;
        return;
    }
    for (int i = 0; i < arraySize; i++) {
        cout << i+1 << ". ";
        printStudent(sortedArray[i]);
    }
}

// Save sorted data
void saveSortedData() {
    try {
        ofstream out("sorted_information.txt");
        if (!out.is_open()) throw runtime_error("Unable to open file 'sorted_information.txt'.");
        for (int i = 0; i < arraySize; i++) {
            out << sortedArray[i]->name << "|" << sortedArray[i]->studentID << "|"
                << sortedArray[i]->email << "|" << sortedArray[i]->cgpa << "|"
                << sortedArray[i]->diploma << "|" << sortedArray[i]->skills << "|"
                << sortedArray[i]->appliedJob << endl;
        }
        out.close();
        cout << "Sorted data saved to 'sorted_information.txt'." << endl;
    } catch (exception& e) {
        cerr << "Error saving file: " << e.what() << endl;
    }
}

// Show summary by count
void showSummary(int count) {
    cout << "Total records in system: " << count << endl;
}

// Show summary by CGPA
void showSummary(float cgpa) {
    cout << "Listing students with CGPA above " << cgpa << ":" << endl;
    if (arraySize == 0) {
        cout << "No records available. Please convert data first." << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < arraySize; i++) {
        if (sortedArray[i]->cgpa >= cgpa) {
            printStudent(sortedArray[i]);
            found = true;
        }
    }
    if (!found) {
        cout << "No students found with CGPA above " << cgpa << "." << endl;
    }
}

// Clear screen
void clearScreen() {
    system("cls");
}

// Pause screen
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.clear(); // Clear any error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    cin.get(); // Wait for Enter key
}

// Verify admin login
bool verifyAdminLogin() {
    string inputUser, inputPass;
    cout << "==== Admin Login ====" << endl;
    cout << "Username: "; cin >> inputUser;
    cout << "Password: "; cin >> inputPass;

    ifstream file("login_admin.txt");
    if (!file.is_open()) {
        cerr << "Unable to open file 'login_admin.txt'!" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        size_t delimPos = line.find(',');
        string storedUser = line.substr(0, delimPos);
        string storedPass = line.substr(delimPos + 1);

        if (inputUser == storedUser && inputPass == storedPass) {
            cout << "Login successful." << endl;
            file.close();
            return true;
        }
    }
    file.close();
    cout << "Invalid username or password." << endl;
    return false;
}

// Admin menu
void adminMenu() {
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
        cin.clear(); // Clear any error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear residual input
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }
        
        switch (choice) {
            case 1: clearScreen(); studentCount = loadStudentsFromFile("C:\\Users\\USER\\OneDrive\\Documents\\TDS4223-Project-GR1\\raw data.txt"); pauseScreen(); break;
            case 2: clearScreen(); displayStudents(); pauseScreen(); break;
            case 3: clearScreen(); addNewStudent(); pauseScreen(); break;
            case 4: clearScreen(); convertToArray(); pauseScreen(); break;
            case 5: clearScreen(); displaySortedData(); pauseScreen(); break;
            case 6: clearScreen(); saveSortedData(); pauseScreen(); break;
            case 7: clearScreen(); showSummary(studentCount); pauseScreen(); break;
            case 8: {
                clearScreen(); float cg;
                cout << "Enter minimum CGPA: "; cin >> cg;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cerr << "Invalid CGPA input." << endl;
                } else {
                    convertToArray();
                    showSummary(cg);
                }
                pauseScreen();
                break;
            }
            case 9: clearScreen(); convertToArray(); selectionSortByCGPA(sortedArray, arraySize); displaySortedData(); pauseScreen(); break;
            case 10: clearScreen(); convertToArray(); selectionSortByID(sortedArray, arraySize); displaySortedData(); pauseScreen(); break;
            case 11: clearScreen(); searchByStudentID(); pauseScreen(); break;
            case 0: clearScreen(); cout << "Exiting admin module..." << endl; break;
            default: clearScreen(); cout << "Invalid option!" << endl; pauseScreen();
        }
    } while (choice != 0);
}

// Main function
int main() {
    // Initialize hash table counts
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashCount[i] = 0;
    }
    
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
