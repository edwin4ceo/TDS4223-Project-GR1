#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../shared/student_structures.hpp"
#include "../shared/student_utils.hpp"
using namespace std;

void viewApplications(const string& studentID) {
    ifstream file("../shared/applications.txt");
    if (!file) {
        cerr << "Error: Could not open applications.txt\n";
        return;
    }

    string line;
    bool found = false;
    cout << "\nYour Internship Applications:\n"
         << "------------------------------------------\n";
    while (getline(file, line)) {
        vector<string> parts = splitLine(line, ',');  // override default '|'
        if (parts.size() >= 5 && parts[0] == studentID) {
            cout << "Job ID    : " << parts[1] << "\n"
                 << "Job Title : " << parts[2] << "\n"
                 << "Company   : " << parts[3] << "\n"
                 << "Status    : " << parts[4] << "\n"
                 << "------------------------------------------\n";
            found = true;
        }
    }
    if (!found)
        cout << "No applications found for Student ID: " << studentID << "\n";
    file.close();
}

