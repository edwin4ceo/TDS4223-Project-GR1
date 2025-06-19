<<<<<<< HEAD
// Admin Module – Edwin Teo's Code (TDS4223 Internship System)
// Final version with login, menu, exception handling, OOP, sorting prep

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <stdexcept>
using namespace std;

const int TABLE_SIZE = 10;

// Struct to store login details
struct Login {
    string username;
    string password;
};

// Base class for users
class User {
protected:
    string role;
public:
    User(string r) : role(r) {
        cout << role << " module activated.\n";
    }
    virtual ~User() {
        cout << role << " module closing...\n";
    }
};

// Derived class for Admin
class Admin : public User {
public:
    Admin() : User("Admin") {}
};

// Struct to store student data
struct Student {
    string name, studentID, email, diploma, skills, appliedJob;
    float cgpa;
    Student* next;

    Student(string n, string id, string e, float g, string d, string s, string j)
        : name(n), studentID(id), email(e), cgpa(g), diploma(d), skills(s), appliedJob(j), next(NULL) {}

    ~Student() {}

    friend void printStudent(const Student*);
};

void printStudent(const Student* s) {
    cout << s->name << " | " << s->studentID << " | " << s->email << " | CGPA: " << s->cgpa << "\n";
}

Student* hashTable[TABLE_SIZE] = {NULL};
Student* sortedArray[100];
int arraySize = 0;

int hashFunction(string studentID) {
    long long id = atol(studentID.c_str());
    return id % TABLE_SIZE;
}

Student* createStudent(string name, string id, string email, float cgpa, string diploma, string skills, string job) {
    Student* newStudent = new Student(name, id, email, cgpa, diploma, skills, job);
    return newStudent;
}

void insertStudent(Student* student) {
    int index = hashFunction(student->studentID);
    student->next = hashTable[index];
    hashTable[index] = student;
}

void loadData() {
    try {
        ifstream file("raw data.txt");
        if (!file.is_open()) throw runtime_error("File not found.");

        string line;
        getline(file, line); getline(file, line);

        while(getline(file, line)) {
            stringstream ss(line);
            string name, id, email, cgpaStr, diploma, skills, job;
            getline(ss, name, '|'); getline(ss, id, '|'); getline(ss, email, '|');
            getline(ss, cgpaStr, '|'); getline(ss, diploma, '|'); getline(ss, skills, '|'); getline(ss, job);

            float cgpa = atof(cgpaStr.c_str());
            Student* student = createStudent(name, id, email, cgpa, diploma, skills, job);
            insertStudent(student);
        }
        file.close();
        cout << "Data loaded from raw data.txt successfully.\n";
    } catch (exception& e) {
        cerr << "Error loading file: " << e.what() << "\n";
    }
}

void displayData() {
    cout << "\n--- Student Records ---\n";
    for(int i = 0; i < TABLE_SIZE; i++) {
        Student* current = hashTable[i];
        while(current != NULL) {
            printStudent(current);
            current = current->next;
        }
    }
}

void addNewStudent() {
    try {
        string name, id, email, diploma, skills, job;
        float cgpa;

        cin.ignore();
        cout << "Enter Name: "; getline(cin, name);
        cout << "Enter Student ID: "; getline(cin, id);
        cout << "Enter Email: "; getline(cin, email);
        cout << "Enter CGPA: "; cin >> cgpa;
        if (cin.fail()) throw invalid_argument("Invalid CGPA input.");
        cin.ignore();
        cout << "Enter Diploma: "; getline(cin, diploma);
        cout << "Enter Skills: "; getline(cin, skills);
        cout << "Enter Applied Job: "; getline(cin, job);

        Student* student = createStudent(name, id, email, cgpa, diploma, skills, job);
        insertStudent(student);

        cout << "Student added successfully.\n";
    } catch (exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}

void convertToArray() {
    arraySize = 0;
    for(int i = 0; i < TABLE_SIZE; i++) {
        Student* current = hashTable[i];
        while(current != NULL && arraySize < 100) {
            sortedArray[arraySize++] = current;
            current = current->next;
        }
    }
    cout << "Data converted to array for sorting/searching.\n";
}

void displaySortedData() {
    cout << "\n--- Sorted Student Records ---\n";
    for(int i = 0; i < arraySize; i++) {
        printStudent(sortedArray[i]);
    }
}

void saveSortedData() {
    try {
        ofstream out("sorted_information.txt");
        if (!out.is_open()) throw runtime_error("Unable to open file.");
        for(int i = 0; i < arraySize; i++) {
            out << sortedArray[i]->name << " | " << sortedArray[i]->studentID << " | "
                << sortedArray[i]->email << " | CGPA: " << sortedArray[i]->cgpa << "\n";
        }
        out.close();
        cout << "Sorted data saved to sorted_information.txt.\n";
    } catch (exception& e) {
        cerr << "Error saving file: " << e.what() << "\n";
    }
}

void showSummary(int count) {
    cout << "Total records in system: " << count << "\n";
}

void showSummary(float cgpa) {
    cout << "Listing students with CGPA above " << cgpa << ":\n";
    for(int i = 0; i < arraySize; i++) {
        if(sortedArray[i]->cgpa >= cgpa) printStudent(sortedArray[i]);
    }
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
}

void clearScreen() {
    system("cls");
}

bool verifyAdminLogin() {
    string inputUser, inputPass;
    cout << "==== Admin Login ====" << endl;
    cout << "Username: "; cin >> inputUser;
    cout << "Password: "; cin >> inputPass;

    ifstream file("login_admin.txt");
    if (!file.is_open()) {
        cerr << "Unable to open login_admin.txt\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        size_t delimPos = line.find(',');
        string storedUser = line.substr(0, delimPos);
        string storedPass = line.substr(delimPos + 1);

        if (inputUser == storedUser && inputPass == storedPass) {
            cout << "Login successful.\n";
            file.close();
            return true;
        }
    }
    file.close();
    cout << "Invalid username or password.\n";
    return false;
}

void adminMenu() {
    int choice;
    Admin admin;

    do {
        clearScreen();
        cout << "==== Internship System - Admin Panel ====" << endl;
        cout << "1. Load Student Data\n2. Display All Records\n3. Add New Student\n4. Convert to Array\n5. Display Sorted Data\n6. Save Sorted Data\n7. Show Summary (Total Records)\n8. Show Summary (CGPA >= X)\n0. Exit\nSelect an option: ";
        cin >> choice;

        switch(choice) {
            case 1: clearScreen(); loadData(); pauseScreen(); break;
            case 2: clearScreen(); displayData(); pauseScreen(); break;
            case 3: clearScreen(); addNewStudent(); pauseScreen(); break;
            case 4: clearScreen(); convertToArray(); pauseScreen(); break;
            case 5: clearScreen(); displaySortedData(); pauseScreen(); break;
            case 6: clearScreen(); saveSortedData(); pauseScreen(); break;
            case 7: clearScreen(); showSummary(arraySize); pauseScreen(); break;
            case 8: {
                clearScreen(); float cg;
                cout << "Enter minimum CGPA: "; cin >> cg;
                showSummary(cg); pauseScreen();
                break;
            }
            case 0: clearScreen(); cout << "Exiting admin module...\n"; break;
            default: cout << "Invalid option!\n"; pauseScreen();
        }
    } while (choice != 0);
}

int main() {
    if (verifyAdminLogin()) {
        cin.ignore();
        pauseScreen();
        adminMenu();
    } else {
        cout << "Access denied.\n";
    }
    return 0;
}
=======
// Admin Module – Edwin Teo's Code (TDS4223 Internship System)
// Final version with login, menu, exception handling, OOP, sorting prep

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <stdexcept>
using namespace std;

const int TABLE_SIZE = 10;

// Struct to store login details
struct Login {
    string username;
    string password;
};

// Base class for users
class User {
protected:
    string role;
public:
    User(string r) : role(r) {
        cout << role << " module activated.\n";
    }
    virtual ~User() {
        cout << role << " module closing...\n";
    }
};

// Derived class for Admin
class Admin : public User {
public:
    Admin() : User("Admin") {}
};

// Struct to store student data
struct Student {
    string name, studentID, email, diploma, skills, appliedJob;
    float cgpa;
    Student* next;

    Student(string n, string id, string e, float g, string d, string s, string j)
        : name(n), studentID(id), email(e), cgpa(g), diploma(d), skills(s), appliedJob(j), next(NULL) {}

    ~Student() {}

    friend void printStudent(const Student*);
};

void printStudent(const Student* s) {
    cout << s->name << " | " << s->studentID << " | " << s->email << " | CGPA: " << s->cgpa << "\n";
}

Student* hashTable[TABLE_SIZE] = {NULL};
Student* sortedArray[100];
int arraySize = 0;

int hashFunction(string studentID) {
    long long id = atol(studentID.c_str());
    return id % TABLE_SIZE;
}

Student* createStudent(string name, string id, string email, float cgpa, string diploma, string skills, string job) {
    Student* newStudent = new Student(name, id, email, cgpa, diploma, skills, job);
    return newStudent;
}

void insertStudent(Student* student) {
    int index = hashFunction(student->studentID);
    student->next = hashTable[index];
    hashTable[index] = student;
}

void loadData() {
    try {
        ifstream file("raw data.txt");
        if (!file.is_open()) throw runtime_error("File not found.");

        string line;
        getline(file, line); getline(file, line);

        while(getline(file, line)) {
            stringstream ss(line);
            string name, id, email, cgpaStr, diploma, skills, job;
            getline(ss, name, '|'); getline(ss, id, '|'); getline(ss, email, '|');
            getline(ss, cgpaStr, '|'); getline(ss, diploma, '|'); getline(ss, skills, '|'); getline(ss, job);

            float cgpa = atof(cgpaStr.c_str());
            Student* student = createStudent(name, id, email, cgpa, diploma, skills, job);
            insertStudent(student);
        }
        file.close();
        cout << "Data loaded from raw data.txt successfully.\n";
    } catch (exception& e) {
        cerr << "Error loading file: " << e.what() << "\n";
    }
}

void displayData() {
    cout << "\n--- Student Records ---\n";
    for(int i = 0; i < TABLE_SIZE; i++) {
        Student* current = hashTable[i];
        while(current != NULL) {
            printStudent(current);
            current = current->next;
        }
    }
}

void addNewStudent() {
    try {
        string name, id, email, diploma, skills, job;
        float cgpa;

        cin.ignore();
        cout << "Enter Name: "; getline(cin, name);
        cout << "Enter Student ID: "; getline(cin, id);
        cout << "Enter Email: "; getline(cin, email);
        cout << "Enter CGPA: "; cin >> cgpa;
        if (cin.fail()) throw invalid_argument("Invalid CGPA input.");
        cin.ignore();
        cout << "Enter Diploma: "; getline(cin, diploma);
        cout << "Enter Skills: "; getline(cin, skills);
        cout << "Enter Applied Job: "; getline(cin, job);

        Student* student = createStudent(name, id, email, cgpa, diploma, skills, job);
        insertStudent(student);

        cout << "Student added successfully.\n";
    } catch (exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}

void convertToArray() {
    arraySize = 0;
    for(int i = 0; i < TABLE_SIZE; i++) {
        Student* current = hashTable[i];
        while(current != NULL && arraySize < 100) {
            sortedArray[arraySize++] = current;
            current = current->next;
        }
    }
    cout << "Data converted to array for sorting/searching.\n";
}

void displaySortedData() {
    cout << "\n--- Sorted Student Records ---\n";
    for(int i = 0; i < arraySize; i++) {
        printStudent(sortedArray[i]);
    }
}

void saveSortedData() {
    try {
        ofstream out("sorted_information.txt");
        if (!out.is_open()) throw runtime_error("Unable to open file.");
        for(int i = 0; i < arraySize; i++) {
            out << sortedArray[i]->name << " | " << sortedArray[i]->studentID << " | "
                << sortedArray[i]->email << " | CGPA: " << sortedArray[i]->cgpa << "\n";
        }
        out.close();
        cout << "Sorted data saved to sorted_information.txt.\n";
    } catch (exception& e) {
        cerr << "Error saving file: " << e.what() << "\n";
    }
}

void showSummary(int count) {
    cout << "Total records in system: " << count << "\n";
}

void showSummary(float cgpa) {
    cout << "Listing students with CGPA above " << cgpa << ":\n";
    for(int i = 0; i < arraySize; i++) {
        if(sortedArray[i]->cgpa >= cgpa) printStudent(sortedArray[i]);
    }
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
}

void clearScreen() {
    system("cls");
}

bool verifyAdminLogin() {
    string inputUser, inputPass;
    cout << "==== Admin Login ====" << endl;
    cout << "Username: "; cin >> inputUser;
    cout << "Password: "; cin >> inputPass;

    ifstream file("login_admin.txt");
    if (!file.is_open()) {
        cerr << "Unable to open login_admin.txt\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        size_t delimPos = line.find(',');
        string storedUser = line.substr(0, delimPos);
        string storedPass = line.substr(delimPos + 1);

        if (inputUser == storedUser && inputPass == storedPass) {
            cout << "Login successful.\n";
            file.close();
            return true;
        }
    }
    file.close();
    cout << "Invalid username or password.\n";
    return false;
}

void adminMenu() {
    int choice;
    Admin admin;

    do {
        clearScreen();
        cout << "==== Internship System - Admin Panel ====" << endl;
        cout << "1. Load Student Data\n2. Display All Records\n3. Add New Student\n4. Convert to Array\n5. Display Sorted Data\n6. Save Sorted Data\n7. Show Summary (Total Records)\n8. Show Summary (CGPA >= X)\n0. Exit\nSelect an option: ";
        cin >> choice;

        switch(choice) {
            case 1: clearScreen(); loadData(); pauseScreen(); break;
            case 2: clearScreen(); displayData(); pauseScreen(); break;
            case 3: clearScreen(); addNewStudent(); pauseScreen(); break;
            case 4: clearScreen(); convertToArray(); pauseScreen(); break;
            case 5: clearScreen(); displaySortedData(); pauseScreen(); break;
            case 6: clearScreen(); saveSortedData(); pauseScreen(); break;
            case 7: clearScreen(); showSummary(arraySize); pauseScreen(); break;
            case 8: {
                clearScreen(); float cg;
                cout << "Enter minimum CGPA: "; cin >> cg;
                showSummary(cg); pauseScreen();
                break;
            }
            case 0: clearScreen(); cout << "Exiting admin module...\n"; break;
            default: cout << "Invalid option!\n"; pauseScreen();
        }
    } while (choice != 0);
}

int main() {
    if (verifyAdminLogin()) {
        cin.ignore();
        pauseScreen();
        adminMenu();
    } else {
        cout << "Access denied.\n";
    }
    return 0;
}
>>>>>>> 265e263691156da9ea9e807b0d8c33b0a2649a8a
