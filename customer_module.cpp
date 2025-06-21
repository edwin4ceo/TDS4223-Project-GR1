#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// ---------- STRUCT DEFINITIONS ----------

struct Student {
    string studentID;
    string name;
    string email;
    string cgpa;
    string diploma;
    string skills;
};

struct Internship {
    string jobID;
    string jobTitle;
    string company;
    string deadline;
};

struct Application {
    string studentID;
    string jobID;
    string jobTitle;
    string company;
    string status;
    Application* next;
};

// ---------- LOGIN FUNCTION ----------

bool loginStudent(const string& id) {
    ifstream file("../shared/students.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string sid;
        getline(ss, sid, '|');
        if (sid == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// ---------- PROFILE FUNCTIONS ----------

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

void editProfile(const string& targetID) {
    vector<Student> students;
    loadStudents(students);
    bool found = false;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].studentID == targetID) {
            found = true;
            cout << "\\nCurrent Information:\\n";
            cout << "Name   : " << students[i].name << endl;
            cout << "Email  : " << students[i].email << endl;
            cout << "CGPA   : " << students[i].cgpa << endl;
            cout << "Diploma: " << students[i].diploma << endl;
            cout << "Skills : " << students[i].skills << endl;

            cout << "\\nEnter new email (or - to keep same): ";
            string newEmail;
            cin.ignore();
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
        cout << "Student ID not found.\\n";
    } else {
        saveStudents(students);
        cout << "\\nProfile updated successfully!\\n";
    }
}

// ---------- INTERNSHIP JOB FUNCTIONS ----------

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

void displayJobs(const vector<Internship>& jobs) {
    cout << "\\nJob Listings:\\n";
    cout << "-----------------------------------------------------------\\n";
    for (int i = 0; i < jobs.size(); i++) {
        cout << "ID       : " << jobs[i].jobID << endl;
        cout << "Title    : " << jobs[i].jobTitle << endl;
        cout << "Company  : " << jobs[i].company << endl;
        cout << "Deadline : " << jobs[i].deadline << endl;
        cout << "-----------------------------------------------------------\\n";
    }
}

bool sortByTitle(const Internship& a, const Internship& b) {
    return a.jobTitle < b.jobTitle;
}

bool sortByDeadline(const Internship& a, const Internship& b) {
    return a.deadline < b.deadline;
}

void sortAndDisplayJobs() {
    vector<Internship> jobs;
    loadJobs(jobs);
    if (jobs.empty()) {
        cout << "No job listings found.\\n";
        return;
    }

    int choice;
    cout << "\\nSort Internship Jobs By:\\n";
    cout << "1. Job Title\\n";
    cout << "2. Deadline\\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        sort(jobs.begin(), jobs.end(), sortByTitle);
    } else if (choice == 2) {
        sort(jobs.begin(), jobs.end(), sortByDeadline);
    } else {
        cout << "Invalid choice.\\n";
        return;
    }

    displayJobs(jobs);
}

// ---------- DYNAMIC STRUCTURE: LINKED LIST FOR APPLICATIONS ----------

Application* createNode(string sid, string jid, string title, string comp, string stat) {
    Application* newNode = new Application;
    newNode->studentID = sid;
    newNode->jobID = jid;
    newNode->jobTitle = title;
    newNode->company = comp;
    newNode->status = stat;
    newNode->next = NULL;
    return newNode;
}

void insertApplication(Application*& head, Application* newNode) {
    if (head == NULL) {
        head = newNode;
    } else {
        Application* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void loadApplications(Application*& head) {
    ifstream file("../shared/applications.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string sid, jid, title, comp, stat;
        getline(ss, sid, '|');
        getline(ss, jid, '|');
        getline(ss, title, '|');
        getline(ss, comp, '|');
        getline(ss, stat, '|');
        Application* node = createNode(sid, jid, title, comp, stat);
        insertApplication(head, node);
    }
    file.close();
}

void generateReport(Application* head, const string& targetID) {
    int total = 0, approved = 0, rejected = 0, pending = 0;
    Application* current = head;
    while (current != NULL) {
        if (current->studentID == targetID) {
            total++;
            if (current->status == "Approved") approved++;
            else if (current->status == "Rejected") rejected++;
            else pending++;
        }
        current = current->next;
    }

    cout << "\\nApplication Summary:\\n";
    cout << "Student ID       : " << targetID << endl;
    cout << "Total Applications: " << total << endl;
    cout << "Approved          : " << approved << endl;
    cout << "Rejected          : " << rejected << endl;
    cout << "Pending           : " << pending << endl;

    ofstream outFile("../shared/student_summary.txt");
    outFile << "Application Summary for Student ID: " << targetID << "\\n";
    outFile << "Total Applications: " << total << "\\n";
    outFile << "Approved          : " << approved << "\\n";
    outFile << "Rejected          : " << rejected << "\\n";
    outFile << "Pending           : " << pending << "\\n";
    outFile.close();
}

// ---------- MAIN MENU ----------

int main() {
    string studentID;
    cout << "Enter your Student ID to login: ";
    cin >> studentID;

    if (!loginStudent(studentID)) {
        cout << "Login failed. Student ID not found.\\n";
        return 0;
    }

    cout << "\\nLogin successful. Welcome, Student " << studentID << "!\\n";

    int choice;
    do {
        cout << "\\n======= Customer Menu =======\\n";
        cout << "1. View and Edit Profile\\n";
        cout << "2. View & Sort Internship Listings\\n";
        cout << "3. View Application Summary\\n";
        cout << "0. Exit\\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                editProfile(studentID);
                break;
            case 2:
                sortAndDisplayJobs();
                break;
            case 3: {
                Application* head = NULL;
                loadApplications(head);
                generateReport(head, studentID);
                while (head != NULL) {
                    Application* temp = head;
                    head = head->next;
                    delete temp;
                }
                break;
            }
            case 0:
                cout << "Goodbye!\\n";
                break;
            default:
                cout << "Invalid choice. Try again.\\n";
        }
    } while (choice != 0);

    return 0;
}
