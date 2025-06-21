#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../shared/student_structures.hpp"
using namespace std;

// Split a line using ',' delimiter
vector<string> splitLine(const string& line, char delimiter = ',') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Load and display applications for the given student ID
void viewApplications(const string& studentID, const string& filename) {
    ifstream file(filename.c_str());
    if (!file) {
        cerr << "Error: Could not open applications.txt\n";
        return;
    }

    string line;
    bool found = false;
    cout << "\nYour Internship Applications:\n";
    cout << "------------------------------------------\n";

    while (getline(file, line)) {
        vector<string> parts = splitLine(line, ',');
        if (parts.size() >= 5 && parts[0] == studentID) {
            cout << "Job ID    : " << parts[1] << endl;
            cout << "Job Title : " << parts[2] << endl;
            cout << "Company   : " << parts[3] << endl;
            cout << "Status    : " << parts[4] << endl;
            cout << "------------------------------------------\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No applications found for Student ID: " << studentID << endl;
    }

    file.close();
}

int main() {
    string studentID;
    cout << "Enter your Student ID to view applications: ";
    cin >> studentID;

    viewApplications(studentID, "../shared/applications.txt");

    return 0;
}

