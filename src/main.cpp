#include "../include/users.h"
#include "../include/userDatabase.h"
#include <iostream>

using namespace std;

int main() {
    UserDatabase db;
    db.connectDb(); // Ensure database connection before registering users.
    string username, password, role;

    // Prompt user for input
    cout << "Enter username: ";
    getline(cin, username);

    cout << "Enter password: ";
    getline(cin, password);

    cout << "Enter role: ";
    getline(cin, role);

    // Create User using UserBuilder
    User user = UserBuilder().setUsername(username).setPassword(password).setRole(role).build();

    cout << "📌 Registering user in database..." << endl;
    // db.registerUser(user);
    cout << "login in " << endl;
    db.loginUser(username,password);

    return 0;
}
