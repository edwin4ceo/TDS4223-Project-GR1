#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
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

string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
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

void searchInternships() {
    string keyword;
    cout << "Enter a keyword to search (job title or company): ";
    cin.ignore();
    getline(cin, keyword);
    keyword = toLower(keyword);

    vector<Internship> internships = loadInternships("../shared/job_listings.txt");
    if (internships.empty()) {
        cout << "No internship data available.\n";
        return;
    }

    bool found = false;
    cout << "\nSearch Results:\n------------------------------------------\n";
    for (int i = 0; i < internships.size(); i++) {
        string titleLower = toLower(internships[i].jobTitle);
        string companyLower = toLower(internships[i].company);
        if (titleLower.find(keyword) != string::npos || companyLower.find(keyword) != string::npos) {
            cout << "Job ID    : " << internships[i].jobID << "\n"
                 << "Job Title : " << internships[i].jobTitle << "\n"
                 << "Company   : " << internships[i].company << "\n"
                 << "Deadline  : " << internships[i].deadline << "\n"
                 << "------------------------------------------\n";
            found = true;
        }
    }

    if (!found)
        cout << "No matching internships found for: " << keyword << "\n";
}
