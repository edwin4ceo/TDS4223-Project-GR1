#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <limits>
using namespace std;

// Include existing structures
#include "student_structures.hpp"
#include "student_utils.hpp"

// Customer structure
struct Customer {
    string customerID;
    string name;
    string email;
    string phone;
    float totalSpent;
    int loyaltyPoints;
    string membershipLevel;
    time_t lastPurchase;
    vector<string> orderHistory;
    vector<string> ticketHistory;
};

// Order structure
struct Order {
    string orderID;
    string customerID;
    string product;
    float amount;
    string status;
    time_t orderDate;
    string paymentMethod;
};

// Support Ticket structure
struct SupportTicket {
    string ticketID;
    string customerID;
    string issue;
    string status;
    time_t createdDate;
    time_t resolvedDate;
};

// Global data
vector<Customer> customers;
vector<Order> orders;
vector<SupportTicket> tickets;
bool customerLoggedIn = false;
string currentCustomerID = "";

// Utility functions
string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt).substr(0, 24);
}

bool isValidEmail(const string& email) {
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool isValidPhone(const string& phone) {
    return phone.length() >= 10 && phone.find_first_not_of("0123456789-") == string::npos;
}

float stringToFloat(const string& str) {
    try { return stof(str); } catch (...) { return 0.0; }
}

// Load customers from file
int loadCustomers(const string& filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) { cout << "Error: Unable to open " << filename << " at " << getCurrentTime() << "\n"; return 0; }
    customers.clear(); string line;
    while (getline(file, line)) {
        stringstream ss(line); vector<string> fields = splitLine(line, '|');
        if (fields.size() >= 8) {
            Customer c; c.customerID = fields[0]; c.name = fields[1]; c.email = fields[2]; c.phone = fields[3];
            c.totalSpent = stringToFloat(fields[4]); c.loyaltyPoints = stoi(fields[5]); c.membershipLevel = fields[6];
            c.lastPurchase = stringToFloat(fields[7]) ? time_t(stringToFloat(fields[7])) : time(0); c.orderHistory.clear(); c.ticketHistory.clear();
            for (int i = 8; i < fields.size() && i < 108; i++) c.orderHistory.push_back(fields[i]); // Up to 100 orders
            customers.push_back(c);
        }
    }
    file.close(); cout << "Loaded " << customers.size() << " customers at " << getCurrentTime() << "\n"; return customers.size();
}

// Load orders from file
int loadOrders(const string& filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) { cout << "Error: Unable to open " << filename << " at " << getCurrentTime() << "\n"; return 0; }
    orders.clear(); string line;
    while (getline(file, line)) {
        stringstream ss(line); vector<string> fields = splitLine(line, '|');
        if (fields.size() >= 7) {
            Order o; o.orderID = fields[0]; o.customerID = fields[1]; o.product = fields[2]; o.amount = stringToFloat(fields[3]);
            o.status = fields[4]; o.orderDate = stringToFloat(fields[5]) ? time_t(stringToFloat(fields[5])) : time(0); o.paymentMethod = fields[6];
            orders.push_back(o);
        }
    }
    file.close(); cout << "Loaded " << orders.size() << " orders at " << getCurrentTime() << "\n"; return orders.size();
}

// Load support tickets from file
int loadTickets(const string& filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) { cout << "Error: Unable to open " << filename << " at " << getCurrentTime() << "\n"; return 0; }
    tickets.clear(); string line;
    while (getline(file, line)) {
        stringstream ss(line); vector<string> fields = splitLine(line, '|');
        if (fields.size() >= 6) {
            SupportTicket t; t.ticketID = fields[0]; t.customerID = fields[1]; t.issue = fields[2]; t.status = fields[3];
            t.createdDate = stringToFloat(fields[4]) ? time_t(stringToFloat(fields[4])) : time(0);
            t.resolvedDate = fields[5].empty() ? 0 : time_t(stringToFloat(fields[5]));
            tickets.push_back(t);
        }
    }
    file.close(); cout << "Loaded " << tickets.size() << " tickets at " << getCurrentTime() << "\n"; return tickets.size();
}

// Display customer details
void displayCustomer(const Customer& c) {
    cout << "\n--- Customer Details ---\n";
    cout << "ID          : " << c.customerID << endl;
    cout << "Name        : " << c.name << endl;
    cout << "Email       : " << c.email << endl;
    cout << "Phone       : " << c.phone << endl;
    cout << "Total Spent : $" << fixed << setprecision(2) << c.totalSpent << endl;
    cout << "Loyalty Pts : " << c.loyaltyPoints << endl;
    cout << "Membership  : " << c.membershipLevel << endl;
    cout << "Last Purchase: " << ctime(&c.lastPurchase) << endl;
    cout << "Order History (" << c.orderHistory.size() << "): ";
    for (const auto& order : c.orderHistory) cout << order << " "; cout << endl;
    cout << "Ticket History (" << c.ticketHistory.size() << "): ";
    for (const auto& ticket : c.ticketHistory) cout << ticket << " "; cout << endl;
}

// Display all customers
void displayAllCustomers() {
    cout << "\n--- All Customers ---\n";
    for (const auto& c : customers) displayCustomer(c);
}

// Register new customer with validation
void registerCustomer() {
    if (customers.size() >= 150) { cout << "Capacity reached at " << getCurrentTime() << "\n"; return; }
    Customer c; string input; int attempts = 3;
    do { cout << "Enter ID: "; getline(cin, input); c.customerID = trim(input); if (c.customerID.length() < 4) { cout << "Too short. Attempts: " << --attempts << "\n"; continue; } break; } while (attempts > 0);
    attempts = 3; do { cout << "Enter Name: "; getline(cin, input); if (input.length() < 2) { cout << "Too short. Attempts: " << --attempts << "\n"; continue; } c.name = trim(input); break; } while (attempts > 0);
    attempts = 3; do { cout << "Enter Email: "; getline(cin, input); if (!isValidEmail(input)) { cout << "Invalid email. Attempts: " << --attempts << "\n"; continue; } c.email = trim(input); break; } while (attempts > 0);
    attempts = 3; do { cout << "Enter Phone: "; getline(cin, input); if (!isValidPhone(input)) { cout << "Invalid phone. Attempts: " << --attempts << "\n"; continue; } c.phone = trim(input); break; } while (attempts > 0);
    c.totalSpent = 0.0; c.loyaltyPoints = 0; c.membershipLevel = "Basic"; c.lastPurchase = time(0); c.orderHistory.clear(); c.ticketHistory.clear();
    customers.push_back(c);
    ofstream file("../shared/customers.txt", ios::app);
    if (file.is_open()) {
        file << c.customerID << "|" << c.name << "|" << c.email << "|" << c.phone << "|" << c.totalSpent << "|"
             << c.loyaltyPoints << "|" << c.membershipLevel << "|" << c.lastPurchase;
        for (const auto& order : c.orderHistory) file << "|" << order; file << "\n";
        file.close(); cout << "Registered at " << getCurrentTime() << "\n";
    } else cout << "Error saving at " << getCurrentTime() << "\n";
}

// Edit customer profile
void editCustomerProfile(const string& targetID) {
    bool found = false;
    for (auto& c : customers) {
        if (c.customerID == targetID) {
            found = true; cout << "\nCurrent Information:\n"; displayCustomer(c);
            string input; int attempts = 3; do { cout << "New Email (- to keep): "; getline(cin, input); if (input != "-" && !isValidEmail(input)) { cout << "Invalid. Attempts: " << --attempts << "\n"; continue; } if (input != "-") c.email = input; break; } while (attempts > 0);
            attempts = 3; do { cout << "New Phone (- to keep): "; getline(cin, input); if (input != "-" && !isValidPhone(input)) { cout << "Invalid. Attempts: " << --attempts << "\n"; continue; } if (input != "-") c.phone = input; break; } while (attempts > 0);
            break;
        }
    }
    if (!found) cout << "Customer ID not found at " << getCurrentTime() << "\n"; else saveCustomers();
}

// Save customers to file
void saveCustomers() {
    ofstream file("../shared/customers.txt");
    if (!file.is_open()) { cout << "Error saving at " << getCurrentTime() << "\n"; return; }
    for (const auto& c : customers) {
        file << c.customerID << "|" << c.name << "|" << c.email << "|" << c.phone << "|" << c.totalSpent << "|"
             << c.loyaltyPoints << "|" << c.membershipLevel << "|" << c.lastPurchase;
        for (const auto& order : c.orderHistory) file << "|" << order; file << "\n";
    }
    file.close(); cout << "Saved at " << getCurrentTime() << "\n";
}

// Login customer
bool loginCustomer(const string& id) {
    for (const auto& c : customers) if (c.customerID == id) { currentCustomerID = id; customerLoggedIn = true; cout << "Login successful at " << getCurrentTime() << "\n"; return true; }
    cout << "Login failed at " << getCurrentTime() << "\n"; return false;
}

// Logout customer
void logoutCustomer() {
    char confirm; cout << "Confirm logout (y/n): "; cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') { customerLoggedIn = false; currentCustomerID = ""; cout << "Logged out at " << getCurrentTime() << "\n"; }
    else cout << "Logout cancelled at " << getCurrentTime() << "\n";
}

// Place order with validation
void placeOrder() {
    if (!customerLoggedIn) { cout << "Please login first at " << getCurrentTime() << "\n"; return; }
    Order o; o.orderID = "O" + to_string(time(0)); o.customerID = currentCustomerID; string input; int attempts = 3;
    do { cout << "Enter Product: "; getline(cin, input); if (input.length() < 2) { cout << "Too short. Attempts: " << --attempts << "\n"; continue; } o.product = trim(input); break; } while (attempts > 0);
    attempts = 3; do { cout << "Enter Amount ($): "; getline(cin, input); o.amount = stringToFloat(input); if (o.amount <= 0) { cout << "Invalid amount. Attempts: " << --attempts << "\n"; continue; } break; } while (attempts > 0);
    o.status = "Pending"; o.orderDate = time(0); cout << "Payment Method (Cash/Card): "; getline(cin, input); o.paymentMethod = trim(input);
    orders.push_back(o); for (auto& c : customers) if (c.customerID == currentCustomerID) { c.totalSpent += o.amount; c.orderHistory.push_back(o.orderID); c.lastPurchase = time(0); }
    ofstream file("../shared/orders.txt", ios::app); file << o.orderID << "|" << o.customerID << "|" << o.product << "|" << o.amount << "|" << o.status << "|" << o.orderDate << "|" << o.paymentMethod << "\n"; file.close();
    cout << "Order placed at " << getCurrentTime() << "\n"; updateLoyalty();
}

// View order history
void viewOrderHistory() {
    if (!customerLoggedIn) { cout << "Please login first at " << getCurrentTime() << "\n"; return; }
    bool found = false; for (const auto& c : customers) if (c.customerID == currentCustomerID) {
        cout << "\n--- Order History ---\n"; for (const auto& orderID : c.orderHistory) {
            for (const auto& o : orders) if (o.orderID == orderID) { cout << "Order ID: " << o.orderID << ", Product: " << o.product << ", Amount: $" << o.amount << ", Status: " << o.status << ", Date: " << ctime(&o.orderDate); found = true; }
        }
    }
    if (!found) cout << "No orders at " << getCurrentTime() << "\n";
}

// Update order status
void updateOrderStatus(const string& orderID, const string& newStatus) {
    for (auto& o : orders) if (o.orderID == orderID) { o.status = newStatus; saveOrders(); cout << "Updated at " << getCurrentTime() << "\n"; return; }
    cout << "Order not found at " << getCurrentTime() << "\n";
}

// Save orders to file
void saveOrders() {
    ofstream file("../shared/orders.txt");
    if (!file.is_open()) { cout << "Error saving at " << getCurrentTime() << "\n"; return; }
    for (const auto& o : orders) {
        file << o.orderID << "|" << o.customerID << "|" << o.product << "|" << o.amount << "|" << o.status << "|" << o.orderDate << "|" << o.paymentMethod << "\n";
    }
    file.close();
}

// Create support ticket
void createSupportTicket() {
    if (!customerLoggedIn) { cout << "Please login first at " << getCurrentTime() << "\n"; return; }
    SupportTicket t; t.ticketID = "T" + to_string(time(0)); t.customerID = currentCustomerID; string input; int attempts = 3;
    do { cout << "Enter Issue: "; getline(cin, input); if (input.length() < 5) { cout << "Too short. Attempts: " << --attempts << "\n"; continue; } t.issue = trim(input); break; } while (attempts > 0);
    t.status = "Open"; t.createdDate = time(0); t.resolvedDate = 0;
    tickets.push_back(t); for (auto& c : customers) if (c.customerID == currentCustomerID) c.ticketHistory.push_back(t.ticketID);
    ofstream file("../shared/tickets.txt", ios::app); file << t.ticketID << "|" << t.customerID << "|" << t.issue << "|" << t.status << "|" << t.createdDate << "|0\n"; file.close();
    cout << "Ticket created at " << getCurrentTime() << "\n";
}

// View support tickets
void viewSupportTickets() {
    if (!customerLoggedIn) { cout << "Please login first at " << getCurrentTime() << "\n"; return; }
    bool found = false; for (const auto& c : customers) if (c.customerID == currentCustomerID) {
        cout << "\n--- Support Tickets ---\n"; for (const auto& ticketID : c.ticketHistory) {
            for (const auto& t : tickets) if (t.ticketID == ticketID) { cout << "Ticket ID: " << t.ticketID << ", Issue: " << t.issue << ", Status: " << t.status << ", Created: " << ctime(&t.createdDate) << (t.resolvedDate ? ", Resolved: " + string(ctime(&t.resolvedDate)) : ""); found = true; }
        }
    }
    if (!found) cout << "No tickets at " << getCurrentTime() << "\n";
}

// Resolve support ticket
void resolveSupportTicket(const string& ticketID) {
    for (auto& t : tickets) if (t.ticketID == ticketID && t.customerID == currentCustomerID) { t.status = "Resolved"; t.resolvedDate = time(0); saveTickets(); cout << "Resolved at " << getCurrentTime() << "\n"; return; }
    cout << "Ticket not found or unauthorized at " << getCurrentTime() << "\n";
}

// Save tickets to file
void saveTickets() {
    ofstream file("../shared/tickets.txt");
    if (!file.is_open()) { cout << "Error saving at " << getCurrentTime() << "\n"; return; }
    for (const auto& t : tickets) {
        file << t.ticketID << "|" << t.customerID << "|" << t.issue << "|" << t.status << "|" << t.createdDate << "|" << t.resolvedDate << "\n";
    }
    file.close();
}

// Update loyalty points
void updateLoyalty() {
    for (auto& c : customers) if (c.customerID == currentCustomerID) {
        float total = c.totalSpent; if (total >= 500) { c.loyaltyPoints += int(total / 100); c.membershipLevel = "Gold"; }
        else if (total >= 200) { c.loyaltyPoints += int(total / 50); c.membershipLevel = "Silver"; }
        else { c.loyaltyPoints += int(total / 25); c.membershipLevel = "Basic"; }
        saveCustomers(); cout << "Loyalty updated at " << getCurrentTime() << "\n";
    }
}

// Redeem loyalty points
void redeemLoyalty() {
    if (!customerLoggedIn) { cout << "Please login first at " << getCurrentTime() << "\n"; return; }
    for (auto& c : customers) if (c.customerID == currentCustomerID) {
        cout << "Current Points: " << c.loyaltyPoints << "\n"; int points; cout << "Points to redeem: "; cin >> points;
        if (points <= c.loyaltyPoints && points >= 50) { c.loyaltyPoints -= points; c.totalSpent -= points * 0.5; cout << "Redeemed " << points << " points at " << getCurrentTime() << "\n"; }
        else cout << "Invalid redemption at " << getCurrentTime() << "\n"; saveCustomers();
    }
}

// Generate customer report
void generateCustomerReport() {
    if (!customerLoggedIn) { cout << "Please login first at " << getCurrentTime() << "\n"; return; }
    ofstream file("../shared/customer_report.txt");
    if (!file.is_open()) { cout << "Error saving at " << getCurrentTime() << "\n"; return; }
    for (const auto& c : customers) if (c.customerID == currentCustomerID) {
        file << "=== Customer Report ===\nID: " << c.customerID << "\nName: " << c.name << "\nTotal Spent: $" << c.totalSpent << "\nLoyalty Points: " << c.loyaltyPoints << "\nMembership: " << c.membershipLevel << "\nLast Purchase: " << ctime(&c.lastPurchase) << "\nOrders:\n";
        for (const auto& orderID : c.orderHistory) for (const auto& o : orders) if (o.orderID == orderID) file << o.orderID << "|" << o.product << "|" << o.amount << "|" << o.status << "\n";
        file << "Tickets:\n"; for (const auto& ticketID : c.ticketHistory) for (const auto& t : tickets) if (t.ticketID == ticketID) file << t.ticketID << "|" << t.issue << "|" << t.status << "\n";
    }
    file.close(); cout << "Report saved at " << getCurrentTime() << "\n";
}

// Customer menu
void customerMenu() {
    int choice; do {
        cout << "\n==== Customer Panel ====\n";
        cout << "1. Logout\n2. Edit Profile\n3. Place Order\n4. View Order History\n5. Create Support Ticket\n6. View Tickets\n7. Redeem Loyalty Points\n8. Generate Report\n0. Exit\nChoice: ";
        cin >> choice; cin.ignore();
        switch (choice) {
            case 1: logoutCustomer(); return;
            case 2: editCustomerProfile(currentCustomerID); break;
            case 3: placeOrder(); break;
            case 4: viewOrderHistory(); break;
            case 5: createSupportTicket(); break;
            case 6: viewSupportTickets(); break;
            case 7: redeemLoyalty(); break;
            case 8: generateCustomerReport(); break;
            case 0: cout << "Exiting at " << getCurrentTime() << "\n"; return;
            default: cout << "Invalid at " << getCurrentTime() << "\n";
        }
    } while (true);
}

// Main function with initialization
int main() {
    // Initialize customers (100)
    string customerData[] = {
        "C001|John Doe|john@customer.com|012-3456789|0.00|0|Basic|0",
        "C002|Jane Smith|jane@customer.com|012-3456790|150.50|3|Basic|1718970000",
        "C003|Ahmad Ali|ahmad@customer.com|012-3456791|300.75|6|Silver|1719056400",
        // ... (97 more entries, truncated for brevity, each with unique ID, name, email, phone, spent, points, level, last purchase)
        "C100|Zara Khan|zara@customer.com|012-3456888|1200.00|24|Gold|1719634800"
    };
    for (int i = 0; i < 100; i++) {
        stringstream ss(customerData[i % 3]); vector<string> fields = splitLine(customerData[i % 3], '|');
        Customer c; c.customerID = "C" + to_string(i + 1); c.name = fields[1]; c.email = fields[2]; c.phone = fields[3];
        c.totalSpent = stringToFloat(fields[4]); c.loyaltyPoints = stoi(fields[5]); c.membershipLevel = fields[6];
        c.lastPurchase = stringToFloat(fields[7]) ? time_t(stringToFloat(fields[7])) : time(0); c.orderHistory.clear(); c.ticketHistory.clear();
        customers.push_back(c);
        ofstream file("../shared/customers.txt", ios::app); file << c.customerID << "|" << c.name << "|" << c.email << "|" << c.phone << "|" << c.totalSpent << "|" << c.loyaltyPoints << "|" << c.membershipLevel << "|" << c.lastPurchase << "\n"; file.close();
    }

    // Initialize orders (200)
    for (int i = 0; i < 200; i++) {
        Order o; o.orderID = "O" + to_string(time(0) + i); o.customerID = "C" + to_string(rand() % 100 + 1);
        o.product = "Product" + to_string(rand() % 10 + 1); o.amount = (rand() % 500 + 50) / 10.0;
        o.status = (rand() % 3 == 0) ? "Completed" : "Pending"; o.orderDate = time(0) - rand() % 864000; o.paymentMethod = (rand() % 2 == 0) ? "Cash" : "Card";
        orders.push_back(o); ofstream file("../shared/orders.txt", ios::app); file << o.orderID << "|" << o.customerID << "|" << o.product << "|" << o.amount << "|" << o.status << "|" << o.orderDate << "|" << o.paymentMethod << "\n"; file.close();
    }

    // Initialize tickets (50)
    for (int i = 0; i < 50; i++) {
        SupportTicket t; t.ticketID = "T" + to_string(time(0) + i); t.customerID = "C" + to_string(rand() % 100 + 1);
        t.issue = "Issue" + to_string(rand() % 5 + 1); t.status = (rand() % 2 == 0) ? "Open" : "Resolved";
        t.createdDate = time(0) - rand() % 864000; t.resolvedDate = (t.status == "Resolved") ? t.createdDate + rand() % 86400 : 0;
        tickets.push_back(t); ofstream file("../shared/tickets.txt", ios::app); file << t.ticketID << "|" << t.customerID << "|" << t.issue << "|" << t.status << "|" << t.createdDate << "|" << t.resolvedDate << "\n"; file.close();
    }

    // Main loop
    int choice; do {
        cout << "\n==== Customer System ====\n";
        cout << "1. Login\n2. Register\n3. Exit\nChoice: ";
        cin >> choice; cin.ignore();
        switch (choice) {
            case 1: { string id; cout << "Enter ID: "; cin >> id; if (loginCustomer(id)) customerMenu(); break; }
            case 2: registerCustomer(); break;
            case 3: cout << "Exiting at " << getCurrentTime() << "\n"; break;
            default: cout << "Invalid at " << getCurrentTime() << "\n";
        }
    } while (choice != 3);

    return 0;
}
