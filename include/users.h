#pragma once
#include  <iostream>
using namespace std;

// user class for building user 
class User{
    private :
        string username;
        string password;
        string role;

        // private constructor so that only userbuilder can create instance 
        User() = default;

        // grant userbuilder access to private members
        friend class UserBuilder;
    public :
        void registerUser();
        void loginUser();

        string getUsername()const{return username;}
        string getPassword()const {return password;}
        string getRole()const {return role;}

        //  hash function
        static string hashPassword(const string& text);
        static bool verifyPassword(const string& plainText, const string& hashedText);
};

// user builder 

class UserBuilder{
    private:
        User user;
    public:
        UserBuilder& setUsername(const string& uName);
        UserBuilder& setPassword(const string& password);
        UserBuilder& setRole(const string& role);
        User build();

    };


