#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

// Struct for internship job listings
struct Internship {
    string jobID;
    string jobTitle;
    string company;
    string deadline;
};

// Function to read job listings from file
void loadJobs(vector<Internship>& jobs) {
    ifstream file("../shared/job_listings.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Internship job;
        getline(ss, job.jobID, '|');
        getline(ss, job.jobTitle, '|');
        getline(ss, job.company, '|');
        getline(ss, job.deadline, '|');
        jobs.push_back(job);
    }
    file.close();
}

// Function to display job listings
void displayJobs(const vector<Internship>& jobs) {
    cout << "\nJob Listings:\n";
    cout << "-----------------------------------------------------------\n";
    for (int i = 0; i < jobs.size(); i++) {
        cout << "ID       : " << jobs[i].jobID << endl;
        cout << "Title    : " << jobs[i].jobTitle << endl;
        cout << "Company  : " << jobs[i].company << endl;
        cout << "Deadline : " << jobs[i].deadline << endl;
        cout << "-----------------------------------------------------------\n";
    }
}

// Sort by job title
bool sortByTitle(const Internship& a, const Internship& b) {
    return a.jobTitle < b.jobTitle;
}

// Sort by deadline
bool sortByDeadline(const Internship& a, const Internship& b) {
    return a.deadline < b.deadline;
}

int main() {
    vector<Internship> jobs;
    loadJobs(jobs);

    if (jobs.empty()) {
        cout << "No job listings found.\n";
        return 0;
    }

    int choice;
    cout << "Sort Internship Jobs By:\n";
    cout << "1. Job Title\n";
    cout << "2. Deadline\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    if (choice == 1) {
        sort(jobs.begin(), jobs.end(), sortByTitle);
        cout << "\nJobs sorted by Job Title:\n";
    } else if (choice == 2) {
        sort(jobs.begin(), jobs.end(), sortByDeadline);
        cout << "\nJobs sorted by Deadline:\n";
    } else {
        cout << "Invalid choice.\n";
        return 0;
    }

    displayJobs(jobs);

    return 0;
}

