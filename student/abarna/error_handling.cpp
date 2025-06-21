#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

// Check if a file exists
bool fileExists(const string& filename) {
    ifstream file(filename.c_str());
    return file.good();
}

// Open file safely with exception
ifstream openFileSafely(const string& filename) {
    ifstream file(filename.c_str());
    if (!file) {
        throw runtime_error("? Error: Cannot open file: " + filename);
    }
    return file;
}

// Validate integer input (for menu choices)
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
            cin.clear();            // reset error flag
            cin.ignore(1000, '\n'); // discard invalid input
        }
    }
}

