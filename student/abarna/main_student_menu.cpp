#include <iostream>
#include <string>
#include <cstdlib> // for system()
using namespace std;

// Function declarations
void applyInternship(const string& studentID);
void viewApplications(const string& studentID);
void searchInternships();

int main() {
    string studentID;

    cout << "==== Student Login ====\n";
    cout << "Enter your Student ID: ";
    cin >> studentID;

    int choice;
    do {
        system("CLS"); // For Windows; use "clear" for Linux/macOS
        cout << "==== Internship System Menu ====\n";
        cout << "Logged in as: " << studentID << "\n\n";
        cout << "1. Apply for Internship\n";
        cout << "2. View My Applications\n";
        cout << "3. Search Internships\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: applyInternship(studentID); break;
            case 2: viewApplications(studentID); break;
            case 3: searchInternships(); break;
            case 4: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }

        if (choice != 4) {
            cout << "\nPress Enter to return to the menu...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 4);

    return 0;
}

