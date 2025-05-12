
#include <iostream>
#include "user.h"
#include "database/database.h"

using namespace std;

void registerUserInDatabase(sqlite3* db, const string& username, const string& password, const string& address) {
    // Implement SQL to insert new user into the Users table
    string sql = "INSERT INTO Users (username, password, address) VALUES ('" + username + "', '" + password + "', '" + address + "');";
    char *err_msg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, 0, &err_msg);

    if(result != 0){
        cout << "Error : " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}