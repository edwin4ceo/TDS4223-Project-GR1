#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <limits>
#include <vector>
#include <algorithm>
using namespace std;

// Constants
const int MAX_STUDENTS = 100;
const int MAX_APPLICATIONS = 200;
const int TABLE_SIZE = 10;
const int MAX_JOBS = 50;

// Forward declarations
class Person;
class Student;
class Admin;
class InternshipJob;
class Application;

// Exception classes for error handling
class FileException : public exception {
private:
    string message;
public:
    FileException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class LoginException : public exception {
private:
    string message;
public:
    LoginException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class DataException : public exception {
private:
    string message;
public:
    DataException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Dynamic Non-Primitive Data Structure - Linked List for Applications
struct ApplicationNode {
    string studentID;
    string jobID;
    string jobTitle;
    string company;
    string status;
    ApplicationNode* next;
    
    ApplicationNode(string sid, string jid, string title, string comp, string stat) 
        : studentID(sid), jobID(jid), jobTitle(title), company(comp), status(stat), next(nullptr) {}
};

class ApplicationList {
private:
    ApplicationNode* head;
    int count;
    
public:
    ApplicationList() : head(nullptr), count(0) {}
    
    ~ApplicationList() {
        clear();
    }
    
    void insert(string studentID, string jobID, string jobTitle, string company, string status);
    void display();
    void clear();
    bool search(string studentID, string jobID);
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
    int getCount() const { return count; }
    
    friend class Student;
    friend class Admin;
};

// Base class - Person
class Person {
protected:
    string id;
    string name;
    string email;
    
public:
    Person() : id(""), name(""), email("") {}
    Person(string id, string name, string email) : id(id), name(name), email(email) {}
    virtual ~Person() {}
    
    virtual void displayInfo() = 0;
    virtual bool login() = 0;
    
    // Overloaded functions
    void setInfo(string id);
    void setInfo(string id, string name);
    void setInfo(string id, string name, string email);
    
    string getID() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    
    friend class InternshipSystem;
};

// Derived class - Student
class Student : public Person {
private:
    float cgpa;
    string diploma;
    string skills;
    static int totalStudents;
    
public:
    Student() : Person(), cgpa(0.0), diploma(""), skills("") { totalStudents++; }
    Student(string id, string name, string email, float cgpa, string diploma, string skills)
        : Person(id, name, email), cgpa(cgpa), diploma(diploma), skills(skills) { totalStudents++; }
    
    ~Student() { totalStudents--; }
    
    void displayInfo() override;
    bool login() override;
    
    // Overloaded functions
    void updateProfile();
    void updateProfile(string newEmail);
    void updateProfile(string newEmail, float newCGPA);
    void updateProfile(string newEmail, float newCGPA, string newSkills);
    
    void viewInternships();
    void applyForInternship();
    void viewMyApplications();
    void generateSummaryReport();
    
    // Getters and Setters
    float getCGPA() const { return cgpa; }
    string getDiploma() const { return diploma; }
    string getSkills() const { return skills; }
    
    void setCGPA(float cgpa) { this->cgpa = cgpa; }
    void setDiploma(string diploma) { this->diploma = diploma; }
    void setSkills(string skills) { this->skills = skills; }
    
    static int getTotalStudents() { return totalStudents; }
    
    friend void saveStudentToFile(const Student& student);
    friend class Admin;
};

// Derived class - Admin
class Admin : public Person {
private:
    string password;
    string role;
    static int totalAdmins;
    
public:
    Admin() : Person(), password(""), role("Admin") { totalAdmins++; }
    Admin(string id, string name, string email, string password)
        : Person(id, name, email), password(password), role("Admin") { totalAdmins++; }
    
    ~Admin() { totalAdmins--; }
    
    void displayInfo() override;
    bool login() override;
    
    void manageStudents();
    void manageInternships();
    void viewAllApplications();
    void generateReports();
    
    static int getTotalAdmins() { return totalAdmins; }
    
    friend class InternshipSystem;
};

// InternshipJob class
class InternshipJob {
private:
    string jobID;
    string title;
    string company;
    string deadline;
    string requirements;
    bool isActive;
    
public:
    InternshipJob() : jobID(""), title(""), company(""), deadline(""), requirements(""), isActive(true) {}
    InternshipJob(string id, string title, string company, string deadline, string req)
        : jobID(id), title(title), company(company), deadline(deadline), requirements(req), isActive(true) {}
    
    ~InternshipJob() {}
    
    // Overloaded display functions
    void display();
    void display(bool showDetails);
    void display(string format);
    void display(bool showDetails, string format);
    
    // Getters
    string getJobID() const { return jobID; }
    string getTitle() const { return title; }
    string getCompany() const { return company; }
    string getDeadline() const { return deadline; }
    string getRequirements() const { return requirements; }
    bool getIsActive() const { return isActive; }
    
    // Setters
    void setJobID(string id) { jobID = id; }
    void setTitle(string title) { this->title = title; }
    void setCompany(string company) { this->company = company; }
    void setDeadline(string deadline) { this->deadline = deadline; }
    void setRequirements(string req) { requirements = req; }
    void setActive(bool active) { isActive = active; }
    
    friend class Admin;
    friend class Student;
};

// Hash Table for fast student lookup
class StudentHashTable {
private:
    Student** table;
    int size;
    int count;
    
    int hash(string key);
    
public:
    StudentHashTable(int size = TABLE_SIZE);
    ~StudentHashTable();
    
    void insert(Student* student);
    Student* search(string studentID);
    void remove(string studentID);
    void display();
    double getLoadFactor() const { return (double)count / size; }
};

// Main System class
class InternshipSystem {
private:
    vector<Student*> students;
    vector<Admin*> admins;
    vector<InternshipJob*> jobs;
    ApplicationList* applications;
    StudentHashTable* studentHash;
    
    // Utility functions
    string trim(const string& str);
    long long stringToLongLong(const string& str);
    float stringToFloat(const string& str);
    
public:
    InternshipSystem();
    ~InternshipSystem();
    
    // File operations
    void loadStudentsFromFile();
    void saveStudentsToFile();
    void loadJobsFromFile();
    void saveJobsToFile();
    void loadApplicationsFromFile();
    void saveApplicationsToFile();
    
    // System operations
    void mainMenu();
    void studentMenu();
    void adminMenu();
    
    // Sorting algorithms (manually implemented)
    void selectionSortStudentsByCGPA();
    void selectionSortStudentsByID();
    void bubbleSortJobsByDeadline();
    
    // Searching algorithms (manually implemented)
    int binarySearchStudentByID(string targetID);
    Student* hashSearchStudent(string studentID);
    
    // System management
    void registerStudent();
    void registerAdmin();
    void addInternshipJob();
    void displayAllStudents();
    void displayAllJobs();
    void displayStatistics();
    
    friend void saveSystemData(const InternshipSystem& system);
    friend void loadSystemData(InternshipSystem& system);
};

// Static member initialization
int Student::totalStudents = 0;
int Admin::totalAdmins = 0;

// Implementation of ApplicationList methods
void ApplicationList::insert(string studentID, string jobID, string jobTitle, string company, string status) {
    ApplicationNode* newNode = new ApplicationNode(studentID, jobID, jobTitle, company, status);
    if (head == nullptr) {
        head = newNode;
    } else {
        ApplicationNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    count++;
}

void ApplicationList::display() {
    cout << "\n=== All Applications ===" << endl;
    cout << left << setw(12) << "Student ID" 
         << setw(8) << "Job ID" 
         << setw(25) << "Job Title" 
         << setw(20) << "Company" 
         << setw(12) << "Status" << endl;
    cout << string(77, '-') << endl;
    
    ApplicationNode* current = head;
    while (current != nullptr) {
        cout << left << setw(12) << current->studentID
             << setw(8) << current->jobID
             << setw(25) << current->jobTitle
             << setw(20) << current->company
             << setw(12) << current->status << endl;
        current = current->next;
    }
}

void ApplicationList::clear() {
    while (head != nullptr) {
        ApplicationNode* temp = head;
        head = head->next;
        delete temp;
    }
    count = 0;
}

bool ApplicationList::search(string studentID, string jobID) {
    ApplicationNode* current = head;
    while (current != nullptr) {
        if (current->studentID == studentID && current->jobID == jobID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void ApplicationList::saveToFile(const string& filename) {
    try {
        ofstream file(filename);
        if (!file.is_open()) {
            throw FileException("Cannot open file for writing: " + filename);
        }
        
        ApplicationNode* current = head;
        while (current != nullptr) {
            file << current->studentID << "|" << current->jobID << "|"
                 << current->jobTitle << "|" << current->company << "|"
                 << current->status << endl;
            current = current->next;
        }
        file.close();
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ApplicationList::loadFromFile(const string& filename) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            throw FileException("Cannot open file for reading: " + filename);
        }
        
        clear(); // Clear existing data
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string studentID, jobID, jobTitle, company, status;
            
            getline(ss, studentID, '|');
            getline(ss, jobID, '|');
            getline(ss, jobTitle, '|');
            getline(ss, company, '|');
            getline(ss, status, '|');
            
            insert(studentID, jobID, jobTitle, company, status);
        }
        file.close();
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

// Implementation of Person methods
void Person::setInfo(string id) {
    this->id = id;
}

void Person::setInfo(string id, string name) {
    this->id = id;
    this->name = name;
}

void Person::setInfo(string id, string name, string email) {
    this->id = id;
    this->name = name;
    this->email = email;
}

// Implementation of Student methods
void Student::displayInfo() {
    cout << "\n=== Student Information ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "CGPA: " << fixed << setprecision(2) << cgpa << endl;
    cout << "Diploma: " << diploma << endl;
    cout << "Skills: " << skills << endl;
}

bool Student::login() {
    try {
        string inputID;
        cout << "Enter Student ID: ";
        cin >> inputID;
        
        ifstream file("students.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open students.txt");
        }
        
        string line;
        while (getline(file, line)) {
            if (line.find(inputID) != string::npos) {
                file.close();
                return true;
            }
        }
        file.close();
        throw LoginException("Invalid Student ID");
    } catch (const exception& e) {
        cout << "Login Error: " << e.what() << endl;
        return false;
    }
}

void Student::updateProfile() {
    cout << "\n=== Update Profile ===" << endl;
    cout << "Current Email: " << email << endl;
    cout << "Enter new email (or press Enter to keep current): ";
    cin.ignore();
    string newEmail;
    getline(cin, newEmail);
    if (!newEmail.empty()) {
        email = newEmail;
    }
}

void Student::updateProfile(string newEmail) {
    email = newEmail;
    cout << "Email updated successfully!" << endl;
}

void Student::updateProfile(string newEmail, float newCGPA) {
    email = newEmail;
    cgpa = newCGPA;
    cout << "Email and CGPA updated successfully!" << endl;
}

void Student::updateProfile(string newEmail, float newCGPA, string newSkills) {
    email = newEmail;
    cgpa = newCGPA;
    skills = newSkills;
    cout << "Profile updated successfully!" << endl;
}

void Student::viewInternships() {
    try {
        ifstream file("job_listings.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open job_listings.txt");
        }
        
        cout << "\n=== Available Internships ===" << endl;
        cout << left << setw(8) << "Job ID" 
             << setw(25) << "Title" 
             << setw(20) << "Company" 
             << setw(12) << "Deadline" << endl;
        cout << string(65, '-') << endl;
        
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string jobID, title, company, deadline;
            
            getline(ss, jobID, '|');
            getline(ss, title, '|');
            getline(ss, company, '|');
            getline(ss, deadline, '|');
            
            cout << left << setw(8) << jobID
                 << setw(25) << title
                 << setw(20) << company
                 << setw(12) << deadline << endl;
        }
        file.close();
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Student::applyForInternship() {
    string jobID;
    cout << "Enter Job ID to apply: ";
    cin >> jobID;
    
    try {
        // Check if job exists
        ifstream jobFile("job_listings.txt");
        if (!jobFile.is_open()) {
            throw FileException("Cannot open job_listings.txt");
        }
        
        bool jobFound = false;
        string jobTitle, company;
        string line;
        
        while (getline(jobFile, line)) {
            if (line.find(jobID) == 0) {
                stringstream ss(line);
                string tempID;
                getline(ss, tempID, '|');
                getline(ss, jobTitle, '|');
                getline(ss, company, '|');
                jobFound = true;
                break;
            }
        }
        jobFile.close();
        
        if (!jobFound) {
            throw DataException("Job ID not found");
        }
        
        // Add application
        ofstream appFile("applications.txt", ios::app);
        if (!appFile.is_open()) {
            throw FileException("Cannot open applications.txt for writing");
        }
        
        appFile << id << "|" << jobID << "|" << jobTitle << "|" 
                << company << "|Pending" << endl;
        appFile.close();
        
        cout << "Application submitted successfully!" << endl;
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Student::viewMyApplications() {
    try {
        ifstream file("applications.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open applications.txt");
        }
        
        cout << "\n=== My Applications ===" << endl;
        cout << left << setw(8) << "Job ID" 
             << setw(25) << "Job Title" 
             << setw(20) << "Company" 
             << setw(12) << "Status" << endl;
        cout << string(65, '-') << endl;
        
        bool hasApplications = false;
        string line;
        while (getline(file, line)) {
            if (line.find(id) == 0) {
                stringstream ss(line);
                string studentID, jobID, jobTitle, company, status;
                
                getline(ss, studentID, '|');
                getline(ss, jobID, '|');
                getline(ss, jobTitle, '|');
                getline(ss, company, '|');
                getline(ss, status, '|');
                
                cout << left << setw(8) << jobID
                     << setw(25) << jobTitle
                     << setw(20) << company
                     << setw(12) << status << endl;
                hasApplications = true;
            }
        }
        
        if (!hasApplications) {
            cout << "No applications found." << endl;
        }
        
        file.close();
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Student::generateSummaryReport() {
    try {
        ifstream file("applications.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open applications.txt");
        }
        
        int total = 0, approved = 0, rejected = 0, pending = 0;
        string line;
        
        while (getline(file, line)) {
            if (line.find(id) == 0) {
                total++;
                if (line.find("Approved") != string::npos) approved++;
                else if (line.find("Rejected") != string::npos) rejected++;
                else pending++;
            }
        }
        file.close();
        
        cout << "\n=== Application Summary ===" << endl;
        cout << "Student ID: " << id << endl;
        cout << "Total Applications: " << total << endl;
        cout << "Approved: " << approved << endl;
        cout << "Rejected: " << rejected << endl;
        cout << "Pending: " << pending << endl;
        
        // Save to file
        ofstream summaryFile("student_summary_" + id + ".txt");
        summaryFile << "Application Summary for Student ID: " << id << "\n";
        summaryFile << "Total Applications: " << total << "\n";
        summaryFile << "Approved: " << approved << "\n";
        summaryFile << "Rejected: " << rejected << "\n";
        summaryFile << "Pending: " << pending << "\n";
        summaryFile.close();
        
        cout << "Summary saved to student_summary_" << id << ".txt" << endl;
        
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

// Implementation of Admin methods
void Admin::displayInfo() {
    cout << "\n=== Admin Information ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Role: " << role << endl;
}

bool Admin::login() {
    try {
        string inputID, inputPassword;
        cout << "Enter Admin ID: ";
        cin >> inputID;
        cout << "Enter Password: ";
        cin >> inputPassword;
        
        ifstream file("admin_login.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open admin_login.txt");
        }
        
        string line;
        while (getline(file, line)) {
            size_t delim = line.find(',');
            if (delim != string::npos) {
                string storedID = line.substr(0, delim);
                string storedPassword = line.substr(delim + 1);
                
                if (inputID == storedID && inputPassword == storedPassword) {
                    file.close();
                    id = inputID;
                    return true;
                }
            }
        }
        file.close();
        throw LoginException("Invalid admin credentials");
    } catch (const exception& e) {
        cout << "Login Error: " << e.what() << endl;
        return false;
    }
}

void Admin::manageStudents() {
    int choice;
    cout << "\n=== Student Management ===" << endl;
    cout << "1. View All Students" << endl;
    cout << "2. Add New Student" << endl;
    cout << "3. Edit Student" << endl;
    cout << "4. Delete Student" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    // Implementation would go here
    cout << "Feature under development..." << endl;
}

void Admin::manageInternships() {
    int choice;
    cout << "\n=== Internship Management ===" << endl;
    cout << "1. View All Jobs" << endl;
    cout << "2. Add New Job" << endl;
    cout << "3. Edit Job" << endl;
    cout << "4. Delete Job" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    // Implementation would go here
    cout << "Feature under development..." << endl;
}

void Admin::viewAllApplications() {
    try {
        ifstream file("applications.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open applications.txt");
        }
        
        cout << "\n=== All Applications ===" << endl;
        cout << left << setw(12) << "Student ID" 
             << setw(8) << "Job ID" 
             << setw(25) << "Job Title" 
             << setw(20) << "Company" 
             << setw(12) << "Status" << endl;
        cout << string(77, '-') << endl;
        
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string studentID, jobID, jobTitle, company, status;
            
            getline(ss, studentID, '|');
            getline(ss, jobID, '|');
            getline(ss, jobTitle, '|');
            getline(ss, company, '|');
            getline(ss, status, '|');
            
            cout << left << setw(12) << studentID
                 << setw(8) << jobID
                 << setw(25) << jobTitle
                 << setw(20) << company
                 << setw(12) << status << endl;
        }
        file.close();
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Admin::generateReports() {
    cout << "\n=== System Reports ===" << endl;
    cout << "Total Students: " << Student::getTotalStudents() << endl;
    cout << "Total Admins: " << Admin::getTotalAdmins() << endl;
    // Additional reporting logic would go here
}

// Implementation of InternshipJob methods
void InternshipJob::display() {
    cout << "Job ID: " << jobID << " | Title: " << title 
         << " | Company: " << company << " | Deadline: " << deadline << endl;
}

void InternshipJob::display(bool showDetails) {
    display();
    if (showDetails) {
        cout << "Requirements: " << requirements << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
    }
}

void InternshipJob::display(string format) {
    if (format == "CSV") {
        cout << jobID << "," << title << "," << company << "," << deadline << endl;
    } else {
        display();
    }
}

void InternshipJob::display(bool showDetails, string format) {
    if (format == "CSV") {
        display(format);
        if (showDetails) {
            cout << "," << requirements << "," << (isActive ? "Active" : "Inactive") << endl;
        }
    } else {
        display(showDetails);
    }
}

// Implementation of StudentHashTable methods
StudentHashTable::StudentHashTable(int size) : size(size), count(0) {
    table = new Student*[size];
    for (int i = 0; i < size; i++) {
        table[i] = nullptr;
    }
}

StudentHashTable::~StudentHashTable() {
    delete[] table;
}

int StudentHashTable::hash(string key) {
    int hash = 0;
    for (char c : key) {
        hash += c;
    }
    return hash % size;
}

void StudentHashTable::insert(Student* student) {
    int index = hash(student->getID());
    // Simple linear probing for collision resolution
    while (table[index] != nullptr) {
        index = (index + 1) % size;
    }
    table[index] = student;
    count++;
}

Student* StudentHashTable::search(string studentID) {
    int index = hash(studentID);
    int originalIndex = index;
    
    while (table[index] != nullptr) {
        if (table[index]->getID() == studentID) {
            return table[index];
        }
        index = (index + 1) % size;
        if (index == originalIndex) break; // Avoid infinite loop
    }
    return nullptr;
}

void StudentHashTable::remove(string studentID) {
    int index = hash(studentID);
    int originalIndex = index;
    
    while (table[index] != nullptr) {
        if (table[index]->getID() == studentID) {
            table[index] = nullptr;
            count--;
            return;
        }
        index = (index + 1) % size;
        if (index == originalIndex) break;
    }
}

void StudentHashTable::display() {
    cout << "\n=== Hash Table Contents ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << "Index " << i << ": ";
        if (table[i] != nullptr) {
            cout << table[i]->getID() << " - " << table[i]->getName();
        } else {
            cout << "Empty";
        }
        cout << endl;
    }
}

// Implementation of InternshipSystem methods
InternshipSystem::InternshipSystem() {
    applications = new ApplicationList();
    studentHash = new StudentHashTable();
    
    // Initialize with some sample data
    initializeSampleData();
}

InternshipSystem::~InternshipSystem() {
    // Clean up dynamic memory
    for (Student* student : students) {
        delete student;
    }
    for (Admin* admin : admins) {
        delete admin;
    }
    for (InternshipJob* job : jobs) {
        delete job;
    }
    delete applications;
    delete studentHash;
}

void InternshipSystem::initializeSampleData() {
    // Sample students
    students.push_back(new Student("1231203277", "Aisyah Binti Rahman", "aisyah.rahman@mmu.edu.my", 3.70, "IT", "Java, HTML, Communication"));
    students.push_back(new Student("1231203278", "Kevin Lim", "kevin.lim@mmu.edu.my", 2.85, "Business", "Marketing, Sales, Data Analysis"));
    students.push_back(new Student("1231203279", "Nurul Izzati", "nurul.izzati@mmu.edu.my", 3.80, "IT", "Graphic Design, Adobe Illustrator, HTML"));
    students.push_back(new Student("1231203280", "Arvind Raj", "arvind.raj@mmu.edu.my", 2.50, "Accounting", "Financial Reporting, Excel, Auditing"));
    students.push_back(new Student("1231203281", "Sarah Wong", "sarah.wong@mmu.edu.my", 3.40, "Business", "Public Speaking, SEO, Business Management"));
    students.push_back(new Student("1231203282", "Faizal Mohd", "faizal.mohd@mmu.edu.my", 3.05, "IT", "C++, MySQL, Network Security"));
    students.push_back(new Student("1231203283", "Michelle Tan", "michelle.tan@mmu.edu.my", 3.75, "IT", "UI/UX Design, Figma, HTML"));
    students.push_back(new Student("1231203284", "Daniel Lee", "daniel.lee@mmu.edu.my", 2.90, "Accounting", "Bookkeeping, Tax Preparation, Excel"));
    students.push_back(new Student("1231203285", "Hema Letchumy", "hema.letchumy@mmu.edu.my", 3.20, "Business", "Project Management, Python, Agile"));
    students.push_back(new Student("1231203286", "Amirul Hakim", "amirul.hakim@mmu.edu.my", 2.10, "IT", "Linux, Troubleshooting, Python"));
    students.push_back(new Student("1231203287", "Lim Kai Jie", "lim.kj@mmu.edu.my", 3.00, "Accounting", "Payroll Management, QuickBooks, Financial Analysis"));
    students.push_back(new Student("1231203288", "Rani Devi", "rani.devi@mmu.edu.my", 3.50, "Business", "Excel, Business Analysis, Communication"));
    students.push_back(new Student("1231203289", "John Goh", "john.goh@mmu.edu.my", 2.75, "IT", "Ruby on Rails, Web Development, GitHub"));
    students.push_back(new Student("1231203290", "Siti Noraini", "siti.noraini@mmu.edu.my", 3.60, "Business", "Content Writing, Canva, Social Media Marketing"));
    students.push_back(new Student("1231203291", "Thinesh Kumar", "thinesh.kumar@mmu.edu.my", 2.40, "IT", "Android Development, Java, SQLite"));
    students.push_back(new Student("1231203292", "Leong Wei Ming", "leong.wei.ming@mmu.edu.my", 3.35, "Accounting", "C#, .NET, SQL"));
    students.push_back(new Student("1231203293", "Maya Shanti", "maya.shanti@mmu.edu.my", 3.55, "Business", "Marketing, Social Media, SEO"));
    students.push_back(new Student("1231203190", "edwin ceo", "mone@gmail.com", 3.98, "IT", "Java"));
    students.push_back(new Student("1231201130", "Riashini a/p Manoj Kumar", "riawee25@gmail.com", 3.7, "IT", "Java, HTML, PHP, C++, Python"));
}
    
// Utility functions
string InternshipSystem::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

long long InternshipSystem::stringToLongLong(const string& str) {
    try {
        return stoll(str);
    } catch (...) {
        return 0;
    }
}

float InternshipSystem::stringToFloat(const string& str) {
    try {
        return stof(str);
    } catch (...) {
        return 0.0;
    }
}

// File operations
void InternshipSystem::loadStudentsFromFile() {
    try {
        ifstream file("students.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open students.txt for reading");
        }

        students.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string id, name, email, cgpaStr, diploma, skills;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, cgpaStr, '|');
            getline(ss, diploma, '|');
            getline(ss, skills, '|');

            float cgpa = stringToFloat(cgpaStr);
            if (students.size() < MAX_STUDENTS) {
                Student* student = new Student(id, name, email, cgpa, diploma, skills);
                students.push_back(student);
                studentHash->insert(student);
            }
        }
        file.close();
        cout << "Loaded " << students.size() << " students from file.\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InternshipSystem::saveStudentsToFile() {
    try {
        ofstream file("students.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open students.txt for writing");
        }

        for (Student* student : students) {
            file << student->getID() << "|" << student->getName() << "|" << student->getEmail() << "|"
                 << fixed << setprecision(2) << student->getCGPA() << "|" << student->getDiploma() << "|"
                 << student->getSkills() << "|\n";
        }
        file.close();
        cout << "Saved " << students.size() << " students to file.\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InternshipSystem::loadJobsFromFile() {
    try {
        ifstream file("job_listings.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open job_listings.txt for reading");
        }

        jobs.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string jobID, title, company, deadline, requirements;
            getline(ss, jobID, '|');
            getline(ss, title, '|');
            getline(ss, company, '|');
            getline(ss, deadline, '|');
            getline(ss, requirements, '|');

            if (jobs.size() < MAX_JOBS) {
                InternshipJob* job = new InternshipJob(jobID, title, company, deadline, requirements);
                jobs.push_back(job);
            }
        }
        file.close();
        cout << "Loaded " << jobs.size() << " jobs from file.\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InternshipSystem::saveJobsToFile() {
    try {
        ofstream file("job_listings.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open job_listings.txt for writing");
        }

        for (InternshipJob* job : jobs) {
            file << job->getJobID() << "|" << job->getTitle() << "|" << job->getCompany() << "|"
                 << job->getDeadline() << "|" << job->getRequirements() << "|\n";
        }
        file.close();
        cout << "Saved " << jobs.size() << " jobs to file.\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InternshipSystem::loadApplicationsFromFile() {
    try {
        applications->loadFromFile("applications.txt");
        cout << "Loaded " << applications->getCount() << " applications from file.\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void InternshipSystem::saveApplicationsToFile() {
    try {
        applications->saveToFile("applications.txt");
        cout << "Saved " << applications->getCount() << " applications to file.\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

// System operations
void InternshipSystem::mainMenu() {
    int choice;
    do {
        cout << "\n=== Internship System Menu ===\n";
        cout << "1. Login as Student\n";
        cout << "2. Login as Admin\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                Student* student = new Student();
                if (student->login()) {
                    studentMenu();
                }
                delete student;
                break;
            }
            case 2: {
                Admin* admin = new Admin();
                if (admin->login()) {
                    adminMenu();
                }
                delete admin;
                break;
            }
            case 3:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
}

void InternshipSystem::studentMenu() {
    int choice;
    Student* currentStudent = nullptr;
    for (Student* student : students) {
        if (student->login()) {
            currentStudent = student;
            break;
        }
    }

    if (!currentStudent) {
        cout << "Student not found or login failed.\n";
        return;
    }

    do {
        cout << "\n=== Student Menu ===\n";
        cout << "1. View Internships\n";
        cout << "2. Apply for Internship\n";
        cout << "3. View My Applications\n";
        cout << "4. Generate Summary Report\n";
        cout << "5. Update Profile\n";
        cout << "6. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: currentStudent->viewInternships(); break;
            case 2: currentStudent->applyForInternship(); break;
            case 3: currentStudent->viewMyApplications(); break;
            case 4: currentStudent->generateSummaryReport(); break;
            case 5: currentStudent->updateProfile(); break;
            case 6: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void InternshipSystem::adminMenu() {
    int choice;
    Admin* currentAdmin = nullptr;
    for (Admin* admin : admins) {
        if (admin->login()) {
            currentAdmin = admin;
            break;
        }
    }

    if (!currentAdmin) {
        cout << "Admin not found or login failed.\n";
        return;
    }

    do {
        cout << "\n=== Admin Menu ===\n";
        cout << "1. Manage Students\n";
        cout << "2. Manage Internships\n";
        cout << "3. View All Applications\n";
        cout << "4. Generate Reports\n";
        cout << "5. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: currentAdmin->manageStudents(); break;
            case 2: currentAdmin->manageInternships(); break;
            case 3: currentAdmin->viewAllApplications(); break;
            case 4: currentAdmin->generateReports(); break;
            case 5: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Sorting algorithms (manually implemented)
void InternshipSystem::selectionSortStudentsByCGPA() {
    for (size_t i = 0; i < students.size() - 1; i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < students.size(); j++) {
            if (students[j]->getCGPA() < students[minIndex]->getCGPA()) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Student* temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
    cout << "Students sorted by CGPA.\n";
}

void InternshipSystem::selectionSortStudentsByID() {
    for (size_t i = 0; i < students.size() - 1; i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < students.size(); j++) {
            if (students[j]->getID() < students[minIndex]->getID()) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Student* temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
    cout << "Students sorted by ID.\n";
}

void InternshipSystem::bubbleSortJobsByDeadline() {
    for (size_t i = 0; i < jobs.size() - 1; i++) {
        for (size_t j = 0; j < jobs.size() - i - 1; j++) {
            // Simple string comparison for deadlines (assuming YYYY-MM-DD format)
            if (jobs[j]->getDeadline() > jobs[j + 1]->getDeadline()) {
                InternshipJob* temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }
    cout << "Jobs sorted by deadline.\n";
}

// Searching algorithms (manually implemented)
int InternshipSystem::binarySearchStudentByID(string targetID) {
    selectionSortStudentsByID(); // Ensure sorted array for binary search
    int left = 0, right = students.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (students[mid]->getID() == targetID) return mid;
        if (students[mid]->getID() < targetID) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

Student* InternshipSystem::hashSearchStudent(string studentID) {
    return studentHash->search(studentID);
}

// System management
void InternshipSystem::registerStudent() {
    if (students.size() >= MAX_STUDENTS) {
        cout << "Maximum student limit reached.\n";
        return;
    }

    string id, name, email, diploma, skills;
    float cgpa;
    cout << "Enter Student ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter CGPA: ";
    cin >> cgpa;
    cin.ignore();
    cout << "Enter Diploma: ";
    getline(cin, diploma);
    cout << "Enter Skills: ";
    getline(cin, skills);

    Student* student = new Student(id, name, email, cgpa, diploma, skills);
    students.push_back(student);
    studentHash->insert(student);
    saveStudentsToFile();
    cout << "Student registered successfully.\n";
}

void InternshipSystem::registerAdmin() {
    if (admins.size() >= 10) { // Arbitrary limit for admins
        cout << "Maximum admin limit reached.\n";
        return;
    }

    string id, name, email, password;
    cout << "Enter Admin ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Password: ";
    getline(cin, password);

    Admin* admin = new Admin(id, name, email, password);
    admins.push_back(admin);

    // Save to admin_login.txt
    ofstream file("admin_login.txt", ios::app);
    if (file.is_open()) {
        file << id << "," << password << "\n";
        file.close();
        cout << "Admin registered successfully.\n";
    } else {
        cout << "Error saving admin credentials.\n";
    }
}

void InternshipSystem::addInternshipJob() {
    if (jobs.size() >= MAX_JOBS) {
        cout << "Maximum job limit reached.\n";
        return;
    }

    string id, title, company, deadline, requirements;
    cout << "Enter Job ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Title: ";
    getline(cin, title);
    cout << "Enter Company: ";
    getline(cin, company);
    cout << "Enter Deadline (YYYY-MM-DD): ";
    getline(cin, deadline);
    cout << "Enter Requirements: ";
    getline(cin, requirements);

    InternshipJob* job = new InternshipJob(id, title, company, deadline, requirements);
    jobs.push_back(job);
    saveJobsToFile();
    cout << "Internship job added successfully.\n";
}

void InternshipSystem::displayAllStudents() {
    cout << "\n=== All Students ===\n";
    for (Student* student : students) {
        student->displayInfo();
        cout << "------------------------\n";
    }
}

void InternshipSystem::displayAllJobs() {
    cout << "\n=== All Jobs ===\n";
    for (InternshipJob* job : jobs) {
        job->display(true); // Show details
        cout << "------------------------\n";
    }
}

void InternshipSystem::displayStatistics() {
    cout << "\n=== System Statistics ===\n";
    cout << "Total Students: " << students.size() << "\n";
    cout << "Total Jobs: " << jobs.size() << "\n";
    cout << "Total Applications: " << applications->getCount() << "\n";
    cout << "Student Hash Table Load Factor: " << studentHash->getLoadFactor() << "\n";
}

// Friend functions
void saveSystemData(const InternshipSystem& system) {
    system.saveStudentsToFile();
    system.saveJobsToFile();
    system.saveApplicationsToFile();
}

void loadSystemData(InternshipSystem& system) {
    system.loadStudentsFromFile();
    system.loadJobsFromFile();
    system.loadApplicationsFromFile();
}

// Main function
int main() {
    InternshipSystem system;
    loadSystemData(system);
    system.mainMenu();
    saveSystemData(system);
    return 0;
}
