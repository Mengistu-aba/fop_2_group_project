#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <utility>
#include "user.h"
#include "restaurant.h"
#include "database/database.h"
#include "menu.h"

// Function prototypes
void processOrder(User* currentUser, const Restaurant& selectedRest, const std::vector<std::pair<std::string, int>>& cart, sqlite3* db);

#endif // ORDER_H