#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
using namespace std;

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

// Remove spaces from string
string trim(string str) {
    // Remove leading spaces
    while (str.length() > 0 && str[0] == ' ') {
        str = str.substr(1);
    }
    // Remove trailing spaces
    while (str.length() > 0 && str[str.length()-1] == ' ') {
        str = str.substr(0, str.length()-1);
    }
    return str;
}

// Convert string to long long (Dev C++ compatible)
long long stringToLongLong(string str) {
    long long result = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result;
}

// Convert string to float (Dev C++ compatible)
float stringToFloat(string str) {
    return atof(str.c_str());
}

// Load students from raw data.txt file
int loadStudentsFromFile(Student students[], const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file!" << endl;
        return 0;
    }
    
    string line;
    int count = 0;
    
    // Skip header lines
    getline(file, line);
    getline(file, line);
    
    cout << "Loading students from file..." << endl;
    
    while (getline(file, line) && count < 100) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        
        // Parse Name
        getline(ss, token, '|');
        token = trim(token);
        strcpy(students[count].name, token.c_str());
        
        // Parse Student ID
        getline(ss, token, '|');
        token = trim(token);
        students[count].studentID = stringToLongLong(token);
        
        // Parse Email
        getline(ss, token, '|');
        token = trim(token);
        strcpy(students[count].email, token.c_str());
        
        // Parse CGPA
        getline(ss, token, '|');
        token = trim(token);
        students[count].cgpa = stringToFloat(token);
        
        // Parse Diploma
        getline(ss, token, '|');
        token = trim(token);
        strcpy(students[count].diploma, token.c_str());
        
        // Parse Skills
        getline(ss, token, '|');
        token = trim(token);
        strcpy(students[count].skills, token.c_str());
        
        // Parse Applied Job
        getline(ss, token);
        token = trim(token);
        strcpy(students[count].appliedJob, token.c_str());
        
        // Set default status
        strcpy(students[count].status, "Active");
        
        count++;
    }
    
    file.close();
    cout << "Loaded " << count << " students successfully!" << endl;
    return count;
}

// Kenneth's 4 Core Functions

// 1. Selection Sort by CGPA (Highest to Lowest)
void selectionSortByCGPA(Student students[], int n) {
    cout << "Sorting by CGPA..." << endl;
    
    for (int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        
        for (int j = i + 1; j < n; j++) {
            if (students[j].cgpa > students[maxIndex].cgpa) {
                maxIndex = j;
            }
        }
        
        if (maxIndex != i) {
            Student temp = students[i];
            students[i] = students[maxIndex];
            students[maxIndex] = temp;
        }
    }
    
    cout << "Sorted by CGPA complete!" << endl;
}

// 2. Selection Sort by Student ID (Lowest to Highest)
void selectionSortByID(Student students[], int n) {
    cout << "Sorting by Student ID..." << endl;
    
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        
        for (int j = i + 1; j < n; j++) {
            if (students[j].studentID < students[minIndex].studentID) {
                minIndex = j;
            }
        }
        
        if (minIndex != i) {
            Student temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
    
    cout << "Sorted by ID complete!" << endl;
}

// 3. Binary Search by Student ID
int binarySearchByID(Student students[], int n, long long targetID) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        if (students[mid].studentID == targetID) {
            return mid;
        }
        
        if (students[mid].studentID < targetID) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

// 4. Search Student by ID Menu
void searchByStudentID(Student students[], int n) {
    long long targetID;
    
    cout << "\n=== Search Student by ID ===" << endl;
    cout << "Enter Student ID: ";
    cin >> targetID;
    
    // Sort by ID first
    selectionSortByID(students, n);
    
    // Search using binary search
    int result = binarySearchByID(students, n, targetID);
    
    if (result != -1) {
        cout << "\n--- STUDENT FOUND ---" << endl;
        cout << "Name: " << students[result].name << endl;
        cout << "ID: " << students[result].studentID << endl;
        cout << "Email: " << students[result].email << endl;
        cout << "CGPA: " << students[result].cgpa << endl;
        cout << "Diploma: " << students[result].diploma << endl;
        cout << "Skills: " << students[result].skills << endl;
        cout << "Applied Job: " << students[result].appliedJob << endl;
    } else {
        cout << "Student not found!" << endl;
    }
}

// Display all students
void displayStudents(Student students[], int n) {
    cout << "\n=== Student List ===" << endl;
    for (int i = 0; i < n; i++) {
        cout << i+1 << ". " << students[i].name 
             << " | ID: " << students[i].studentID 
             << " | CGPA: " << students[i].cgpa << endl;
    }
}

// Test all functions
void testKennethFunctions() {
    Student students[100];
    
    cout << "=== Kenneth's Functions Test ===" << endl;
    
    // Load data from file
    int n = loadStudentsFromFile(students, "raw data.txt");
    if (n == 0) return;
    
    cout << "\nOriginal Data:" << endl;
    displayStudents(students, n);
    
    // Test 1: Sort by CGPA
    cout << "\n--- Sort by CGPA Test ---" << endl;
    selectionSortByCGPA(students, n);
    displayStudents(students, n);
    
    // Reload data for next test
    n = loadStudentsFromFile(students, "raw data.txt");
    
    // Test 2: Sort by ID
    cout << "\n--- Sort by ID Test ---" << endl;
    selectionSortByID(students, n);
    displayStudents(students, n);
    
    // Test 3: Search by ID
    cout << "\n--- Search by ID Test ---" << endl;
    searchByStudentID(students, n);
}

// Main function
int main() {
    testKennethFunctions();
    
    cout << "\nPress Enter to exit...";
    cin.get();
    cin.get();
    
    return 0;
}
