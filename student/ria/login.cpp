#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Function to verify Student ID
bool loginStudent(const string& id) {
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

int main() {
    string studentID;
    cout << "Enter your Student ID to login: ";
    cin >> studentID;

    if (loginStudent(studentID)) {
        cout << "\nLogin successful. Welcome, Student " << studentID << "!\n";
    } else {
        cout << "\nLogin failed. Student ID not found.\n";
    }

    return 0;
}

