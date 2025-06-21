#include "error_handling.hpp"

bool fileExists(const string& filename) {
    ifstream file(filename.c_str());
    return file.good();
}

bool openFileSafely(const string& filename, ifstream& fileRef) {
    fileRef.open(filename.c_str());
    return fileRef.good();
}

int getValidatedChoice(int min, int max) {
    int choice;
    while (true) {
        cout << "Enter your choice (" << min << " - " << max << "): ";
        if (cin >> choice) {
            if (choice >= min && choice <= max) {
                return choice;
            } else {
                cout << "??  Invalid choice. Please enter between " << min << " and " << max << ".\n";
            }
        } else {
            cout << "??  Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}
