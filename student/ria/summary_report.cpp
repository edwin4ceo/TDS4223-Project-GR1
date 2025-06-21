#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Application {
    string studentID;
    string jobID;
    string jobTitle;
    string company;
    string status;
};

// Function to load applications from file
void loadApplications(vector<Application>& apps) {
    ifstream file("../shared/applications.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Application a;
        getline(ss, a.studentID, '|');
        getline(ss, a.jobID, '|');
        getline(ss, a.jobTitle, '|');
        getline(ss, a.company, '|');
        getline(ss, a.status, '|');
        apps.push_back(a);
    }
    file.close();
}

// Function to generate and save summary report
void generateReport(const vector<Application>& apps, const string& targetID) {
    int total = 0, approved = 0, rejected = 0, pending = 0;

    for (int i = 0; i < apps.size(); i++) {
        if (apps[i].studentID == targetID) {
            total++;
            if (apps[i].status == "Approved") approved++;
            else if (apps[i].status == "Rejected") rejected++;
            else pending++;
        }
    }

    // Display on screen
    cout << "\nApplication Summary:\n";
    cout << "Student ID       : " << targetID << endl;
    cout << "Total Applications: " << total << endl;
    cout << "Approved          : " << approved << endl;
    cout << "Rejected          : " << rejected << endl;
    cout << "Pending           : " << pending << endl;

    // Save to student_summary.txt
    ofstream outFile("../shared/student_summary.txt");
    outFile << "Application Summary for Student ID: " << targetID << "\n";
    outFile << "Total Applications: " << total << "\n";
    outFile << "Approved          : " << approved << "\n";
    outFile << "Rejected          : " << rejected << "\n";
    outFile << "Pending           : " << pending << "\n";
    outFile.close();
}

int main() {
    vector<Application> apps;
    loadApplications(apps);

    string studentID;
    cout << "Enter your Student ID to generate application summary: ";
    cin >> studentID;

    generateReport(apps, studentID);

    cout << "\nSummary saved to student_summary.txt\n";

    return 0;
}

