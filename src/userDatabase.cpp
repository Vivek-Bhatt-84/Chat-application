#include "../include/userDatabase.h"
#include "../include/users.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <fstream>

UserDatabase::UserDatabase() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = nullptr;
    connectDb(); // Automatically connect to DB in constructor
}

// Destructor - Close database connection
UserDatabase::~UserDatabase() {
    if (con) {
        delete con;
    }
}

// Connect to MySQL database
void UserDatabase::connectDb() {
    try {
        con = driver->connect("tcp://127.0.0.1:3306", "root", "Bhatt@20092003");
        con->setSchema("ChatApp");
        cout << "Database connection successful!" << endl;
    } catch (sql::SQLException &e) {
        cerr << "Database connection failed: " << e.what() << endl;
        con = nullptr; // Ensure con is nullptr if the connection fails
    }
}

// Register a user
void UserDatabase::registerUser(const User& user) {
    if (!con) {
        cerr << "Database not connected!" << endl;
        return;
    }

    try {
        sql::PreparedStatement* psmt = con->prepareStatement(
            "INSERT INTO users (username, password, role) VALUES (?, ?, ?)"
        );

        psmt->setString(1, user.getUsername());
        psmt->setString(2, user.getPassword());  // Ensure password is hashed
        psmt->setString(3, user.getRole());  

        cout << "📌 Query about to execute: INSERT INTO users ("
        << user.getUsername() << ", " 
        << user.getPassword() << ", " 
        << user.getRole() << ")\n";

        psmt->execute();
        cout << "User registered successfully!" << endl;

        delete psmt;
    } catch (sql::SQLException &e) {
        cerr << "User registration failed: " << e.what() << endl;
    }
}


// Login user
bool UserDatabase::loginUser(const string& uName, const string& pass) {
    cout << "✅ Connected to database in login function UserDatabase::loginUser" << endl;

    if (!con) {
        cerr << "❌ Database not connected!" << endl;
        return false;
    }

    sql::PreparedStatement* psmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        psmt = con->prepareStatement(
            "SELECT password FROM users WHERE username = ?"
        );

        psmt->setString(1, uName);
        res = psmt->executeQuery();

        if (res->next()) {  // ✅ User exists
            string storedPassword = res->getString("password");

            if (User::verifyPassword(pass, storedPassword)) {
                cout << "✅ Login successful!\n";
                delete res;
                delete psmt;
                return true;
            } else {
                cout << "❌ Incorrect password. Please try again.\n";
            }
        } else {
            cout << "❌ User not found!\n";
        }
    } catch (sql::SQLException &e) {
        cerr << "❌ Login failed: " << e.what() << endl;
    }

    delete res;
    delete psmt;
    return false;  // ✅ Return false if login fails
}


void UserDatabase::sendMessage(const string& sender, const string& receiver, const string& message) {
    if (!con) {
        cerr << "❌ Database not connected!" << endl;
        return;
    }

    try {
        sql::PreparedStatement* psmt = con->prepareStatement(
            "INSERT INTO messages (sender, receiver, message) VALUES (?, ?, ?)"
        );

        psmt->setString(1, sender);
        psmt->setString(2, receiver);
        psmt->setString(3, message);
        
        psmt->execute();
        cout << "📨 Message sent from " << sender << " to " << receiver << endl;
        
        delete psmt;
    } catch (sql::SQLException &e) {
        cerr << "❌ Failed to send message: " << e.what() << endl;
    }
}

vector<string> UserDatabase::fetchMessages(const string& username) {
    vector<string> messages;
    if (!con) {
        cerr << "❌ Database not connected!" << endl;
        return messages;
    }

    try {
        sql::PreparedStatement* psmt = con->prepareStatement(
            "SELECT sender, message, timestamp FROM messages WHERE receiver = ? ORDER BY timestamp ASC"
        );

        psmt->setString(1, username);
        sql::ResultSet* res = psmt->executeQuery();

        while (res->next()) {
            string sender = res->getString("sender");
            string msg = res->getString("message");
            string time = res->getString("timestamp");
            messages.push_back("📩 From " + sender + " at " + time + ": " + msg);
        }

        delete res;
        delete psmt;
    } catch (sql::SQLException &e) {
        cerr << "❌ Failed to fetch messages: " << e.what() << endl;
    }

    return messages;
}

void UserDatabase::storeMessage(const string& recipient, const string& sender, const string& message) {
    ofstream file("messages.txt", ios::app);
    if (file.is_open()) {
        file << recipient << " " << sender << " " << message << endl;
        file.close();
    }
}


vector<string> UserDatabase::getOfflineMessages(const string& recipient) {
    ifstream file("messages.txt");
    vector<string> messages;
    string r, sender, msg;

    if (file.is_open()) {
        while (file >> r >> sender) {
            getline(file, msg);
            if (r == recipient) {
                messages.push_back(sender + ": " + msg);
            }
        }
        file.close();
    }
    return messages;
}

void UserDatabase::deleteMessages(const string& recipient) {
    ifstream file("messages.txt");
    ofstream temp("temp.txt");

    string r, sender, msg;
    if (file.is_open() && temp.is_open()) {
        while (file >> r >> sender) {
            getline(file, msg);
            if (r != recipient) { 
                temp << r << " " << sender << msg << endl;
            }
        }
        file.close();
        temp.close();
        remove("messages.txt");
        rename("temp.txt", "messages.txt");
    }
}

