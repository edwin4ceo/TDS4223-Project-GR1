#ifndef STUDENT_UTILS_HPP
#define STUDENT_UTILS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

// Generic line splitter
inline vector<string> splitLine(const string& line, char delimiter = '|') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

// Internship structure for shared use
struct Internship {
    string jobID, jobTitle, company, deadline;
};

// Load internships from job_listings.txt
inline vector<Internship> loadInternships(const string& filename) {
    vector<Internship> internships;
    ifstream file(filename.c_str());
    if (!file) {
        cerr << "Error: Could not open " << filename << "\n";
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

#endif
