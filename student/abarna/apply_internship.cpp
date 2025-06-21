#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../shared/student_structures.hpp"
using namespace std;

struct Internship {
    string jobID, jobTitle, company, deadline;
};

vector<string> splitLine(const string& line, char delimiter = '|') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

vector<Internship> loadInternships(const string& filename) {
    vector<Internship> internships;
    ifstream file(filename.c_str());
    if (!file) {
        cerr << "Error: Could not open job_listings.txt\n";
        return internships;
    }
    string line;
    while (getline(file, line)) {
        vector<string> parts = splitLine(line, '|');
        if (parts.size() >= 4) {
            Internship job;
            job.jobID = parts[0];
            job.jobTitle = parts[1];
            job.company = parts[2];
            job.deadline = parts[3];
            internships.push_back(job);
        }
    }
    file.close();
    return internships;
}

void saveApplication(const Application& app, const string& filename) {
    ofstream file(filename.c_str(), ios::app);
    if (!file) {
        cerr << "Error: Could not open applications.txt\n";
        return;
    }
    file << app.studentID << "," << app.jobID << "," << app.jobTitle 
         << "," << app.company << "," << app.status << endl;
    file.close();
}

void applyInternship(const string& studentID) {
    vector<Internship> internships = loadInternships("../shared/job_listings.txt");
    if (internships.empty()) {
        cout << "No internships available.\n";
        return;
    }

    cout << "\nAvailable Internships:\n";
    for (int i = 0; i < internships.size(); i++) {
        cout << i + 1 << ". " << internships[i].jobTitle
             << " at " << internships[i].company
             << " (Deadline: " << internships[i].deadline << ")\n";
    }

    int choice;
    cout << "\nEnter the number of the internship to apply for: ";
    cin >> choice;
    if (choice < 1 || choice > internships.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    Application app;
    app.studentID = studentID;
    app.jobID     = internships[choice - 1].jobID;
    app.jobTitle  = internships[choice - 1].jobTitle;
    app.company   = internships[choice - 1].company;
    app.status    = "Pending";

    saveApplication(app, "../shared/applications.txt");
    cout << "\n✅ Application submitted successfully.\n";
}
