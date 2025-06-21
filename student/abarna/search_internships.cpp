#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> // only for transform (toLower)
using namespace std;

// Define Internship struct
struct Internship {
    string jobID;
    string jobTitle;
    string company;
    string deadline;
};

// Split line using '|'
vector<string> splitLine(const string& line, char delimiter = '|') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Convert string to lowercase
string toLower(const string& str) {
    string lower = str;
    for (size_t i = 0; i < lower.length(); i++) {
        lower[i] = tolower(lower[i]);
    }
    return lower;
}

// Load all internships from file
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

// Manual linear search (no STL search)
void searchInternships(const vector<Internship>& internships, const string& keyword) {
    string searchTerm = toLower(keyword);
    bool found = false;

    cout << "\nSearch Results:\n";
    cout << "------------------------------------------\n";

    for (size_t i = 0; i < internships.size(); i++) {
        string titleLower = toLower(internships[i].jobTitle);
        string companyLower = toLower(internships[i].company);

        if (titleLower.find(searchTerm) != string::npos || companyLower.find(searchTerm) != string::npos) {
            cout << "Job ID    : " << internships[i].jobID << endl;
            cout << "Job Title : " << internships[i].jobTitle << endl;
            cout << "Company   : " << internships[i].company << endl;
            cout << "Deadline  : " << internships[i].deadline << endl;
            cout << "------------------------------------------\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No matching internships found for: " << keyword << endl;
    }
}

int main() {
    string keyword;
    cout << "Enter a keyword to search (job title or company): ";
    cin.ignore();
    getline(cin, keyword);

    vector<Internship> internships = loadInternships("../shared/job_listings.txt");
    if (internships.empty()) {
        cout << "No internship data available.\n";
        return 1;
    }

    searchInternships(internships, keyword);

    return 0;
}

