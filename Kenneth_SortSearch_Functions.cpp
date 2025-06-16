#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

// Student struct definition
struct Student {
    int studentID;
    char name[50];
    char email[100];
    float cgpa;
    char diploma[100];
    char appliedJob[100];
    char status[20];
};

// Kenneth's 4 Core Functions for Admin Module

// 1. Selection Sort by CGPA (Highest to Lowest)
void selectionSortByCGPA(Student students[], int n) {
    cout << "Sorting students by CGPA..." << endl;
    
    for (int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        
        // Find student with highest CGPA
        for (int j = i + 1; j < n; j++) {
            if (students[j].cgpa > students[maxIndex].cgpa) {
                maxIndex = j;
            }
        }
        
        // Swap if needed
        if (maxIndex != i) {
            Student temp = students[i];
            students[i] = students[maxIndex];
            students[maxIndex] = temp;
        }
    }
    
    cout << "Students sorted by CGPA successfully!" << endl;
}

// 2. Selection Sort by Student ID (Lowest to Highest)
void selectionSortByID(Student students[], int n) {
    cout << "Sorting students by ID..." << endl;
    
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        
        // Find student with smallest ID
        for (int j = i + 1; j < n; j++) {
            if (students[j].studentID < students[minIndex].studentID) {
                minIndex = j;
            }
        }
        
        // Swap if needed
        if (minIndex != i) {
            Student temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
    
    cout << "Students sorted by ID successfully!" << endl;
}

// 3. Binary Search by Student ID
int binarySearchByID(Student students[], int n, int targetID) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        if (students[mid].studentID == targetID) {
            return mid;  // Found
        }
        
        if (students[mid].studentID < targetID) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;  // Not found
}

// 4. Search Student by ID (Menu Function)
void searchByStudentID(Student students[], int n) {
    int targetID;
    
    cout << "\n=== Search Student by ID ===" << endl;
    cout << "Enter Student ID: ";
    cin >> targetID;
    
    // Sort array by ID first for binary search
    cout << "Preparing data for search..." << endl;
    selectionSortByID(students, n);
    
    // Perform binary search
    int result = binarySearchByID(students, n, targetID);
    
    if (result != -1) {
        cout << "\n--- STUDENT FOUND ---" << endl;
        cout << "Student ID: " << students[result].studentID << endl;
        cout << "Name: " << students[result].name << endl;
        cout << "Email: " << students[result].email << endl;
        cout << "CGPA: " << fixed << setprecision(2) << students[result].cgpa << endl;
        cout << "Diploma: " << students[result].diploma << endl;
        cout << "Applied Job: " << students[result].appliedJob << endl;
        cout << "Status: " << students[result].status << endl;
    } else {
        cout << "\nStudent with ID " << targetID << " not found!" << endl;
    }
}

// Helper function to display all students
void displayStudents(Student students[], int n) {
    cout << "\n=== Student List ===" << endl;
    for (int i = 0; i < n; i++) {
        cout << i+1 << ". ID: " << students[i].studentID 
             << ", Name: " << students[i].name 
             << ", CGPA: " << fixed << setprecision(2) << students[i].cgpa << endl;
    }
}

// Test function
void testKennethFunctions() {
    // Sample data
    Student testStudents[5] = {
        {12345, "Alice Johnson", "alice@mail.com", 3.85, "Computer Science", "Software Engineer", "Active"},
        {12340, "Bob Smith", "bob@mail.com", 3.20, "Information Technology", "Data Analyst", "Pending"},
        {12350, "Charlie Brown", "charlie@mail.com", 3.95, "Computer Engineering", "Developer", "Active"},
        {12335, "Diana Prince", "diana@mail.com", 3.60, "Software Engineering", "Web Developer", "Active"},
        {12355, "Eve Wilson", "eve@mail.com", 3.75, "Computer Science", "Mobile Developer", "Pending"}
    };
    
    int n = 5;
    
    cout << "=== Testing Kenneth's Functions ===" << endl;
    
    cout << "\nOriginal Data:" << endl;
    displayStudents(testStudents, n);
    
    // Test 1: Sort by CGPA
    cout << "\n--- Test 1: Sort by CGPA ---" << endl;
    selectionSortByCGPA(testStudents, n);
    displayStudents(testStudents, n);
    
    // Test 2: Sort by ID
    cout << "\n--- Test 2: Sort by ID ---" << endl;
    selectionSortByID(testStudents, n);
    displayStudents(testStudents, n);
    
    // Test 3: Search by ID
    cout << "\n--- Test 3: Search by ID ---" << endl;
    searchByStudentID(testStudents, n);
}

// Main function for testing
int main() {
    testKennethFunctions();
    
    cout << "\nPress any key to exit...";
    cin.get();
    cin.get();
    
    return 0;
}
