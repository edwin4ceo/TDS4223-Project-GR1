#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../shared/student_structures.hpp"
#include "../shared/student_utils.hpp"
using namespace std;

// Lowercase converter
string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

// Search logic
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

