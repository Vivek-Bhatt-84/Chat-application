#include "../include/userDatabase.h"
#include "../include/users.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

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
void UserDatabase::loginUser(const string& uName, const string& pass) {
    if (!con) {
        cerr << "Database not connected!" << endl;
        return;
    }

    try {
        sql::PreparedStatement* psmt = con->prepareStatement(
            "SELECT password FROM users WHERE username = ?"
        );

        psmt->setString(1, uName);
        sql::ResultSet* res = psmt->executeQuery();

        if (res->next()) {
            string storedPassword = res->getString("password");

            if (User::verifyPassword(pass, storedPassword)) {
                cout << "Login successful!\n";
            } else {
                cout << "Incorrect password!\n";
            }
        } else {
            cout << "User not found!\n";
        }

        delete res;
        delete psmt;
    } catch (sql::SQLException &e) {
        cerr << "Login failed: " << e.what() << endl;
    }
}
