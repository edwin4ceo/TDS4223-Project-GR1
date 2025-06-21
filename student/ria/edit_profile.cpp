#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Struct to store student info
struct Student {
    string studentID;
    string name;
    string email;
    string cgpa;
    string diploma;
    string skills;
};

// Function to load students from file
void loadStudents(vector<Student>& students) {
    ifstream file("../shared/students.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Student s;
        getline(ss, s.studentID, '|');
        getline(ss, s.name, '|');
        getline(ss, s.email, '|');
        getline(ss, s.cgpa, '|');
        getline(ss, s.diploma, '|');
        getline(ss, s.skills, '|');
        students.push_back(s);
    }
    file.close();
}

// Function to save students back to file
void saveStudents(const vector<Student>& students) {
    ofstream file("../shared/students.txt");
    for (int i = 0; i < students.size(); i++) {
        file << students[i].studentID << "|"
             << students[i].name << "|"
             << students[i].email << "|"
             << students[i].cgpa << "|"
             << students[i].diploma << "|"
             << students[i].skills << "|\n";
    }
    file.close();
}

int main() {
    vector<Student> students;
    loadStudents(students);

    string targetID;
    cout << "Enter your Student ID to edit profile: ";
    cin >> targetID;

    bool found = false;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].studentID == targetID) {
            found = true;

            cout << "\nCurrent Information:\n";
            cout << "Name   : " << students[i].name << endl;
            cout << "Email  : " << students[i].email << endl;
            cout << "CGPA   : " << students[i].cgpa << endl;
            cout << "Diploma: " << students[i].diploma << endl;
            cout << "Skills : " << students[i].skills << endl;

            // Ask what to update
            cout << "\nEnter new email (or - to keep same): ";
            string newEmail;
            cin.ignore(); // clear newline
            getline(cin, newEmail);
            if (newEmail != "-") students[i].email = newEmail;

            cout << "Enter new CGPA (or - to keep same): ";
            string newCGPA;
            getline(cin, newCGPA);
            if (newCGPA != "-") students[i].cgpa = newCGPA;

            cout << "Enter new skills (or - to keep same): ";
            string newSkills;
            getline(cin, newSkills);
            if (newSkills != "-") students[i].skills = newSkills;

            break;
        }
    }

    if (!found) {
        cout << "Student ID not found.\n";
    } else {
        saveStudents(students);
        cout << "\nProfile updated successfully!\n";
    }

    return 0;
}

