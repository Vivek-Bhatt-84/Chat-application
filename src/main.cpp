#include "../include/users.h"
#include "../include/userDatabase.h"
#include <iostream>

using namespace std;

void signupPage() {
    cout << "----------SIGN UP MENU ------------" << endl;
    UserDatabase db;
    string username, password, role;

    cout << "\n--- Signup Page ---\n";
    cout << "Enter username: ";
    cin.ignore();
    getline(cin, username);

    cout << "Enter password: ";
    getline(cin, password);

    cout << "Enter role (user/admin): ";
    getline(cin, role);

    User user = UserBuilder().setUsername(username).setPassword(password).setRole(role).build();
    db.registerUser(user);

    cout << "✅ User " << username << " registered successfully!\n";
}

void enterCredentials(string& username, string& password) {
    cout << "\nEnter username: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
}

void loginPage() {
    cout << "------------ LOGIN MENU ----------" << endl;
    UserDatabase db;
    string username, password;
    int attempts = 3;

    enterCredentials(username, password);

    while (attempts > 0) {
        if (db.loginUser(username, password)) {
            cout << "✅ Login successful! Welcome, " << username << "!\n";
            return;
        } else {
            attempts--;
            if (attempts > 0) {
                cout << "❌ Incorrect password. Attempts left: " << attempts << "\n";
                cout << "Please try again.\n";
                enterCredentials(username, password);
            }
        }
    }

    cout << "\nToo many failed attempts! Redirecting to signup...\n";
    signupPage();
}

void messageUser() {
    UserDatabase db;
    string sender, receiver, message;
    
    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, sender);
    
    cout << "Enter recipient username: ";
    getline(cin, receiver);
    
    cout << "Enter message: ";
    getline(cin, message);
    
    db.sendMessage(sender, receiver, message);
    cout << "✅ Message sent successfully!\n";
}

void checkMessages() {
    UserDatabase db;
    string username;
    
    cout << "Enter your username to check messages: ";
    cin.ignore();
    getline(cin, username);
    
    vector<string> messages = db.fetchMessages(username);
    
    if (messages.empty()) {
        cout << "📭 No new messages!\n";
    } else {
        cout << "📨 Your messages:\n";
        for (const string& msg : messages) {
            cout << msg << endl;
        }
    }
}

void driver() {
    while (true) {
        int choice;
        cout << "\n------ Chat Application ------\n";
        cout << "1️⃣ Signup\n2️⃣ Login\n3️⃣ Send Message\n4️⃣ Check Messages\n5️⃣ Exit\nChoose an option: ";
        cin >> choice;
        cin.ignore(); // Ignore leftover newline in buffer

        switch (choice) {
            case 1:
                signupPage();
                break;
            case 2:
                loginPage();
                break;
            case 3:
                messageUser();
                break;
            case 4:
                checkMessages();
                break;
            case 5:
                cout << "👋 Exiting...\n";
                return;
            default:
                cout << "❌ Invalid choice! Try again.\n";
        }
    }
}

int main() {
    cout << "--- Welcome to the Chat App ---\n";
    driver();
    return 0;
}
