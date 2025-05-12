#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <utility>
#include "sqlite3.h"

// Forward declarations
struct FoodItem;
struct Restaurant;
struct User;

// Structure to represent a food item
struct FoodItem {
    std::string name;
    double price;
};

// Structure to represent a restaurant
struct Restaurant {
    std::string name;
    std::vector<FoodItem> foodMenu;
    std::vector<FoodItem> drinkMenu;
};

// Structure to represent a user
struct User {
    std::string username;
    std::string password;
    std::string address;
    std::vector<std::pair<Restaurant, std::vector<std::pair<std::string, int>>>> orderHistory;
};




// Function prototypes
sqlite3* connectToDatabase(const std::string& dbName);
void createTables(sqlite3* db);
void loadDataToDatabase(sqlite3* db, const std::vector<User>& users, const std::vector<Restaurant>& restaurants);
void loadDataFromDatabase(sqlite3* db, std::vector<User>& users, std::vector<Restaurant>& restaurants);
void insertOrder(sqlite3* db, int userId, int restaurantId, double totalPrice);
void loadOrderHistory(sqlite3* db, const std::vector<User>& users);
void closeDatabase(sqlite3* db);

#endif // DATABASE_H