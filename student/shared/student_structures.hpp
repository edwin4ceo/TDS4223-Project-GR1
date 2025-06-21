#ifndef STUDENT_STRUCTURES_HPP
#define STUDENT_STRUCTURES_HPP

#include <string>
using namespace std;

// Struct to store student profile details
struct Student {
    string studentID;    // e.g., ST001
    string name;
    string email;
    float cgpa;
    string diploma;
    string skills;
};

// Struct to represent an internship application made by a student
struct Application {
    string studentID;
    string jobID;
    string jobTitle;
    string company;
    string status;  // e.g., "Pending", "Approved", "Rejected"
};

#endif // STUDENT_STRUCTURES_HPP
