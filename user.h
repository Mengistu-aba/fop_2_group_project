#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <utility>
#include "restaurant.h"
#include "database/database.h"

// Function prototypes
void registerUserInDatabase(sqlite3* db, const std::string& username, const std::string& password, const std::string& address);

#endif // USER_H