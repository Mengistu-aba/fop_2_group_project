#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include <vector>
#include "database/database.h"

// Function prototypes
void loadRestaurantsFromDatabase(sqlite3* db, std::vector<Restaurant>& restaurants);

#endif // RESTAURANT_H