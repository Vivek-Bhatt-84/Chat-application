#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include "users.h"

using namespace std;

class UserDatabase{
    private:
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
    public:
        UserDatabase();  
        ~UserDatabase(); 
        void connectDb();
        void registerUser(const User& user);
        void loginUser(const string& uName , const string& pass);
};

