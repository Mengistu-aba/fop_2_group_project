#ifndef MENU_H
#define MENU_H

#include <vector>
#include "restaurant.h"
#include "user.h"
#include "database/database.h"


// Function prototypes
void clearInput();
int estimateDeliveryTime();
void displayFoodMenu(const Restaurant& restaurant);
void displayDrinkMenu(const Restaurant& restaurant);
void browseRestaurants(User* currentUser, const std::vector<Restaurant>& restaurants, sqlite3* db);
void viewOrderHistory(const User* currentUser);
void registerUser(std::vector<User>& users, sqlite3* db);
User* loginUser(std::vector<User>& users);
void browseAsGuest(const std::vector<Restaurant>& restaurants);

#endif // MENU_H