#include "../include/users.h"
// implement hash function
string User :: hashPassword(const string& text){
    string hash;
    char salt = 'Z';

    for(char c : text){
        hash += c ^ salt;
    }

    cout << "hash function " << endl;
    cout << "hashed password :  " << hash <<  endl;
    return hash;
}

// verify password
bool User::verifyPassword(const string& plainText, const string& hashedText) {
    return hashPassword(plainText) == hashedText;
}

// implement UserBuilder methods
UserBuilder&  UserBuilder :: setUsername(const string& uName){
    cout << "building user name" << endl;
    user.username = uName;
    return *this;
}
UserBuilder&  UserBuilder :: setPassword(const string& pass){
    cout << "hashing password" << endl;
    user.password = User::hashPassword(pass);
    return *this;
}
UserBuilder&  UserBuilder :: setRole(const string& r){
    cout << "setting role"<< endl;
    user.role = r;
    return *this;
}

// build method to return builder object
User UserBuilder::build(){
    cout << "user built" << endl;
    return user;
}

// implementing user  methods
void User::registerUser(){
    cout << "Registered user " << username << "with role : " << role << endl;
}

void User :: loginUser(){
    cout << "Logging in user : " << username << endl;
}

