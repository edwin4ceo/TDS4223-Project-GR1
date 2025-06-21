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
#include <ctime>
#include <cctype>
using namespace std;

// Constants
const int MAX_STUDENTS = 100;
const int MAX_APPLICATIONS = 200;
const int TABLE_SIZE = 10;
const int MAX_JOBS = 50;
const int MAX_STAFF = 20;
const string LOG_FILE = "system_log.txt";

// Forward declarations
class Person;
class Student;
class Admin;
class Staff;
class InternshipJob;
class Application;

// Exception classes for error handling
class FileException : public exception 
{
private:
    string message;
public:
    FileException(const string& msg) : message(msg) {}
    virtual ~FileException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class LoginException : public exception 
{
private:
    string message;
public:
    LoginException(const string& msg) : message(msg) {}
    virtual ~LoginException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class DataException : public exception 
{
private:
    string message;
public:
    DataException(const string& msg) : message(msg) {}
    virtual ~DataException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class SecurityException : public exception 
{
private:
    string message;
public:
    SecurityException(const string& msg) : message(msg) {}
    virtual ~SecurityException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

// Log entry structure
struct LogEntry 
{
    string timestamp;
    string eventType;
    string details;
    
    LogEntry(string type, string desc) {
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp = timestamp.substr(0, timestamp.length()-1); // Remove newline
        eventType = type;
        details = desc;
    }
    
    string toString() const {
        return timestamp + " | " + eventType + " | " + details;
    }
};

// Dynamic Non-Primitive Data Structure - Linked List for Applications
struct ApplicationNode 
{
    string studentID;
    string jobID;
    string jobTitle;
    string company;
    string status;
    ApplicationNode* next;
    
    ApplicationNode(string sid, string jid, string title, string comp, string stat) 
        : studentID(sid), jobID(jid), jobTitle(title), company(comp), status(stat), next(NULL) {}
};

class ApplicationList 
{
private:
    ApplicationNode* head;
    int count;
    
public:
    ApplicationList() : head(NULL), count(0) {}
    
    ~ApplicationList() 
    {
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
class Person 
{
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
class Student : public Person 
{
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
    
    void displayInfo();
    bool login();
    
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
    
    friend void saveStudentToFile(Student& student, ostream& file);
    friend class Admin;
};

// Derived class - Admin
class Admin : public Person 
{
private:
    string password;
    string role;
    static int totalAdmins;
    
public:
    Admin() : Person(), password(""), role("Admin") { totalAdmins++; }
    Admin(string id, string name, string email, string password)
        : Person(id, name, email), password(password), role("Admin") { totalAdmins++; }
    
    ~Admin() { totalAdmins--; }
    
    void displayInfo();
    bool login();
    
    void manageStudents();
    void manageInternships();
    void viewAllApplications();
    void generateReports();
    void manageStaff();
    
    static int getTotalAdmins() { return totalAdmins; }
    
    friend void saveAdminToFile(Admin& admin, ostream& file);
    friend class InternshipSystem;
};

// New Staff class
class Staff : public Person 
{
private:
    string department;
    string position;
    string password;
    static int totalStaff;
    
public:
    Staff() : Person(), department(""), position(""), password("") { totalStaff++; }
    Staff(string id, string name, string email, string dept, string pos, string pwd)
        : Person(id, name, email), department(dept), position(pos), password(pwd) { totalStaff++; }
    
    ~Staff() { totalStaff--; }
    
    void displayInfo();
    bool login();
    
    void processApplications();
    void generateDepartmentReport();
    
    string getDepartment() const { return department; }
    string getPosition() const { return position; }
    
    void setDepartment(string dept) { department = dept; }
    void setPosition(string pos) { position = pos; }
    
    static int getTotalStaff() { return totalStaff; }
    
    friend void saveStaffToFile(Staff& staff, ostream& file);
};

// InternshipJob class
class InternshipJob 
{
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
    
    friend void saveJobToFile(InternshipJob& job, ostream& file);
    friend class Admin;
    friend class Student;
};

// Hash Table for fast student lookup
class StudentHashTable 
{
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
class InternshipSystem 
{
private:
    vector<Student*> students;
    vector<Admin*> admins;
    vector<Staff*> staffMembers;
    vector<InternshipJob*> jobs;
    ApplicationList* applications;
    StudentHashTable* studentHash;
    
    void initializeSampleData();
    
    // Utility functions
    string trim(const string& str);
    long long stringToLongLong(const string& str);
    float stringToFloat(const string& str);
    string encryptPassword(const string& password);
    string decryptPassword(const string& encrypted);
    void logEvent(const string& eventType, const string& details);
    
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
    void loadStaffFromFile();
    void saveStaffToFile();
    void loadAdminsFromFile();
    void saveAdminsToFile();
    
    // System operations
    void mainMenu();
    void studentMenu();
    void adminMenu();
    void staffMenu();
    
    // Sorting algorithms (manually implemented)
    void selectionSortStudentsByCGPA();
    void selectionSortStudentsByID();
    void bubbleSortJobsByDeadline();
    void mergeSortJobsByCompany(int low, int high);
    void mergeJobsByCompany(int low, int mid, int high);
    
    // Searching algorithms (manually implemented)
    int binarySearchStudentByID(string targetID);
    Student* hashSearchStudent(string studentID);
    int linearSearchJobByTitle(string title);
    
    // System management
    void registerStudent();
    void registerAdmin();
    void registerStaff();
    void addInternshipJob();
    void displayAllStudents();
    void displayAllJobs();
    void displayAllStaff();
    void displayStatistics();
    
    friend void saveSystemData(InternshipSystem& system);
    friend void loadSystemData(InternshipSystem& system);
};

// Static member initialization
int Student::totalStudents = 0;
int Admin::totalAdmins = 0;
int Staff::totalStaff = 0;

// Implementation of ApplicationList methods
void ApplicationList::insert(string studentID, string jobID, string jobTitle, string company, string status) 
{
    ApplicationNode* newNode = new ApplicationNode(studentID, jobID, jobTitle, company, status);
    if (head == NULL) {
        head = newNode;
    } else {
        ApplicationNode* current = head;
        while (current->next != NULL) 
        {
            current = current->next;
        }
        current->next = newNode;
    }
    count++;
}

void ApplicationList::display() 
{
    cout << "\n=== All Applications ===" << endl;
    cout << left << setw(12) << "Student ID" 
         << setw(8) << "Job ID" 
         << setw(25) << "Job Title" 
         << setw(20) << "Company" 
         << setw(12) << "Status" << endl;
    cout << string(77, '-') << endl;
    
    ApplicationNode* current = head;
    while (current != NULL) {
        cout << left << setw(12) << current->studentID
             << setw(8) << current->jobID
             << setw(25) << current->jobTitle
             << setw(20) << current->company
             << setw(12) << current->status << endl;
        current = current->next;
    }
}

void ApplicationList::clear() 
{
    while (head != NULL) {
        ApplicationNode* temp = head;
        head = head->next;
        delete temp;
    }
    count = 0;
}

bool ApplicationList::search(string studentID, string jobID) 
{
    ApplicationNode* current = head;
    while (current != NULL) {
        if (current->studentID == studentID && current->jobID == jobID) 
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

void ApplicationList::saveToFile(const string& filename) 
{
    try {
        ofstream file(filename.c_str());
        if (!file.is_open()) 
        {
            throw FileException("Cannot open file for writing: " + filename);
        }
        
        ApplicationNode* current = head;
        while (current != NULL) 
        {
            file << current->studentID << "|" << current->jobID << "|"
                 << current->jobTitle << "|" << current->company << "|"
                 << current->status << endl;
            current = current->next;
        }
        file.close();
    } catch (const FileException& e) 
    {
        cout << "Error: " << e.what() << endl;
    }
}

void ApplicationList::loadFromFile(const string& filename) 
{
    try {
        ifstream file(filename.c_str());
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
    } catch (const FileException& e) 
    {
        cout << "Error: " << e.what() << endl;
    }
}

// Implementation of Person methods
void Person::setInfo(string id) 
{
    this->id = id;
}

void Person::setInfo(string id, string name) 
{
    this->id = id;
    this->name = name;
}

void Person::setInfo(string id, string name, string email) 
{
    this->id = id;
    this->name = name;
    this->email = email;
}

// Implementation of Student methods
void Student::displayInfo() 
{
    cout << "\n=== Student Information ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "CGPA: " << fixed << setprecision(2) << cgpa << endl;
    cout << "Diploma: " << diploma << endl;
    cout << "Skills: " << skills << endl;
}

bool Student::login() 
{
    try {
        string inputID;
        cout << "Enter Student ID: ";
        cin >> inputID;
        
        ifstream file("students.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open students.txt");
        }
        
        string line;
        while (getline(file, line)) 
        {
            if (line.find(inputID) != string::npos) 
            {
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

void Student::updateProfile() 
{
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

void Student::updateProfile(string newEmail) 
{
    email = newEmail;
    cout << "Email updated successfully!" << endl;
}

void Student::updateProfile(string newEmail, float newCGPA) 
{
    email = newEmail;
    cgpa = newCGPA;
    cout << "Email and CGPA updated successfully!" << endl;
}

void Student::updateProfile(string newEmail, float newCGPA, string newSkills) 
{
    email = newEmail;
    cgpa = newCGPA;
    skills = newSkills;
    cout << "Profile updated successfully!" << endl;
}

void Student::viewInternships() 
{
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
        while (getline(file, line)) 
        {
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

void Student::applyForInternship() 
{
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
        
        while (getline(jobFile, line)) 
        {
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

void Student::viewMyApplications() 
{
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

void Student::generateSummaryReport() 
{
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
        string filename = "student_summary_" + id + ".txt";
        ofstream summaryFile(filename.c_str());
        summaryFile << "Application Summary for Student ID: " << id << "\n";
        summaryFile << "Total Applications: " << total << "\n";
        summaryFile << "Approved: " << approved << "\n";
        summaryFile << "Rejected: " << rejected << "\n";
        summaryFile << "Pending: " << pending << "\n";
        summaryFile.close();
        
        cout << "Summary saved to student_summary_" << id << ".txt" << endl;
        
    } catch (const FileException& e) 
    {
        cout << "Error: " << e.what() << endl;
    }
}

// Implementation of Admin methods
void Admin::displayInfo() 
{
    cout << "\n=== Admin Information ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Role: " << role << endl;
}

bool Admin::login() 
{
    try {
        string inputID, inputPassword;
        cout << "Enter Admin ID: ";
        cin >> inputID;
        cout << "Enter Password: ";
        cin >> inputPassword;
        
        ifstream file("admin_login.txt");
        if (!file.is_open()) 
        {
            throw FileException("Cannot open admin_login.txt");
        }
        
        string line;
        while (getline(file, line)) 
        {
            size_t delim = line.find(',');
            if (delim != string::npos) 
            {
                string storedID = line.substr(0, delim);
                string storedPassword = line.substr(delim + 1);
                
                if (inputID == storedID && inputPassword == storedPassword) 
                {
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

void Admin::manageStudents() 
{
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

void Admin::manageInternships() 
{
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

void Admin::manageStaff() 
{
    int choice;
    cout << "\n=== Staff Management ===" << endl;
    cout << "1. View All Staff" << endl;
    cout << "2. Add New Staff" << endl;
    cout << "3. Edit Staff" << endl;
    cout << "4. Delete Staff" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    cout << "Staff management feature under development..." << endl;
}

void Admin::viewAllApplications() 
{
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
        while (getline(file, line)) 
        {
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
    } catch (const FileException& e) 
    {
        cout << "Error: " << e.what() << endl;
    }
}

void Admin::generateReports() 
{
    cout << "\n=== System Reports ===" << endl;
    cout << "Total Students: " << Student::getTotalStudents() << endl;
    cout << "Total Admins: " << Admin::getTotalAdmins() << endl;
    cout << "Total Staff: " << Staff::getTotalStaff() << endl;
    // Additional reporting logic would go here
}

// Implementation of Staff methods
void Staff::displayInfo() 
{
    cout << "\n=== Staff Information ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Department: " << department << endl;
    cout << "Position: " << position << endl;
}

bool Staff::login() 
{
    try {
        string inputID, inputPassword;
        cout << "Enter Staff ID: ";
        cin >> inputID;
        cout << "Enter Password: ";
        cin >> inputPassword;
        
        ifstream file("staff.txt");
        if (!file.is_open()) 
        {
            throw FileException("Cannot open staff.txt");
        }
        
        string line;
        while (getline(file, line)) 
        {
            stringstream ss(line);
            string storedID, storedName, storedEmail, storedDept, storedPos, storedPwd;
            
            getline(ss, storedID, '|');
            getline(ss, storedName, '|');
            getline(ss, storedEmail, '|');
            getline(ss, storedDept, '|');
            getline(ss, storedPos, '|');
            getline(ss, storedPwd, '|');
            
            if (inputID == storedID && inputPassword == storedPwd) 
            {
                file.close();
                id = inputID;
                name = storedName;
                email = storedEmail;
                department = storedDept;
                position = storedPos;
                return true;
            }
        }
        file.close();
        throw LoginException("Invalid staff credentials");
    } catch (const exception& e) {
        cout << "Login Error: " << e.what() << endl;
        return false;
    }
}

void Staff::processApplications() 
{
    cout << "\nProcessing internship applications..." << endl;
    // Implementation would go here
    cout << "Application processing feature under development..." << endl;
}

void Staff::generateDepartmentReport() 
{
    cout << "\nGenerating department report for " << department << "..." << endl;
    // Implementation would go here
    cout << "Department reporting feature under development..." << endl;
}

// Implementation of InternshipJob methods
void InternshipJob::display() 
{
    cout << "Job ID: " << jobID << " | Title: " << title 
         << " | Company: " << company << " | Deadline: " << deadline << endl;
}

void InternshipJob::display(bool showDetails) 
{
    display();
    if (showDetails) 
    {
        cout << "Requirements: " << requirements << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
    }
}

void InternshipJob::display(string format) 
{
    if (format == "CSV") 
    {
        cout << jobID << "," << title << "," << company << "," << deadline << endl;
    } else {
        display();
    }
}

void InternshipJob::display(bool showDetails, string format) 
{
    if (format == "CSV") 
    {
        display(format);
        if (showDetails) 
        {
            cout << "," << requirements << "," << (isActive ? "Active" : "Inactive") << endl;
        }
    } else {
        display(showDetails);
    }
}

// Implementation of StudentHashTable methods
StudentHashTable::StudentHashTable(int size) : size(size), count(0) 
{
    table = new Student*[size];
    for (int i = 0; i < size; i++) 
    {
        table[i] = NULL;
    }
}

StudentHashTable::~StudentHashTable() 
{
    for (int i = 0; i < size; i++) {
        if (table[i] != NULL) {
            delete table[i];
        }
    }
    delete[] table;
}

int StudentHashTable::hash(string key) 
{
    int hash = 0;
    for (size_t i = 0; i < key.length(); i++) 
    {
        hash = (hash * 31 + key[i]) % size;
    }
    return hash % size;
}

void StudentHashTable::insert(Student* student) 
{
    int index = hash(student->getID());
    // Simple linear probing for collision resolution
    int originalIndex = index;
    while (table[index] != NULL) {
        index = (index + 1) % size;
        if (index == originalIndex) {
            throw runtime_error("Hash table is full");
        }
    }
    table[index] = student;
    count++;
}

Student* StudentHashTable::search(string studentID) 
{
    int index = hash(studentID);
    int originalIndex = index;
    
    while (table[index] != NULL) 
    {
        if (table[index]->getID() == studentID) 
        {
            return table[index];
        }
        index = (index + 1) % size;
        if (index == originalIndex) break; // Avoid infinite loop
    }
    return NULL;
}

void StudentHashTable::remove(string studentID) 
{
    int index = hash(studentID);
    int originalIndex = index;
    
    while (table[index] != NULL) 
    {
        if (table[index]->getID() == studentID) 
        {
            delete table[index];
            table[index] = NULL;
            count--;
            return;
        }
        index = (index + 1) % size;
        if (index == originalIndex) break;
    }
}

void StudentHashTable::display() 
{
    cout << "\n=== Hash Table Contents ===" << endl;
    for (int i = 0; i < size; i++) 
    {
        cout << "Index " << i << ": ";
        if (table[i] != NULL) {
            cout << table[i]->getID() << " - " << table[i]->getName();
        } else {
            cout << "Empty";
        }
        cout << endl;
    }
}

// Friend functions implementations
void saveStudentToFile(Student& student, ostream& file) 
{
    file << student.id << "|" << student.name << "|" << student.email << "|"
         << fixed << setprecision(2) << student.cgpa << "|" 
         << student.diploma << "|" << student.skills << "|\n";
}

void saveAdminToFile(Admin& admin, ostream& file) 
{
    file << admin.id << "|" << admin.name << "|" << admin.email << "|"
         << admin.password << "|\n";
}

void saveStaffToFile(Staff& staff, ostream& file) 
{
    file << staff.id << "|" << staff.name << "|" << staff.email << "|"
         << staff.department << "|" << staff.position << "|"
         << staff.password << "|\n";
}

void saveJobToFile(InternshipJob& job, ostream& file) 
{
    file << job.jobID << "|" << job.title << "|" << job.company << "|"
         << job.deadline << "|" << job.requirements << "|\n";
}

// Implementation of InternshipSystem methods
InternshipSystem::InternshipSystem() 
{
    applications = new ApplicationList();
    studentHash = new StudentHashTable();
    
    // Initialize with some sample data
    initializeSampleData();
}

InternshipSystem::~InternshipSystem() 
{
    // Clean up dynamic memory
    for (size_t i = 0; i < students.size(); i++) 
    {
        delete students[i];
    }
    for (size_t i = 0; i < admins.size(); i++) 
    {
        delete admins[i];
    }
    for (size_t i = 0; i < staffMembers.size(); i++) 
    {
        delete staffMembers[i];
    }
    for (size_t i = 0; i < jobs.size(); i++) 
    {
        delete jobs[i];
    }
    delete applications;
    delete studentHash;
}

void InternshipSystem::initializeSampleData() 
{
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
    
    // Sample staff
    staffMembers.push_back(new Staff("STF1001", "Dr. Lim Wei Chen", "lim.wc@mmu.edu.my", "Career Services", "Manager", "pass123"));
    staffMembers.push_back(new Staff("STF1002", "Ms. Nor Azlina", "nor.azlina@mmu.edu.my", "IT Department", "Coordinator", "pwd456"));
    staffMembers.push_back(new Staff("STF1003", "Mr. Rajesh Kumar", "rajesh.k@mmu.edu.my", "Business Faculty", "Advisor", "secure789"));
    
    // Sample jobs
    jobs.push_back(new InternshipJob("JOB1001", "Software Developer Intern", "Tech Solutions Sdn Bhd", "2023-12-15", "C++, Python, Problem Solving"));
    jobs.push_back(new InternshipJob("JOB1002", "Marketing Intern", "Global Marketing Group", "2023-11-30", "Communication, Creativity, Social Media"));
    jobs.push_back(new InternshipJob("JOB1003", "Finance Intern", "Capital Investments Berhad", "2024-01-10", "Accounting, Excel, Analytical Skills"));
    
    logEvent("SYSTEM", "Sample data initialized");
}

// Utility functions
string InternshipSystem::trim(const string& str) 
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

long long InternshipSystem::stringToLongLong(const string& str) 
{
    try {
        return atoll(str.c_str());
    } catch (...) {
        return 0;
    }
}

float InternshipSystem::stringToFloat(const string& str) 
{
    try {
        return (float)atof(str.c_str());
    } catch (...) {
        return 0.0;
    }
}

string InternshipSystem::encryptPassword(const string& password) 
{
    string encrypted = password;
    for (size_t i = 0; i < encrypted.length(); i++) {
        char c = encrypted[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = ((c - base + 3) % 26) + base;
        } else if (isdigit(c)) {
            c = ((c - '0' + 5) % 10) + '0';
        }
    }
    return encrypted;
}

string InternshipSystem::decryptPassword(const string& encrypted) 
{
    string decrypted = encrypted;
    for (size_t i = 0; i < decrypted.length(); i++) {
        char c = decrypted[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = ((c - base - 3 + 26) % 26) + base;
        } else if (isdigit(c)) {
            c = ((c - '0' - 5 + 10) % 10) + '0';
        }
    }
    return decrypted;
}

void InternshipSystem::logEvent(const string& eventType, const string& details) 
{
    try {
        ofstream logFile(LOG_FILE.c_str(), ios::app);
        if (!logFile.is_open()) {
            throw FileException("Cannot open log file: " + LOG_FILE);
        }
        
        LogEntry entry(eventType, details);
        logFile << entry.toString() << endl;
        logFile.close();
    } catch (const FileException& e) {
        cerr << "Logging Error: " << e.what() << endl;
    }
}

// File operations
void InternshipSystem::loadStudentsFromFile() 
{
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
            if (students.size() < MAX_STUDENTS) 
            {
                Student* student = new Student(id, name, email, cgpa, diploma, skills);
                students.push_back(student);
                studentHash->insert(student);
            }
        }
        file.close();
        cout << "Loaded " << students.size() << " students from file.\n";
        logEvent("SYSTEM", "Students loaded from file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::saveStudentsToFile() 
{
    try {
        ofstream file("students.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open students.txt for writing");
        }

        for (size_t i = 0; i < students.size(); i++) 
        {
            saveStudentToFile(*students[i], file);
        }
        file.close();
        cout << "Saved " << students.size() << " students to file.\n";
        logEvent("SYSTEM", "Students saved to file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::loadStaffFromFile() 
{
    try {
        ifstream file("staff.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open staff.txt for reading");
        }

        staffMembers.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string id, name, email, department, position, password;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, department, '|');
            getline(ss, position, '|');
            getline(ss, password, '|');

            if (staffMembers.size() < MAX_STAFF) 
            {
                Staff* staff = new Staff(id, name, email, department, position, password);
                staffMembers.push_back(staff);
            }
        }
        file.close();
        cout << "Loaded " << staffMembers.size() << " staff members from file.\n";
        logEvent("SYSTEM", "Staff loaded from file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::saveStaffToFile() 
{
    try {
        ofstream file("staff.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open staff.txt for writing");
        }

        for (size_t i = 0; i < staffMembers.size(); i++) 
        {
            ::saveStaffToFile(*staffMembers[i], file);  // Use global scope
        }
        file.close();
        cout << "Saved " << staffMembers.size() << " staff members to file.\n";
        logEvent("SYSTEM", "Staff saved to file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::loadAdminsFromFile() 
{
    try {
        ifstream file("admin_login.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open admin_login.txt for reading");
        }

        admins.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string id, name, email, password;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, password, '|');

            Admin* admin = new Admin(id, name, email, password);
            admins.push_back(admin);
        }
        file.close();
        cout << "Loaded " << admins.size() << " admins from file.\n";
        logEvent("SYSTEM", "Admins loaded from file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::saveAdminsToFile() 
{
    try {
        ofstream file("admin_login.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open admin_login.txt for writing");
        }

        for (size_t i = 0; i < admins.size(); i++) 
        {
            saveAdminToFile(*admins[i], file);
        }
        file.close();
        cout << "Saved " << admins.size() << " admins to file.\n";
        logEvent("SYSTEM", "Admins saved to file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::loadJobsFromFile() 
{
    try {
        ifstream file("job_listings.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open job_listings.txt for reading");
        }

        jobs.clear();
        string line;
        while (getline(file, line)) 
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string jobID, title, company, deadline, requirements;
            getline(ss, jobID, '|');
            getline(ss, title, '|');
            getline(ss, company, '|');
            getline(ss, deadline, '|');
            getline(ss, requirements, '|');

            if (jobs.size() < MAX_JOBS) 
            {
                InternshipJob* job = new InternshipJob(jobID, title, company, deadline, requirements);
                jobs.push_back(job);
            }
        }
        file.close();
        cout << "Loaded " << jobs.size() << " jobs from file.\n";
        logEvent("SYSTEM", "Jobs loaded from file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::saveJobsToFile() 
{
    try {
        ofstream file("job_listings.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open job_listings.txt for writing");
        }

        for (size_t i = 0; i < jobs.size(); i++) 
        {
            saveJobToFile(*jobs[i], file);
        }
        file.close();
        cout << "Saved " << jobs.size() << " jobs to file.\n";
        logEvent("SYSTEM", "Jobs saved to file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::loadApplicationsFromFile() 
{
    try {
        applications->loadFromFile("applications.txt");
        cout << "Loaded " << applications->getCount() << " applications from file.\n";
        logEvent("SYSTEM", "Applications loaded from file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

void InternshipSystem::saveApplicationsToFile() 
{
    try {
        applications->saveToFile("applications.txt");
        cout << "Saved " << applications->getCount() << " applications to file.\n";
        logEvent("SYSTEM", "Applications saved to file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}

// System operations
void InternshipSystem::mainMenu() 
{
    int choice;
    do {
        cout << "\n=== Internship System Menu ===\n";
        cout << "1. Login as Student\n";
        cout << "2. Login as Admin\n";
        cout << "3. Login as Staff\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) 
        {
            case 1: 
            {
                Student* student = new Student();
                if (student->login()) 
                {
                    studentMenu();
                }
                delete student;
                break;
            }
            case 2: 
            {
                Admin* admin = new Admin();
                if (admin->login()) 
                {
                    adminMenu();
                }
                delete admin;
                break;
            }
            case 3:
            {
                Staff* staff = new Staff();
                if (staff->login()) 
                {
                    staffMenu();
                }
                delete staff;
                break;
            }
            case 4:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}

void InternshipSystem::studentMenu() 
{
    int choice;
    Student* currentStudent = NULL;
    for (size_t i = 0; i < students.size(); i++) 
    {
        if (students[i]->login()) 
        {
            currentStudent = students[i];
            break;
        }
    }

    if (!currentStudent) 
    {
        cout << "Student not found or login failed.\n";
        return;
    }

    do {
        cout << "\n=== Student Menu (" << currentStudent->getName() << ") ===\n";
        cout << "1. View Internships\n";
        cout << "2. Apply for Internship\n";
        cout << "3. View My Applications\n";
        cout << "4. Generate Summary Report\n";
        cout << "5. Update Profile\n";
        cout << "6. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) 
        {
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

void InternshipSystem::adminMenu() 
{
    int choice;
    Admin* currentAdmin = NULL;
    for (size_t i = 0; i < admins.size(); i++) 
    {
        if (admins[i]->login()) 
        {
            currentAdmin = admins[i];
            break;
        }
    }

    if (!currentAdmin) 
    {
        cout << "Admin not found or login failed.\n";
        return;
    }

    do {
        cout << "\n=== Admin Menu (" << currentAdmin->getName() << ") ===\n";
        cout << "1. Manage Students\n";
        cout << "2. Manage Internships\n";
        cout << "3. Manage Staff\n";
        cout << "4. View All Applications\n";
        cout << "5. Generate Reports\n";
        cout << "6. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) 
        {
            case 1: currentAdmin->manageStudents(); break;
            case 2: currentAdmin->manageInternships(); break;
            case 3: currentAdmin->manageStaff(); break;
            case 4: currentAdmin->viewAllApplications(); break;
            case 5: currentAdmin->generateReports(); break;
            case 6: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void InternshipSystem::staffMenu() 
{
    int choice;
    Staff* currentStaff = NULL;
    for (size_t i = 0; i < staffMembers.size(); i++) 
    {
        if (staffMembers[i]->login()) 
        {
            currentStaff = staffMembers[i];
            break;
        }
    }

    if (!currentStaff) 
    {
        cout << "Staff not found or login failed.\n";
        return;
    }

    do {
        cout << "\n=== Staff Menu (" << currentStaff->getName() << ") ===\n";
        cout << "1. Process Applications\n";
        cout << "2. Generate Department Report\n";
        cout << "3. View Department Internships\n";
        cout << "4. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) 
        {
            case 1: currentStaff->processApplications(); break;
            case 2: currentStaff->generateDepartmentReport(); break;
            case 3: 
                cout << "\nInternships for " << currentStaff->getDepartment() << ":\n";
                // Implementation would show department-specific internships
                cout << "Department internship view under development...\n";
                break;
            case 4: cout << "Logging out...\n"; return;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Sorting algorithms
void InternshipSystem::selectionSortStudentsByCGPA() 
{
    for (size_t i = 0; i < students.size() - 1; i++) 
    {
        size_t minIndex = i;
        for (size_t j = i + 1; j < students.size(); j++) 
        {
            if (students[j]->getCGPA() < students[minIndex]->getCGPA()) 
            {
                minIndex = j;
            }
        }
        if (minIndex != i) 
        {
            Student* temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
    cout << "Students sorted by CGPA.\n";
    logEvent("SYSTEM", "Students sorted by CGPA");
}

void InternshipSystem::selectionSortStudentsByID() 
{
    for (size_t i = 0; i < students.size() - 1; i++) 
    {
        size_t minIndex = i;
        for (size_t j = i + 1; j < students.size(); j++) 
        {
            if (students[j]->getID() < students[minIndex]->getID()) 
            {
                minIndex = j;
            }
        }
        if (minIndex != i) 
        {
            Student* temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
    cout << "Students sorted by ID.\n";
    logEvent("SYSTEM", "Students sorted by ID");
}

void InternshipSystem::mergeSortJobsByCompany(int low, int high) 
{
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSortJobsByCompany(low, mid);
        mergeSortJobsByCompany(mid + 1, high);
        mergeJobsByCompany(low, mid, high);
    }
}

void InternshipSystem::mergeJobsByCompany(int low, int mid, int high) 
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

    vector<InternshipJob*> left(n1), right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = jobs[low + i];
    for (int j = 0; j < n2; j++)
        right[j] = jobs[mid + 1 + j];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) 
	{
        if (left[i]->getCompany() <= right[j]->getCompany()) 
		{
            jobs[k] = left[i];
            i++;
        } else {
            jobs[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) 
	{
        jobs[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) 
	{
        jobs[k] = right[j];
        j++;
        k++;
    }
}

// Searching algorithms
int InternshipSystem::binarySearchStudentByID(string targetID) 
{
    selectionSortStudentsByID(); // Ensure sorted array for binary search
    int left = 0, right = students.size() - 1;
    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (students[mid]->getID() == targetID) return mid;
        if (students[mid]->getID() < targetID) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int InternshipSystem::linearSearchJobByTitle(string title) 
{
    for (size_t i = 0; i < jobs.size(); i++) 
    {
        if (jobs[i]->getTitle().find(title) != string::npos) 
		{
            return i;
        }
    }
    return -1;
}

// System management
void InternshipSystem::registerStaff() 
{
    if (staffMembers.size() >= MAX_STAFF) 
    {
        cout << "Maximum staff limit reached.\n";
        return;
    }

    string id, name, email, department, position, password;
    cout << "Enter Staff ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Department: ";
    getline(cin, department);
    cout << "Enter Position: ";
    getline(cin, position);
    cout << "Enter Password: ";
    getline(cin, password);

    // Encrypt password
    string encrypted = encryptPassword(password);

    Staff* staff = new Staff(id, name, email, department, position, encrypted);
    staffMembers.push_back(staff);
    saveStaffToFile();
    cout << "Staff registered successfully.\n";
    logEvent("SYSTEM", "Staff registered: " + id);
}

void InternshipSystem::displayAllStaff() 
{
    cout << "\n=== All Staff Members ===\n";
    for (size_t i = 0; i < staffMembers.size(); i++) 
    {
        staffMembers[i]->displayInfo();
        cout << "------------------------\n";
    }
}

void InternshipSystem::displayStatistics() 
{
    cout << "\n=== System Statistics ===" << endl;
    cout << "Total Students: " << students.size() << endl;
    cout << "Total Admins: " << admins.size() << endl;
    cout << "Total Staff: " << staffMembers.size() << endl;
    cout << "Total Jobs: " << jobs.size() << endl;
    cout << "Total Applications: " << applications->getCount() << endl;
    cout << "Student Hash Table Load Factor: " << studentHash->getLoadFactor() << endl;
    logEvent("SYSTEM", "Statistics viewed");
}

// Friend functions
void saveSystemData(InternshipSystem& system) 
{
    system.saveStudentsToFile();
    system.saveJobsToFile();
    system.saveApplicationsToFile();
    system.saveStaffToFile();
    system.saveAdminsToFile();
}

void loadSystemData(InternshipSystem& system) 
{
    system.loadStudentsFromFile();
    system.loadJobsFromFile();
    system.loadApplicationsFromFile();
    system.loadStaffFromFile();
    system.loadAdminsFromFile();
}

// Main function
int main() 
{
    InternshipSystem system;
    loadSystemData(system);
    system.mainMenu();
    saveSystemData(system);
    return 0;
}
