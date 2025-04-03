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
        bool loginUser(const string& uName , const string& pass);
        void sendMessage(const string& sender, const string& receiver, const string& message);
        vector<string> fetchMessages(const string& username);
        void storeMessage(const string& recipient, const string& sender, const string& message);
        vector<string> getOfflineMessages(const string& recipient);
        void deleteMessages(const string& recipient);
};



