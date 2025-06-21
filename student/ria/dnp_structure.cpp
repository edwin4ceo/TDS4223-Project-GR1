#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Application {
    string studentID;
    string jobID;
    string jobTitle;
    string company;
    string status;
    Application* next;
};

// Function to create a new node
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

// Function to insert at end of linked list
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

// Function to load from file and build linked list
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

// Function to display applications from linked list
void displayApplications(Application* head) {
    cout << "\nApplications in Memory (Linked List):\n";
    cout << "--------------------------------------------------\n";
    Application* current = head;
    while (current != NULL) {
        cout << "Student ID: " << current->studentID << endl;
        cout << "Job ID    : " << current->jobID << endl;
        cout << "Title     : " << current->jobTitle << endl;
        cout << "Company   : " << current->company << endl;
        cout << "Status    : " << current->status << endl;
        cout << "--------------------------------------------------\n";
        current = current->next;
    }
}

int main() {
    Application* head = NULL;

    loadApplications(head);
    displayApplications(head);

    // Clean up memory
    Application* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}

