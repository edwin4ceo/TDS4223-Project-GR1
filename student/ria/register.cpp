#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Student {
    string studentID;
    string name;
    string email;
    string cgpa;
    string diploma;
    string skills;
};

// Function to check if Student ID already exists
bool studentExists(const string& id) {
    ifstream file("../shared/students.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string sid;
        getline(ss, sid, '|');
        if (sid == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Function to register a new student
void registerStudent() {
    Student s;
    cout << "Enter Student ID: ";
    cin >> s.studentID;

    if (studentExists(s.studentID)) {
        cout << "Student ID already exists. Registration failed.\n";
        return;
    }

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, s.name);
    cout << "Enter Email: ";
    getline(cin, s.email);
    cout << "Enter CGPA: ";
    getline(cin, s.cgpa);
    cout << "Enter Diploma: ";
    getline(cin, s.diploma);
    cout << "Enter Skills (comma-separated): ";
    getline(cin, s.skills);

    ofstream file("../shared/students.txt", ios::app);
    file << s.studentID << "|" << s.name << "|" << s.email << "|" << s.cgpa << "|"
         << s.diploma << "|" << s.skills << "|\n";
    file.close();

    cout << "\nRegistration successful!\n";
}

int main() {
    registerStudent();
    return 0;
}

