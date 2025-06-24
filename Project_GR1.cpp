// Fix the include statements - Remove STL headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <limits>
#include <ctime>
#include <cctype>
#include <string>
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
class InternshipSystem; // Add this forward declaration

// Exception classes for error handling
class FileException : public exception 
{
private:
    string message;

public:
    FileException(const string& msg) : message(msg) {}
    virtual ~FileException() throw() {}
    virtual const char* what() const throw() 
	{
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
    virtual const char* what() const throw() 
	{
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
    virtual const char* what() const throw() 
	{
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
    virtual const char* what() const throw() 
	{
        return message.c_str();
    }
};


// Log entry structure
struct LogEntry 
{
    string timestamp;
    string eventType;
    string details;


    LogEntry(string type, string desc) 
	{
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp = timestamp.substr(0, timestamp.length()-1); // Remove newline
        eventType = type;
        details = desc;
    }

    

    string toString() const 
	{
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


    void manageStudents(InternshipSystem* system);
    void manageInternships(InternshipSystem* system);
    void viewAllApplications();
    void generateReports();
    void manageStaff(InternshipSystem* system);


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

    void processApplications(InternshipSystem* system); // Updated declaration
    void generateDepartmentReport(InternshipSystem* system); // Updated declaration
    void viewDepartmentInternships(InternshipSystem* system); // New declaration

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
    // Fix constructor to match member variables
    InternshipJob(string id, string t, string c, string d, string req = "") 
        : jobID(id), title(t), company(c), deadline(d), requirements(req), isActive(true) {}
    
    ~InternshipJob() {}

    // Add missing getter for requirements
    string getRequirements() const { return requirements; }
    bool getIsActive() const { return isActive; }
    
    // Add missing setter
    void setRequirements(string req) { requirements = req; }
    void setIsActive(bool active) { isActive = active; }

    string getJobID() const { return jobID; }
    string getTitle() const { return title; }
    string getCompany() const { return company; }
    string getDeadline() const { return deadline; }

    void displayInfo() {
        cout << "\n=== Job Information ===" << endl;
        cout << "Job ID: " << jobID << endl;
        cout << "Title: " << title << endl;
        cout << "Company: " << company << endl;
        cout << "Deadline: " << deadline << endl;
        cout << "Requirements: " << requirements << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
    }

    void updateProfile() {
        cout << "Updating job details for ID: " << jobID << endl;
        string newTitle, newCompany, newDeadline, newRequirements;
        cout << "Enter new Title: ";
        getline(cin, newTitle);
        cout << "Enter new Company: ";
        getline(cin, newCompany);
        cout << "Enter new Deadline: ";
        getline(cin, newDeadline);
        cout << "Enter new Requirements: ";
        getline(cin, newRequirements);
        
        if (!newTitle.empty()) title = newTitle;
        if (!newCompany.empty()) company = newCompany;
        if (!newDeadline.empty()) deadline = newDeadline;
        if (!newRequirements.empty()) requirements = newRequirements;
        cout << "Job updated.\n";
    }

    void display() {
        cout << "Job ID: " << jobID << " | Title: " << title 
             << " | Company: " << company << " | Deadline: " << deadline << endl;
    }

    friend void saveJobToFile(InternshipJob& job, ostream& file);
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
    InternshipJob* jobs[50];
    int jobCount;
    Staff* currentStaff;
    Admin* currentAdmin;  // Add this member variable

    // Replace with raw arrays
    Student* students[MAX_STUDENTS];
    Admin* admins[MAX_STUDENTS];
    Staff* staffMembers[MAX_STAFF];

    int studentCount;
    int adminCount;
    int staffCount;

    ApplicationList* applications;
    StudentHashTable* studentHash;

public:
    InternshipSystem(); // Single constructor declaration
    ~InternshipSystem(); // Single destructor declaration
    void mainMenu();
    void staffMenu(Staff* staff);
    int getJobCount() const { return jobCount; } // Getter
    InternshipJob* getJob(int index) const { return jobs[index]; } // Adjusted getter for array access
    Staff* getStaffMember(int index) const { return staffMembers[index]; }
    // ... other methods ...

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
    void studentMenu();
    void adminMenu();

    // Sorting algorithms
    void selectionSortStudentsByCGPA();
    void selectionSortStudentsByID();
    void bubbleSortJobsByDeadline();
    void mergeSortJobsByCompany(int low, int high);
    void mergeJobsByCompany(int low, int mid, int high);

    // Searching algorithms
    int binarySearchStudentByID(string targetID);
    Student* hashSearchStudent(string studentID);
    int linearSearchJobByTitle(string title);
    int fibonacciSearchStudentByID(string targetID);
    int interpolationSearchStudentByCGPA(float targetCGPA);

    // System management
    void registerStudent();
    void registerAdmin();
    void registerStaff();
    void addInternshipJob();
    void displayAllStudents();
    void displayAllJobs();
    void displayAllStaff();
    void displayStatistics();

    // Validation functions
    bool validateStudentID(string studentID);
    bool validateEmail(string email);
    bool validateCGPA(float cgpa);
    bool validatePassword(string password);

    // Backup and restore functions
    void backupSystemData();
    void restoreSystemData(string backupTimestamp);
    string getCurrentTimestamp();

    // Reporting functions
    void generateDetailedReport();
    void generateStudentPerformanceReport();

    // Search and filter functions
    void searchStudentsByDiploma(string diploma);
    void searchStudentsByCGPARange(float minCGPA, float maxCGPA);
    void searchJobsByCompany(string company);

    // System optimization and maintenance
    void optimizeHashTable();
    void defragmentData();
    int calculateSystemHealth();
    void runSystemDiagnostics();

    // Additional sorting functions
    void insertionSortStudentsByName();
    void bubbleSortJobsByTitle();

    friend class Admin;
    friend class Staff;
    friend void saveStudentToFile(Student& student, ostream& file);
    friend void saveAdminToFile(Admin& admin, ostream& file);
    friend void saveStaffToFile(Staff& staff, ostream& file);
    friend void saveJobToFile(InternshipJob& job, ostream& file);
    friend void saveSystemData(InternshipSystem& system);
    friend void loadSystemData(InternshipSystem& system);

    void initializeSampleData(); // Moved to public

    // Utility functions
    string trim(const string& str);
    long long stringToLongLong(const string& str);
    float stringToFloat(const string& str);
    string encryptPassword(const string& password);
    string decryptPassword(const string& encrypted);
    void logEvent(const string& eventType, const string& details);
};

// Static member initialization
int Student::totalStudents = 0;
int Admin::totalAdmins = 0;
int Staff::totalStaff = 0;


// Implementation of ApplicationList methods
void ApplicationList::insert(string studentID, string jobID, string jobTitle, string company, string status) 
{
    ApplicationNode* newNode = new ApplicationNode(studentID, jobID, jobTitle, company, status);
    if (head == NULL) 
	{
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
    while (current != NULL) 
	{
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
    while (head != NULL) 
	{
        ApplicationNode* temp = head;
        head = head->next;
        delete temp;
    }
    count = 0;
}



bool ApplicationList::search(string studentID, string jobID) 
{
    ApplicationNode* current = head;
    while (current != NULL) 
	{
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
        while (getline(file, line)) 
		{
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
    //email
    cout << "Current Email: " << email << endl;
    cout << "Enter new email (or press Enter to keep current): ";
    cin.ignore();
    string newEmail;
    getline(cin, newEmail);
    if (!newEmail.empty()) {
        email = newEmail;
    }
    
    // CGPA
    cout << "Current CGPA: " << cgpa << endl;
    cout << "Enter new CGPA (or -1 to keep current): ";
    float newCGPA;
    cin >> newCGPA;
    cin.ignore();
    if (newCGPA >= 0.0 && newCGPA <= 4.0) {
        cgpa = newCGPA;
    }

    // Skills
    cout << "Current Skills: " << skills << endl;
    cout << "Enter new skills (or press Enter to keep current): ";
    string newSkills;
    getline(cin, newSkills);
    if (!newSkills.empty()) {
        skills = newSkills;
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

        ifstream file("login_admin.txt"); // 
        if (!file.is_open()) 
        {
            throw FileException("Cannot open login_admin.txt"); // 
        }

        string line;
        while (getline(file, line)) 
        {
            stringstream ss(line);
            string storedID, storedName, storedEmail, storedPassword;
            
            // Parse using pipe delimiter
            getline(ss, storedID, '|');
            getline(ss, storedName, '|');
            getline(ss, storedEmail, '|');
            getline(ss, storedPassword, '|');
            
            if (inputID == storedID && inputPassword == storedPassword) 
            {
                file.close();
                id = inputID;
                name = storedName;
                email = storedEmail;
                return true;
            }
        }
        file.close();
        throw LoginException("Invalid admin credentials");

    } catch (const exception& e) {
        cout << "Login Error: " << e.what() << endl;
        return false;
    }

}


void Admin::manageStudents(InternshipSystem* system)
{
    int choice;
    cout << "\n=== Student Management ===" << endl;
    cout << "1. View All Students" << endl;
    cout << "2. Add New Student" << endl;
    cout << "3. Edit Student" << endl;
    cout << "4. Delete Student" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    
    // Use system's existing student array instead

    if (choice == 1) {
        cout << "\n=== Student List ===\n";
        for (int i = 0; i < system->studentCount; i++) {
            system->students[i]->displayInfo();
            cout << "----------------------\n";
        }
    }
    else if (choice == 2) {
        system->registerStudent(); // Use system's method
    }
    else if (choice == 3) {
        string editID;
        cout << "Enter Student ID to edit: ";
        getline(cin, editID);
        bool found = false;
        for (int i = 0; i < system->studentCount; i++) {
            if (system->students[i]->getID() == editID) {
                cout << "Editing student: " << system->students[i]->getName() << endl;
                system->students[i]->updateProfile();
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Student ID not found.\n";
        }
    } 
    else if (choice == 4) {
        string deleteID;
        cout << "Enter Student ID to delete: ";
        getline(cin, deleteID);
        bool found = false;
        for (int i = 0; i < system->studentCount; i++) {
            if (system->students[i]->getID() == deleteID) {
                delete system->students[i];
                // Shift remaining elements
                for (int j = i; j < system->studentCount - 1; j++) {
                    system->students[j] = system->students[j + 1];
                }
                system->studentCount--;
                found = true;
                cout << "Student deleted.\n";
                break;
            }
        }
        if (!found) {
            cout << "Student ID not found.\n";
        }
    } else {
        cout << "Invalid choice.\n";
    }
}


void Admin::manageInternships(InternshipSystem* system)
{
    int choice;
    cout << "\n=== Internship Management ===" << endl;
    cout << "1. View All Jobs" << endl;
    cout << "2. Add New Job" << endl;
    cout << "3. Edit Job" << endl;
    cout << "4. Delete Job" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // vector<InternshipJob*> jobList;
    // Use system's existing job array

    if (choice == 1) {
        cout << "\n=== Job List ===\n";
        for (int i = 0; i < system->jobCount; i++) {
            if (system->jobs[i]) system->jobs[i]->displayInfo();
            cout << "----------------------\n";
        }
    } else if (choice == 2) {
        system->addInternshipJob(); // Use system's method
    } else if (choice == 3) {
        string editID;
        cout << "Enter Job ID to edit: ";
        getline(cin, editID);
        for (int i = 0; i < system->jobCount; i++) {
            if (system->jobs[i] && system->jobs[i]->getJobID() == editID) {
                cout << "Editing job: " << system->jobs[i]->getTitle() << endl;
                system->jobs[i]->updateProfile();
                break;
            }
        }
    } else if (choice == 4) {
        string deleteID;
        cout << "Enter Job ID to delete: ";
        getline(cin, deleteID);
        for (int i = 0; i < system->jobCount; i++) {
            if (system->jobs[i] && system->jobs[i]->getJobID() == deleteID) {
                delete system->jobs[i];
                for (int j = i; j < system->jobCount - 1; j++) {
                    system->jobs[j] = system->jobs[j + 1];
                }
                system->jobCount--;
                cout << "Job deleted.\n";
                break;
            }
        }
    } else {
        cout << "Invalid choice.\n";
    }
}

void Admin::manageStaff(InternshipSystem* system)
{
    int choice;
    cout << "\n=== Staff Management ===" << endl;
    cout << "1. View All Staff" << endl;
    cout << "2. Add New Staff" << endl;
    cout << "3. Edit Staff" << endl;
    cout << "4. Delete Staff" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // vector<Staff*> staffList;
    // Use system's existing staff array

    if (choice == 1) {
        cout << "\n=== Staff List ===\n";
        for (int i = 0; i < system->staffCount; i++) {
            if (system->staffMembers[i]) system->staffMembers[i]->displayInfo();
            cout << "----------------------\n";
        }
    } else if (choice == 2) {
        system->registerStaff(); // Use system's method
    } else if (choice == 3) {
        string editID;
        cout << "Enter Staff ID to edit: ";
        getline(cin, editID);
        for (int i = 0; i < system->staffCount; i++) {
            if (system->staffMembers[i] && system->staffMembers[i]->getID() == editID) {
                cout << "Editing staff: " << system->staffMembers[i]->getName() << endl;
                // Add staff update method here
                break;
            }
        }
    } else if (choice == 4) {
        string deleteID;
        cout << "Enter Staff ID to delete: ";
        getline(cin, deleteID);
        for (int i = 0; i < system->staffCount; i++) {
            if (system->staffMembers[i] && system->staffMembers[i]->getID() == deleteID) {
                delete system->staffMembers[i];
                for (int j = i; j < system->staffCount - 1; j++) {
                    system->staffMembers[j] = system->staffMembers[j + 1];
                }
                system->staffCount--;
                cout << "Staff deleted.\n";
                break;
            }
        }
    } else {
        cout << "Invalid choice.\n";
    }
}


void Admin::viewAllApplications() 
{
    try {
        ifstream file("applications.txt");
        if (!file.is_open()) 
		{
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
    // Add more stats if integrated with InternshipSystem
    cout << "Total Jobs: " << 0 << endl; // Placeholder, integrate with system->getJobCount()
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
    } catch (const exception& e) 
	{
        cout << "Login Error: " << e.what() << endl;
        return false;
    }

}


void Staff::processApplications(InternshipSystem* system)
{
    try {
        ifstream file("applications.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open applications.txt");
        }

        cout << "\n=== Applications to Process ===\n";
        cout << left << setw(12) << "Student ID"
             << setw(8) << "Job ID"
             << setw(25) << "Job Title"
             << setw(20) << "Company"
             << setw(12) << "Status" << endl;
        cout << string(77, '-') << endl;

        //vector<string> linesToUpdate;
        // Use raw array instead
        string linesToUpdate[200]; // Max applications
        int updateCount = 0;
        
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string studentID, jobID, jobTitle, company, status;
            getline(ss, studentID, '|');
            getline(ss, jobID, '|');
            getline(ss, jobTitle, '|');
            getline(ss, company, '|');
            getline(ss, status, '|');

            if (company.find(department) != string::npos && status == "Pending") {
                cout << left << setw(12) << studentID
                     << setw(8) << jobID
                     << setw(25) << jobTitle
                     << setw(20) << company
                     << setw(12) << status << endl;

                int choice;
                cout << "Set status for " << jobID << " (1: Approved, 2: Rejected, 3: Skip): ";
                cin >> choice;
                if (choice == 1) status = "Approved";
                else if (choice == 2) status = "Rejected";
                if (choice == 1 || choice == 2) {
                    // linesToUpdate.push_back(...)
                    // se array indexing
                    linesToUpdate[updateCount++] = studentID + "|" + jobID + "|" + jobTitle + "|" + company + "|" + status;
                }
            }
        }
        file.close();

        if (updateCount > 0) {
            // Update file logic using array instead of vector
            ofstream outFile("applications.txt");
            if (!outFile.is_open()) {
                throw FileException("Cannot open applications.txt for writing");
            }
            
            for (int i = 0; i < updateCount; i++) {
                outFile << linesToUpdate[i] << endl;
            }
            outFile.close();
            cout << "Application statuses updated!\n";
        } else {
            cout << "No pending applications to process.\n";
        }
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Staff::generateDepartmentReport(InternshipSystem* system)
{
    try {
        ofstream report(("department_report_" + department + "_" + system->getCurrentTimestamp() + ".txt").c_str());
        if (!report.is_open()) {
            throw FileException("Cannot create department report file");
        }

        report << "=== " << department << " Department Report ===\n";
        report << "Generated on: " << system->getCurrentTimestamp() << "\n";
        report << "Total Staff: " << Staff::getTotalStaff() << "\n";

        int deptApps = 0;
        ifstream appFile("applications.txt");
        if (appFile.is_open()) {
            string line;
            while (getline(appFile, line)) {
                stringstream ss(line);
                string company;
                getline(ss, company, '|'); // Adjusted to skip fields correctly
                getline(ss, company, '|'); // Move to company field
                getline(ss, company, '|'); // Get company
                if (company.find(department) != string::npos) deptApps++;
            }
            appFile.close();
        }
        report << "Total Applications: " << deptApps << "\n";
        report.close();
        cout << "Department report generated: department_report_" << department << "_" << system->getCurrentTimestamp() << ".txt\n";
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Staff::viewDepartmentInternships(InternshipSystem* system)
{
    cout << "\n=== Internships for " << department << " Department ===\n";
    cout << left << setw(8) << "Job ID"
         << setw(30) << "Title"
         << setw(20) << "Company"
         << setw(12) << "Deadline" << endl;
    cout << string(70, '-') << endl;

    int count = 0;
    for (int i = 0; i < system->getJobCount(); i++) {
        InternshipJob* job = system->getJob(i);
        if (job && job->getCompany().find(department) != string::npos) {
            cout << left << setw(8) << job->getJobID()
                 << setw(30) << job->getTitle()
                 << setw(20) << job->getCompany()
                 << setw(12) << job->getDeadline() << endl;
            count++;
        }
    }
    if (count == 0) {
        cout << "No internships found for " << department << " department.\n";
    } else {
        cout << "\nTotal internships found: " << count << endl;
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

    // Initialize counters

    studentCount = 0;

    adminCount = 0;

    staffCount = 0;

    jobCount = 0;

    

    // Initialize arrays to NULL

    for (int i = 0; i < MAX_STUDENTS; i++) {

        students[i] = NULL;

        admins[i] = NULL;

    }

    for (int i = 0; i < MAX_STAFF; i++) {

        staffMembers[i] = NULL;

    }

    for (int i = 0; i < MAX_JOBS; i++) {

        jobs[i] = NULL;

    }

    

    // Initialize pointers

    currentStaff = NULL;

    currentAdmin = NULL;  // Initialize this

    

    applications = new ApplicationList();

    studentHash = new StudentHashTable();

    

    // Initialize with some sample data

    initializeSampleData();

}



InternshipSystem::~InternshipSystem() 

{

    // Clean up dynamic memory

    for (int i = 0; i < studentCount; i++) {

        delete students[i];

    }

    for (int i = 0; i < adminCount; i++) {

        delete admins[i];

    }

    for (int i = 0; i < staffCount; i++) {

        delete staffMembers[i];

    }

    for (int i = 0; i < jobCount; i++) {

        delete jobs[i];

    }

    delete applications;

    delete studentHash;

}



// Fix sample job creation in initializeSampleData()
void InternshipSystem::initializeSampleData() 
{
    // Sample students - using array indexing instead of push_back
    students[studentCount++] = new Student("1231203277", "Aisyah Binti Rahman", "aisyah.rahman@mmu.edu.my", 3.70, "IT", "Java, HTML, Communication");
    students[studentCount++] = new Student("1231203278", "Kevin Lim", "kevin.lim@mmu.edu.my", 2.85, "Business", "Marketing, Sales, Data Analysis");
    students[studentCount++] = new Student("1231203279", "Nurul Izzati", "nurul.izzati@mmu.edu.my", 3.80, "IT", "Graphic Design, Adobe Illustrator, HTML");
    students[studentCount++] = new Student("1231203280", "Arvind Raj", "arvind.raj@mmu.edu.my", 2.50, "Accounting", "Financial Reporting, Excel, Auditing");
    students[studentCount++] = new Student("1231203281", "Sarah Wong", "sarah.wong@mmu.edu.my", 3.40, "Business", "Public Speaking, SEO, Business Management");
    students[studentCount++] = new Student("1231203282", "Faizal Mohd", "faizal.mohd@mmu.edu.my", 3.05, "IT", "C++, MySQL, Network Security");
    students[studentCount++] = new Student("1231203283", "Michelle Tan", "michelle.tan@mmu.edu.my", 3.75, "IT", "UI/UX Design, Figma, HTML");
    students[studentCount++] = new Student("1231203284", "Daniel Lee", "daniel.lee@mmu.edu.my", 2.90, "Accounting", "Bookkeeping, Tax Preparation, Excel");
    students[studentCount++] = new Student("1231203285", "Hema Letchumy", "hema.letchumy@mmu.edu.my", 3.20, "Business", "Project Management, Python, Agile");
    students[studentCount++] = new Student("1231203286", "Amirul Hakim", "amirul.hakim@mmu.edu.my", 2.10, "IT", "Linux, Troubleshooting, Python");
    students[studentCount++] = new Student("1231203287", "Lim Kai Jie", "lim.kj@mmu.edu.my", 3.00, "Accounting", "Payroll Management, QuickBooks, Financial Analysis");
    students[studentCount++] = new Student("1231203288", "Rani Devi", "rani.devi@mmu.edu.my", 3.50, "Business", "Excel, Business Analysis, Communication");
    students[studentCount++] = new Student("1231203289", "John Goh", "john.goh@mmu.edu.my", 2.75, "IT", "Ruby on Rails, Web Development, GitHub");
    students[studentCount++] = new Student("1231203290", "Siti Noraini", "siti.noraini@mmu.edu.my", 3.60, "Business", "Content Writing, Canva, Social Media Marketing");
    students[studentCount++] = new Student("1231203291", "Thinesh Kumar", "thinesh.kumar@mmu.edu.my", 2.40, "IT", "Android Development, Java, SQLite");
    students[studentCount++] = new Student("1231203292", "Leong Wei Ming", "leong.wei.ming@mmu.edu.my", 3.35, "Accounting", "C#, .NET, SQL");
    students[studentCount++] = new Student("1231203293", "Maya Shanti", "maya.shanti@mmu.edu.my", 3.55, "Business", "Marketing, Social Media, SEO");
    students[studentCount++] = new Student("1231203190", "edwin ceo", "mone@gmail.com", 3.98, "IT", "Java");
    students[studentCount++] = new Student("1231201130", "Riashini a/p Manoj Kumar", "riawee25@gmail.com", 3.7, "IT", "Java, HTML, PHP, C++, Python");
    
    // ? Add sample admins to the system
    admins[adminCount++] = new Admin("admin1", "Admin One", "admin1@mmu.edu.my", "password123");
    admins[adminCount++] = new Admin("admin2", "Admin Two", "admin2@mmu.edu.my", "admin456");
    admins[adminCount++] = new Admin("test", "Test Admin", "test@mmu.edu.my", "123456");
    
    // Sample staff
    staffMembers[staffCount++] = new Staff("STF1001", "Dr. Lim Wei Chen", "lim.wc@mmu.edu.my", "Career Services", "Manager", "pass123");
    staffMembers[staffCount++] = new Staff("STF1002", "Ms. Nor Azlina", "nor.azlina@mmu.edu.my", "IT Department", "Coordinator", "pwd456");
    staffMembers[staffCount++] = new Staff("STF1003", "Mr. Rajesh Kumar", "rajesh.k@mmu.edu.my", "Business Faculty", "Advisor", "secure789");
    
    // Sample jobs - Add requirements parameter
    jobs[jobCount++] = new InternshipJob("JOB1001", "Software Developer Intern", "Tech Solutions Sdn Bhd", "2023-12-15", "C++, Python, Problem Solving");
    jobs[jobCount++] = new InternshipJob("JOB1002", "Marketing Intern", "Global Marketing Group", "2023-11-30", "Communication, Creativity, Social Media");
    jobs[jobCount++] = new InternshipJob("JOB1003", "Finance Intern", "Capital Investments Berhad", "2024-01-10", "Accounting, Excel, Analytical Skills");
    
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

        // Clear existing data
        for (int i = 0; i < studentCount; i++) {
            delete students[i];
            students[i] = NULL;
        }
        studentCount = 0;
        
        string line;
        while (getline(file, line) && studentCount < MAX_STUDENTS) {

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
            Student* student = new Student(id, name, email, cgpa, diploma, skills);
            students[studentCount++] = student;
            studentHash->insert(student);
        }
        file.close();
        cout << "Loaded " << studentCount << " students from file.\n";
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

        for (int i = 0; i < studentCount; i++) {
            saveStudentToFile(*students[i], file);
        }
        file.close();
        cout << "Saved " << studentCount << " students to file.\n";
        logEvent("SYSTEM", "Students saved to file");
        
    } catch (const FileException& e) {  // ? Fix: Add proper catch block
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

        

        //  staffMembers.clear();  // This is vector method!
        //  Clear array manually instead
        for (int i = 0; i < staffCount; i++) {
            delete staffMembers[i];
            staffMembers[i] = NULL;
        }
        staffCount = 0;

        string line;

        while (getline(file, line) && staffCount < MAX_STAFF) {

            if (line.empty()) continue;



            stringstream ss(line);

            string id, name, email, department, position, password;

            getline(ss, id, '|');

            getline(ss, name, '|');

            getline(ss, email, '|');

            getline(ss, department, '|');

            getline(ss, position, '|');

            getline(ss, password, '|');



            //  staffMembers.push_back(staff);  // Vector method!
            //  Use array indexing instead
            staffMembers[staffCount++] = new Staff(id, name, email, department, position, password);

        }

        file.close();

        cout << "Loaded " << staffCount << " staff members from file.\n";

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



        //  for (size_t i = 0; i < staffMembers.size(); i++) 
        //  Use staffCount instead
        for (int i = 0; i < staffCount; i++) {

            ::saveStaffToFile(*staffMembers[i], file);

        }

        file.close();

        cout << "Saved " << staffCount << " staff members to file.\n";

        logEvent("SYSTEM", "Staff saved to file");

    } catch (const FileException& e) {

        cout << "Error: " << e.what() << endl;

        logEvent("ERROR", e.what());

    }

}



void InternshipSystem::loadAdminsFromFile() 

{

    try {
        ifstream file("login_admin.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open login_admin.txt for reading");
        }

        // Clear array manually
        for (int i = 0; i < adminCount; i++) {
            delete admins[i];
            admins[i] = NULL;
        }
        adminCount = 0;

        string line;
        while (getline(file, line) && adminCount < MAX_STUDENTS) {
            if (line.empty()) continue;

            stringstream ss(line);
            string id, name, email, password;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, password, '|');

            admins[adminCount++] = new Admin(id, name, email, password);
        }
        file.close();
        cout << "Loaded " << adminCount << " admins from file.\n";
        logEvent("SYSTEM", "Admins loaded from file");
    } catch (const FileException& e) {
        cout << "Error: " << e.what() << endl;
        logEvent("ERROR", e.what());
    }
}



void InternshipSystem::saveAdminsToFile() 

{

    try {
        ofstream file("login_admin.txt");
        if (!file.is_open()) {
            throw FileException("Cannot open login_admin.txt for writing");
        }

        for (int i = 0; i < adminCount; i++) {
            saveAdminToFile(*admins[i], file);
        }
        file.close();
        cout << "Saved " << adminCount << " admins to file.\n";
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

        //  jobs.clear();
        //  Clear array manually
        for (int i = 0; i < jobCount; i++) {
            delete jobs[i];
            jobs[i] = NULL;
        }
        jobCount = 0;

        string line;

        while (getline(file, line) && jobCount < MAX_JOBS) {

            if (line.empty()) continue;

            stringstream ss(line);

            string jobID, title, company, deadline, requirements;

            getline(ss, jobID, '|');

            getline(ss, title, '|');

            getline(ss, company, '|');

            getline(ss, deadline, '|');

            getline(ss, requirements, '|');



            // if (jobs.size() < MAX_JOBS) { jobs.push_back(job); }
            //  Use array indexing instead
            jobs[jobCount++] = new InternshipJob(jobID, title, company, deadline, requirements);
        }
        file.close();
        //  cout << "Loaded " << jobs.size() << " jobs from file.\n";
        //  Fix to use jobCount
        cout << "Loaded " << jobCount << " jobs from file.\n";
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

        // for (size_t i = 0; i < jobs.size(); i++) 
        //  Use jobCount instead
        for (int i = 0; i < jobCount; i++) {
            saveJobToFile(*jobs[i], file);
        }
        file.close();
        // cout << "Saved " << jobs.size() << " jobs to file.\n";
        // Fix to use jobCount
        cout << "Saved " << jobCount << " jobs to file.\n";
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
			if (staff->login()) {
			currentStaff = staff;
			staffMenu(currentStaff);
			} else {
			 delete staff;
			 }
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
    Student tempStudent;

    do {
        cout << "\n=== Student Menu ===\n";
        cout << "1. View Internships\n";
        cout << "2. Apply for Internship\n";
        cout << "3. View My Applications\n";
        cout << "4. Generate Summary Report\n";
        cout << "5. Update Profile\n";
        cout << "6. Search Jobs by Company\n";
        cout << "7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                tempStudent.viewInternships();
                break;
            case 2:
                tempStudent.applyForInternship();
                break;
            case 3:
                tempStudent.viewMyApplications();
                break;
            case 4:
                tempStudent.generateSummaryReport();
                break;
            case 5:
                tempStudent.updateProfile();
                break;
            case 6:
                {
                    string company;
                    cout << "Enter company name to search: ";
                    getline(cin, company);
                    searchJobsByCompany(company);
                }
                break;
            case 7:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Fix adminMenu function
void InternshipSystem::adminMenu() 
{
    Admin* currentAdmin = new Admin(); // Create local admin object
    
    int choice;
    do {
        cout << "\n=== Admin Menu ===\n";
        cout << "1. Manage Students\n";
        cout << "2. Manage Internships\n";
        cout << "3. View All Applications\n";
        cout << "4. Generate Reports\n";
        cout << "5. Search Students by Diploma\n";
        cout << "6. Search Students by CGPA Range\n";
        cout << "7. Search Jobs by Company\n";
        cout << "8. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                currentAdmin->manageStudents(this);
                break;
            case 2:
                currentAdmin->manageInternships(this);
                break;
            case 3:
                currentAdmin->viewAllApplications();
                break;
            case 4:
                currentAdmin->generateReports();
                break;
            case 5:
                {
                    string diploma;
                    cout << "Enter diploma to search: ";
                    getline(cin, diploma);
                    searchStudentsByDiploma(diploma);
                }
                break;
            case 6:
                {
                    float minCGPA, maxCGPA;
                    cout << "Enter minimum CGPA: ";
                    cin >> minCGPA;
                    cout << "Enter maximum CGPA: ";
                    cin >> maxCGPA;
                    cin.ignore();
                    searchStudentsByCGPARange(minCGPA, maxCGPA);
                }
                break;
            case 7:
                {
                    string company;
                    cout << "Enter company name to search: ";
                    getline(cin, company);
                    searchJobsByCompany(company);
                }
                break;
            case 8:
                cout << "Logging out...\n";
                delete currentAdmin; // Clean up
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void InternshipSystem::staffMenu(Staff* staff)
{
    int choice;
    do {
        cout << "\n=== Staff Menu ===\n";
        cout << "1. Process Applications\n";
        cout << "2. Generate Department Report\n";
        cout << "3. View Department Internships\n";
        cout << "4. Search Students by CGPA Range\n";
        cout << "5. Search Jobs by Company\n";
        cout << "6. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                staff->processApplications(this);
                break;
            case 2:
                staff->generateDepartmentReport(this);
                break;
            case 3:
                staff->viewDepartmentInternships(this);
                break;
            case 4:
                {
                    float minCGPA, maxCGPA;
                    cout << "Enter minimum CGPA: ";
                    cin >> minCGPA;
                    cout << "Enter maximum CGPA: ";
                    cin >> maxCGPA;
                    cin.ignore();
                    searchStudentsByCGPARange(minCGPA, maxCGPA);
                }
                break;
            case 5:
                {
                    string company;
                    cout << "Enter company name: ";
                    getline(cin, company);
                    searchJobsByCompany(company);
                }
                break;
            case 6:
                cout << "Logging out...\n";
                currentStaff = NULL;
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}


// Fix sorting algorithms
void InternshipSystem::selectionSortStudentsByCGPA() 
{
    for (int i = 0; i < studentCount - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < studentCount; j++) {
            if (students[j]->getCGPA() > students[maxIndex]->getCGPA()) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            Student* temp = students[i];
            students[i] = students[maxIndex];  // ? Fix this line
            students[maxIndex] = temp;         // ? Fix this line
        }
    }
    cout << "Students sorted by CGPA.\n";
    logEvent("SYSTEM", "Students sorted by CGPA");
}

void InternshipSystem::selectionSortStudentsByID() 
{
    for (int i = 0; i < studentCount - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < studentCount; j++) {
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
    logEvent("SYSTEM", "Students sorted by ID");
}



void InternshipSystem::bubbleSortJobsByDeadline() 

{
    for (int i = 0; i < jobCount - 1; i++) {
        for (int j = 0; j < jobCount - i - 1; j++) {

            if (jobs[j]->getDeadline() > jobs[j + 1]->getDeadline()) {
                InternshipJob* temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }
    cout << "Jobs sorted by deadline using Bubble Sort.\n";
    logEvent("SYSTEM", "Jobs sorted by deadline");
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




    // Use raw arrays instead
    InternshipJob** left = new InternshipJob*[n1];
    InternshipJob** right = new InternshipJob*[n2];



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

    // Clean up memory
    delete[] left;
    delete[] right;
}

// Fix search algorithms
int InternshipSystem::binarySearchStudentByID(string targetID) 
{
    selectionSortStudentsByID(); // Ensure sorted array for binary search
    int left = 0, right = studentCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (students[mid]->getID() == targetID) return mid;
        if (students[mid]->getID() < targetID) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int InternshipSystem::linearSearchJobByTitle(string title) 
{

    for (int i = 0; i < jobCount; i++) {
        if (jobs[i]->getTitle().find(title) != string::npos) {
            return i;
        }
    }
    return -1;
}

int InternshipSystem::fibonacciSearchStudentByID(string targetID) 
{
    selectionSortStudentsByID(); // Ensure sorted
    
    int fibM2 = 0; // (m-2)'th Fibonacci number
    int fibM1 = 1; // (m-1)'th Fibonacci number
    int fibM = fibM2 + fibM1; // m'th Fibonacci number
    
    while (fibM < studentCount) {
        fibM2 = fibM1;
        fibM1 = fibM;
        fibM = fibM2 + fibM1;
    }
    
    int offset = -1;
    
    while (fibM > 1) {
        int i = min(offset + fibM2, studentCount - 1);
        
        if (students[i]->getID() < targetID) {
            fibM = fibM1;
            fibM1 = fibM2;
            fibM2 = fibM - fibM1;
            offset = i;
        }
        else if (students[i]->getID() > targetID) {
            fibM = fibM2;
            fibM1 = fibM1 - fibM2;
            fibM2 = fibM - fibM1;
        }
        else {
            cout << "Student found using Fibonacci Search!\n";
            return i;
        }
    }
    
    if (fibM1 && offset + 1 < studentCount && students[offset + 1]->getID() == targetID) {
        return offset + 1;
    }
    
    return -1;
}

int InternshipSystem::interpolationSearchStudentByCGPA(float targetCGPA) 
{
    selectionSortStudentsByCGPA(); // Ensure sorted by CGPA
    
    int low = 0;
    int high = studentCount - 1;
    
    while (low <= high && targetCGPA >= students[low]->getCGPA() && targetCGPA <= students[high]->getCGPA()) {
        if (low == high) {
            if (students[low]->getCGPA() == targetCGPA) return low;
            return -1;
        }
        
        int pos = low + (((double)(targetCGPA - students[low]->getCGPA()) / 
                         (students[high]->getCGPA() - students[low]->getCGPA())) * (high - low));
        
        if (students[pos]->getCGPA() == targetCGPA) {
            cout << "Student found using Interpolation Search!\n";
            return pos;
        }
        
        if (students[pos]->getCGPA() < targetCGPA) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    
    return -1;
}

// Advanced data validation functions
bool InternshipSystem::validateStudentID(string studentID) 
{
    if (studentID.length() != 10) {
        cout << "Error: Student ID must be exactly 10 digits.\n";
        return false;
    }
    
    // for (char c : studentID) {
    // Use traditional for loop
    for (size_t i = 0; i < studentID.length(); i++) {
        char c = studentID[i];
        if (!isdigit(c)) {
            cout << "Error: Student ID must contain only digits.\n";
            return false;
        }
    }
    
    // Check if ID already exists
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getID() == studentID) {
            cout << "Error: Student ID already exists.\n";
            return false;
        }
    }
    
    return true;
}

bool InternshipSystem::validateEmail(string email) 
{
    if (email.find('@') == string::npos) {
        cout << "Error: Invalid email format - missing @ symbol.\n";
        return false;
    }
    
    if (email.find('.') == string::npos) {
        cout << "Error: Invalid email format - missing domain extension.\n";
        return false;
    }
    
    if (email.length() < 5) {
        cout << "Error: Email too short.\n";
        return false;
    }
    
    // Check for duplicate emails
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getEmail() == email) {
            cout << "Error: Email already registered.\n";
            return false;
        }
    }
    
    return true;
}

bool InternshipSystem::validateCGPA(float cgpa) 
{
    if (cgpa < 0.0 || cgpa > 4.0) {
        cout << "Error: CGPA must be between 0.0 and 4.0.\n";
        return false;
    }
    
    return true;
}

bool InternshipSystem::validatePassword(string password) 
{
    if (password.length() < 6) {
        cout << "Error: Password must be at least 6 characters long.\n";
        return false;
    }
    
    bool hasUpper = false, hasLower = false, hasDigit = false;
    
    // for (char c : password) {
    // Use traditional for loop
    for (size_t i = 0; i < password.length(); i++) {
        char c = password[i];
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }
    
    if (!hasUpper || !hasLower || !hasDigit) {
        cout << "Error: Password must contain uppercase, lowercase, and digit.\n";
        return false;
    }
    
    return true;
}

// Extended file management functions
void InternshipSystem::backupSystemData() 
{
    try {
        // Create backup directory if not exists
        string backupDir = "backup_" + getCurrentTimestamp();
        
        // Backup students
        string studentBackup = backupDir + "_students.txt";
        ofstream studentFile(studentBackup.c_str());
        if (!studentFile.is_open()) {
            throw FileException("Cannot create student backup file");
        }
        
        for (int i = 0; i < studentCount; i++) {
            saveStudentToFile(*students[i], studentFile);
        }
        studentFile.close();
        
        // Backup jobs
        string jobBackup = backupDir + "_jobs.txt";
        ofstream jobFile(jobBackup.c_str());
        if (!jobFile.is_open()) {
            throw FileException("Cannot create job backup file");
        }
        
        for (int i = 0; i < jobCount; i++) {
            saveJobToFile(*jobs[i], jobFile);
        }
        jobFile.close();
        
        // Backup staff
        string staffBackup = backupDir + "_staff.txt";
        ofstream staffFile(staffBackup.c_str());
        if (!staffFile.is_open()) {
            throw FileException("Cannot create staff backup file");
        }
        
        // saveStaffToFile(*staffMembers[i], staffFile);
        // Use the global friend function instead
        for (int i = 0; i < staffCount; i++) {
            ::saveStaffToFile(*staffMembers[i], staffFile);
        }
        staffFile.close();
        
        // Backup applications
        string appBackup = backupDir + "_applications.txt";
        applications->saveToFile(appBackup);
        
        cout << "System backup completed successfully!\n";
        logEvent("SYSTEM", "Backup created: " + backupDir);
        
    } catch (const FileException& e) {
        cout << "Backup Error: " << e.what() << endl;
        logEvent("ERROR", "Backup failed: " + string(e.what()));
    }
}

void InternshipSystem::restoreSystemData(string backupTimestamp) 
{
    try {
        cout << "Restoring system data from backup: " << backupTimestamp << endl;
        
        // Restore students
        string studentRestore = "backup_" + backupTimestamp + "_students.txt";
        ifstream studentFile(studentRestore.c_str());
        if (!studentFile.is_open()) {
            throw FileException("Cannot open student backup file");
        }
        
        // Clear current data
        for (int i = 0; i < studentCount; i++) {
            delete students[i];
        }
        studentCount = 0;
        
        string line;
        while (getline(studentFile, line) && studentCount < MAX_STUDENTS) {
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
            students[studentCount++] = new Student(id, name, email, cgpa, diploma, skills);
        }
        studentFile.close();
        
        cout << "System restore completed successfully!\n";
        logEvent("SYSTEM", "System restored from: " + backupTimestamp);
        
    } catch (const FileException& e) {
        cout << "Restore Error: " << e.what() << endl;
        logEvent("ERROR", "Restore failed: " + string(e.what()));
    }
}

string InternshipSystem::getCurrentTimestamp() 
{
       time_t now = time(0);
    char* timeStr = ctime(&now);
    string timestamp = timeStr;
    
    // Remove newline and replace spaces/colons with underscores
    timestamp = timestamp.substr(0, timestamp.length() - 1);
    for (size_t i = 0; i < timestamp.length(); i++) {
        if (timestamp[i] == ' ' || timestamp[i] == ':') {
            timestamp[i] = '_';
        }
    }
    
    return timestamp;
}

// System analytics and reporting functions
void InternshipSystem::generateDetailedReport() 
{
    try {
        string reportFile = "detailed_report_" + getCurrentTimestamp() + ".txt";
        ofstream report(reportFile.c_str());
        if (!report.is_open()) {
            throw FileException("Cannot create report file");
        }
        
        report << "=== INTERNSHIP SYSTEM DETAILED REPORT ===" << endl;
        report << "Generated on: " << getCurrentTimestamp() << endl;
        report << string(50, '=') << endl;
        
        // Student statistics
        report << "\n--- STUDENT STATISTICS ---" << endl;
        report << "Total Students: " << studentCount << endl;
        
        float totalCGPA = 0.0;
        float highestCGPA = 0.0;
        float lowestCGPA = 4.0;
        
        for (int i = 0; i < studentCount; i++) {
            float cgpa = students[i]->getCGPA();
            totalCGPA += cgpa;
            if (cgpa > highestCGPA) highestCGPA = cgpa;
            if (cgpa < lowestCGPA) lowestCGPA = cgpa;
        }
        
        if (studentCount > 0) {
            report << "Average CGPA: " << fixed << setprecision(2) << (totalCGPA / studentCount) << endl;
            report << "Highest CGPA: " << highestCGPA << endl;
            report << "Lowest CGPA: " << lowestCGPA << endl;
        }
        
        // Diploma distribution
        int itCount = 0, businessCount = 0, accountingCount = 0;
        for (int i = 0; i < studentCount; i++) {
            string diploma = students[i]->getDiploma(); // ? Fix: Use students[i]->getDiploma()
            if (diploma == "IT") itCount++;
            else if (diploma == "Business") businessCount++;
            else if (diploma == "Accounting") accountingCount++;
        }
        
        report << "\n--- DIPLOMA DISTRIBUTION ---" << endl;
        if (studentCount > 0) {
            report << "IT Students: " << itCount << " (" << (itCount * 100.0 / studentCount) << "%)" << endl;
            report << "Business Students: " << businessCount << " (" << (businessCount * 100.0 / studentCount) << "%)" << endl;
            report << "Accounting Students: " << accountingCount << " (" << (accountingCount * 100.0 / studentCount) << "%)" << endl;
        }
        
        // Job statistics
        report << "\n--- JOB STATISTICS ---" << endl;
        report << "Total Jobs Available: " << jobCount << endl;
        report << "Total Applications: " << applications->getCount() << endl;
        
        report.close();
        cout << "Detailed report generated: " << reportFile << endl;
        logEvent("SYSTEM", "Detailed report generated");
        
    } catch (const FileException& e) {
        cout << "Report Error: " << e.what() << endl;
        logEvent("ERROR", "Report generation failed");
    }
}

void InternshipSystem::generateStudentPerformanceReport() 
{
    try {
        string reportFile = "student_performance_" + getCurrentTimestamp() + ".txt";
        ofstream report(reportFile.c_str());
        if (!report.is_open()) {
            throw FileException("Cannot create performance report file");
        }
        
        report << "=== STUDENT PERFORMANCE ANALYSIS ===" << endl;
        report << "Generated on: " << getCurrentTimestamp() << endl;
        report << string(50, '=') << endl;
        
        // Sort students by CGPA for analysis
        selectionSortStudentsByCGPA();
        
        report << "\n--- TOP 10 STUDENTS BY CGPA ---" << endl;
        int topCount = min(10, studentCount);
        for (int i = 0; i < topCount; i++) {
            report << (i + 1) << ". " << students[i]->getName() 
                   << " (ID: " << students[i]->getID() 
                   << ") - CGPA: " << fixed << setprecision(2) << students[i]->getCGPA() 
                   << " - " << students[i]->getDiploma() << endl;
        }
        
        report << "\n--- CGPA GRADE DISTRIBUTION ---" << endl;
        int gradeA = 0, gradeB = 0, gradeC = 0, gradeD = 0;
        
        for (int i = 0; i < studentCount; i++) {
            float cgpa = students[i]->getCGPA();
            if (cgpa >= 3.5) gradeA++;
            else if (cgpa >= 3.0) gradeB++;
            else if (cgpa >= 2.5) gradeC++;
            else gradeD++;
        }
        
        report << "Grade A (3.5-4.0): " << gradeA << " students (" << (gradeA * 100.0 / studentCount) << "%)" << endl;
        report << "Grade B (3.0-3.49): " << gradeB << " students (" << (gradeB * 100.0 / studentCount) << "%)" << endl;
        report << "Grade C (2.5-2.99): " << gradeC << " students (" << (gradeC * 100.0 / studentCount) << "%)" << endl;
        report << "Grade D (Below 2.5): " << gradeD << " students (" << (gradeD * 100.0 / studentCount) << "%)" << endl;
        
        report.close();
        cout << "Student performance report generated: " << reportFile << endl;
        logEvent("SYSTEM", "Performance report generated");
        
    } catch (const FileException& e) {
        cout << "Performance Report Error: " << e.what() << endl;
        logEvent("ERROR", "Performance report generation failed");
    }
}

// Advanced search and filter functions
void InternshipSystem::searchStudentsByDiploma(string diploma) 
{
    cout << "\n=== Students in " << diploma << " Program ===" << endl;
    cout << left << setw(12) << "Student ID" 
         << setw(25) << "Name" 
         << setw(6) << "CGPA" 
         << setw(30) << "Skills" << endl;
    cout << string(73, '-') << endl;
    
    int count = 0;
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getDiploma() == diploma) {
            cout << left << setw(12) << students[i]->getID()
                 << setw(25) << students[i]->getName()
                 << setw(6) << fixed << setprecision(2) << students[i]->getCGPA()
                 << setw(30) << students[i]->getSkills() << endl;
            count++;
        }
    }
    
    if (count == 0) {
        cout << "No students found in " << diploma << " program." << endl;
    } else {
        cout << "\nTotal students found: " << count << endl;
    }
    
    logEvent("SEARCH", "Students searched by diploma: " + diploma);
}

void InternshipSystem::searchStudentsByCGPARange(float minCGPA, float maxCGPA) 
{
    cout << "\n=== Students with CGPA between " << minCGPA << " and " << maxCGPA << " ===" << endl;
    cout << left << setw(12) << "Student ID" 
         << setw(25) << "Name" 
         << setw(6) << "CGPA" 
         << setw(15) << "Diploma" << endl;
    cout << string(58, '-') << endl;
    
    int count = 0;
    for (int i = 0; i < studentCount; i++) {
        float cgpa = students[i]->getCGPA();
        if (cgpa >= minCGPA && cgpa <= maxCGPA) {
            cout << left << setw(12) << students[i]->getID()
                 << setw(25) << students[i]->getName()
                 << setw(6) << fixed << setprecision(2) << cgpa
                 << setw(15) << students[i]->getDiploma() << endl;
            count++;
        }
    }
    
    if (count == 0) {
        cout << "No students found in the specified CGPA range." << endl;
    } else {
        cout << "\nTotal students found: " << count << endl;
    }
    
    logEvent("SEARCH", "Students searched by CGPA range");
}

void InternshipSystem::searchJobsByCompany(string company) 
{
    cout << "\n=== Jobs from " << company << " ===" << endl;
    cout << left << setw(8) << "Job ID" 
         << setw(30) << "Title" 
         << setw(12) << "Deadline" 
         << setw(20) << "Requirements" << endl;
    cout << string(70, '-') << endl;
    
    int count = 0;
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i]->getCompany().find(company) != string::npos) {
            cout << left << setw(8) << jobs[i]->getJobID()
                 << setw(30) << jobs[i]->getTitle()
                 << setw(12) << jobs[i]->getDeadline()
                 << setw(20) << jobs[i]->getRequirements() << endl;
            count++;
        }
    }
    
    if (count == 0) {
        cout << "No jobs found from " << company << endl;
    } else {
        cout << "\nTotal jobs found: " << count << endl;
    }
    
    logEvent("SEARCH", "Jobs searched by company: " + company);
}

// System maintenance and optimization functions
void InternshipSystem::optimizeHashTable() 
{
    cout << "Optimizing hash table..." << endl;
    
    // Save current students
    Student* tempStudents[MAX_STUDENTS];
    int tempCount = 0;
    
    for (int i = 0; i < studentCount; i++) {
        tempStudents[tempCount++] = students[i];
    }
    
    // Create new optimized hash table
    delete studentHash;
    int newSize = studentCount * 2; // Double the size for better distribution
    if (newSize < TABLE_SIZE) newSize = TABLE_SIZE;
    
    studentHash = new StudentHashTable(newSize);
    
    // Re-insert all students
    for (int i = 0; i < tempCount; i++) {
        studentHash->insert(tempStudents[i]);
    }
    
    cout << "Hash table optimized. New size: " << newSize << endl;
    cout << "Load factor: " << fixed << setprecision(2) << studentHash->getLoadFactor() << endl;
    logEvent("SYSTEM", "Hash table optimized");
}

void InternshipSystem::defragmentData() 
{
    cout << "Defragmenting data structures..." << endl;
    
    // Remove NULL pointers and compact arrays
    int newStudentCount = 0;
    for (int i = 0; i < studentCount; i++) {
        if (students[i] != NULL) {
            if (newStudentCount != i) {
                students[newStudentCount] = students[i];
                students[i] = NULL;
            }
            newStudentCount++;
        }
    }
    studentCount = newStudentCount;
    
    int newJobCount = 0;
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i] != NULL) {
            if (newJobCount != i) {
                jobs[newJobCount] = jobs[i];
                jobs[i] = NULL;
            }
            newJobCount++;
        }
    }
    jobCount = newJobCount;
    
    int newStaffCount = 0;
    for (int i = 0; i < staffCount; i++) {
        if (staffMembers[i] != NULL) {
            if (newStaffCount != i) {
                staffMembers[newStaffCount] = staffMembers[i];
                staffMembers[i] = NULL;
            }
            newStaffCount++;
        }
    }
    staffCount = newStaffCount;
    
    cout << "Data defragmentation completed." << endl;
    cout << "Active records - Students: " << studentCount 
         << ", Jobs: " << jobCount 
         << ", Staff: " << staffCount << endl;
    logEvent("SYSTEM", "Data defragmentation completed");
}

int InternshipSystem::calculateSystemHealth() 
{
    int healthScore = 100;
    
    // Check load factors
    double hashLoadFactor = studentHash->getLoadFactor();
    if (hashLoadFactor > 0.8) healthScore -= 10;
    if (hashLoadFactor > 0.9) healthScore -= 20;
    
    // Check data integrity
    int nullStudents = 0, nullJobs = 0, nullStaff = 0;
    
    for (int i = 0; i < studentCount; i++) {
        if (students[i] == NULL) nullStudents++;
    }
    
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i] == NULL) nullJobs++;
    }
    
    for (int i = 0; i < staffCount; i++) {
        if (staffMembers[i] == NULL) nullStaff++;
    }
    
    int totalNulls = nullStudents + nullJobs + nullStaff;
    healthScore -= (totalNulls * 2);
    
    // Check file consistency
    // This would involve checking if files exist and are readable
    
    if (healthScore < 0) healthScore = 0;
    if (healthScore > 100) healthScore = 100;
    
    return healthScore;
}

void InternshipSystem::runSystemDiagnostics() 
{
    cout << "\n=== SYSTEM DIAGNOSTICS ===" << endl;
    cout << "Running comprehensive system check..." << endl;
    
    int healthScore = calculateSystemHealth();
    
    cout << "\nSystem Health Score: " << healthScore << "/100" << endl;
    
    if (healthScore >= 90) {
        cout << "Status: EXCELLENT - System running optimally" << endl;
    } else if (healthScore >= 75) {
        cout << "Status: GOOD - System running well" << endl;
    } else if (healthScore >= 60) {
        cout << "Status: FAIR - Consider optimization" << endl;
    } else if (healthScore >= 40) {
        cout << "Status: POOR - Optimization recommended" << endl;
    } else {
        cout << "Status: CRITICAL - Immediate attention required" << endl;
    }
    
    cout << "\nDetailed Diagnostics:" << endl;
    cout << "- Student Count: " << studentCount << "/" << MAX_STUDENTS << endl;
    cout << "- Job Count: " << jobCount << "/" << MAX_JOBS << endl;
    cout << "- Staff Count: " << staffCount << "/" << MAX_STAFF << endl;
    cout << "- Application Count: " << applications->getCount() << endl;
    cout << "- Hash Table Load Factor: " << fixed << setprecision(2) << studentHash->getLoadFactor() << endl;
    
    // Memory usage estimation
    int memoryUsage = (studentCount * sizeof(Student*)) + 
                     (jobCount * sizeof(InternshipJob*)) + 
                     (staffCount * sizeof(Staff*));
    cout << "- Estimated Memory Usage: " << memoryUsage << " bytes" << endl;
    
    stringstream healthStr;
    healthStr << healthScore;
    logEvent("SYSTEM", "System diagnostics completed - Health: " + healthStr.str());
}

void InternshipSystem::insertionSortStudentsByName() 
{
    for (int i = 1; i < studentCount; i++) {
        Student* key = students[i];
        int j = i - 1;
        
        while (j >= 0 && students[j]->getName() > key->getName()) {
            students[j + 1] = students[j];
            j--;
        }
        students[j + 1] = key;
    }
    cout << "Students sorted by name using Insertion Sort.\n";
    logEvent("SYSTEM", "Students sorted by name");
}

void InternshipSystem::bubbleSortJobsByTitle() 
{
    for (int i = 0; i < jobCount - 1; i++) {
        for (int j = 0; j < jobCount - i - 1; j++) {
            if (jobs[j]->getTitle() > jobs[j + 1]->getTitle()) {
                InternshipJob* temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }
    cout << "Jobs sorted by title using Bubble Sort.\n";
    logEvent("SYSTEM", "Jobs sorted by title");
}

// Add these missing system management functions

Student* InternshipSystem::hashSearchStudent(string studentID) 
{
    return studentHash->search(studentID);
}

void InternshipSystem::registerStudent() 
{
    if (studentCount >= MAX_STUDENTS) {
        cout << "Maximum student capacity reached!" << endl;
        return;
    }
    
    string id, name, email, diploma, skills;
    float cgpa;
    
    cout << "Enter Student ID: ";
    cin >> id;
    if (!validateStudentID(id)) return;
    
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter Email: ";
    cin >> email;
    if (!validateEmail(email)) return;
    
    cout << "Enter CGPA: ";
    cin >> cgpa;
    if (!validateCGPA(cgpa)) return;
    
    cout << "Enter Diploma: ";
    cin.ignore();
    getline(cin, diploma);
    
    cout << "Enter Skills: ";
    getline(cin, skills);
    
    students[studentCount++] = new Student(id, name, email, cgpa, diploma, skills);
    studentHash->insert(students[studentCount-1]);
    cout << "Student registered successfully!" << endl;
    logEvent("SYSTEM", "New student registered: " + id);
}

void InternshipSystem::registerAdmin() 
{
    if (adminCount >= MAX_STUDENTS) {
        cout << "Maximum admin capacity reached!" << endl;
        return;
    }
    
    string id, name, email, password;
    
    cout << "Enter Admin ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Password: ";
    cin >> password;
    if (!validatePassword(password)) return;
    
    admins[adminCount++] = new Admin(id, name, email, password);
    cout << "Admin registered successfully!" << endl;
    logEvent("SYSTEM", "New admin registered: " + id);
}

void InternshipSystem::registerStaff() 
{
    if (staffCount >= MAX_STAFF) {
        cout << "Maximum staff capacity reached!" << endl;
        return;
    }
    
    string id, name, email, department, position, password;
    
    cout << "Enter Staff ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Department: ";
    getline(cin, department);
    cout << "Enter Position: ";
    getline(cin, position);
    cout << "Enter Password: ";
    cin >> password;
    if (!validatePassword(password)) return;
    
    staffMembers[staffCount++] = new Staff(id, name, email, department, position, password);
    cout << "Staff registered successfully!" << endl;
    logEvent("SYSTEM", "New staff registered: " + id);
}

void InternshipSystem::addInternshipJob() 
{
    if (jobCount >= MAX_JOBS) {
        cout << "Maximum job capacity reached!" << endl;
        return;
    }
    
    string jobID, title, company, deadline, requirements;
    
    cout << "Enter Job ID: ";
    cin >> jobID;
    cout << "Enter Job Title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter Company: ";
    getline(cin, company);
    cout << "Enter Deadline (YYYY-MM-DD): ";
    cin >> deadline;
    cout << "Enter Requirements: ";
    cin.ignore();
    getline(cin, requirements);
    
    jobs[jobCount++] = new InternshipJob(jobID, title, company, deadline, requirements);
    cout << "Job added successfully!" << endl;
    logEvent("SYSTEM", "New job added: " + jobID);
}

void InternshipSystem::displayAllStudents() 
{
    cout << "\n=== All Students ===" << endl;
    cout << left << setw(12) << "Student ID" 
         << setw(25) << "Name" 
         << setw(6) << "CGPA" 
         << setw(15) << "Diploma" << endl;
    cout << string(58, '-') << endl;
    
    for (int i = 0; i < studentCount; i++) {
        cout << left << setw(12) << students[i]->getID()
             << setw(25) << students[i]->getName()
             << setw(6) << fixed << setprecision(2) << students[i]->getCGPA()
             << setw(15) << students[i]->getDiploma() << endl;
    }
}

void InternshipSystem::displayAllJobs() 
{
    cout << "\n=== All Jobs ===" << endl;
    for (int i = 0; i < jobCount; i++) {
        jobs[i]->display();
    }
}

void InternshipSystem::displayAllStaff() 
{
    cout << "\n=== All Staff ===" << endl;
    for (int i = 0; i < staffCount; i++) {
        staffMembers[i]->displayInfo();
    }
}

void InternshipSystem::displayStatistics() 
{
    cout << "\n=== System Statistics ===" << endl;
    cout << "Total Students: " << studentCount << endl;
    cout << "Total Jobs: " << jobCount << endl;
    cout << "Total Staff: " << staffCount << endl;
    cout << "Total Applications: " << applications->getCount() << endl;
}
int main() 
{
    try {
        InternshipSystem system;
        system.mainMenu();
    } catch (const exception& e) {
        cout << "System Error: " << e.what() << endl;
    }
    
    return 0;
}
