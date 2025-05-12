#include <iostream>
#include <vector>
#include "database.h"

using namespace std;

// Function to connect to the SQLite database
sqlite3* connectToDatabase(const string& dbName) {
    sqlite3* db;
    int exit = sqlite3_open(dbName.c_str(), &db);
    if (exit) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

// Function to create tables if they do not exist
void createTables(sqlite3* db) {
    const char* createUsersTable = 
        "CREATE TABLE IF NOT EXISTS Users ("
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL UNIQUE, "
        "password TEXT NOT NULL, "
        "address TEXT NOT NULL);";

    const char* createRestaurantsTable = 
        "CREATE TABLE IF NOT EXISTS Restaurants ("
        "restaurant_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL UNIQUE);";

    const char* createFoodItemsTable = 
        "CREATE TABLE IF NOT EXISTS FoodItems ("
        "food_item_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "restaurant_id INTEGER, "
        "name TEXT NOT NULL, "
        "price REAL NOT NULL, "
        "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(restaurant_id));";

    const char* createDrinkItemsTable = 
        "CREATE TABLE IF NOT EXISTS DrinkItems ("
        "drink_item_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "restaurant_id INTEGER, "
        "name TEXT NOT NULL, "
        "price REAL NOT NULL, "
        "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(restaurant_id));";

    const char* createOrderHistoryTable = 
        "CREATE TABLE IF NOT EXISTS OrderHistory ("
        "order_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER, "
        "restaurant_id INTEGER, "
        "order_time DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "total_price REAL, "
        "FOREIGN KEY(user_id) REFERENCES Users(user_id), "
        "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(restaurant_id));";
    
    sqlite3_exec(db, createUsersTable, nullptr, 0, nullptr);
    sqlite3_exec(db, createRestaurantsTable, nullptr, 0, nullptr);
    sqlite3_exec(db, createFoodItemsTable, nullptr, 0, nullptr);
    sqlite3_exec(db, createDrinkItemsTable, nullptr, 0, nullptr);
    sqlite3_exec(db, createOrderHistoryTable, nullptr, 0, nullptr);
}

// Function to load data from vectors to the database
void loadDataToDatabase(sqlite3* db, const vector<User>& users, const vector<Restaurant>& restaurants) {
    for (const auto& user : users) {
        string sql = "INSERT OR IGNORE INTO Users (username, password, address) VALUES ('" + 
                     user.username + "', '" + user.password + "', '" + user.address + "');";
        sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);
    }

    for (const auto& restaurant : restaurants) {
        string sql = "INSERT OR IGNORE INTO Restaurants (name) VALUES ('" + restaurant.name + "');";
        sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);

        // Load food items for the restaurant
        for (const auto& food : restaurant.foodMenu) {
            string foodSql = "INSERT OR IGNORE INTO FoodItems (restaurant_id, name, price) VALUES "
                             "((SELECT restaurant_id FROM Restaurants WHERE name = '" + restaurant.name + "'), '" + 
                             food.name + "', " + to_string(food.price) + ");";
            sqlite3_exec(db, foodSql.c_str(), nullptr, 0, nullptr);
        }

        // Load drink items for the restaurant
        for (const auto& drink : restaurant.drinkMenu) {
            string drinkSql = "INSERT OR IGNORE INTO DrinkItems (restaurant_id, name, price) VALUES "
                              "((SELECT restaurant_id FROM Restaurants WHERE name = '" + restaurant.name + "'), '" + 
                              drink.name + "', " + to_string(drink.price) + ");";
            sqlite3_exec(db, drinkSql.c_str(), nullptr, 0, nullptr);
        }
    }
}


// Static callback for loading users
static int userCallback(void* data, int argc, char** argv, char** azColName) {
    std::vector<User>* users = static_cast<std::vector<User>*>(data);
    User user;
    user.username = argv[0] ? argv[0] : "";
    user.password = argv[1] ? argv[1] : "";
    users->push_back(user);
    return 0;
}

// Static callback for loading restaurants
static int restaurantCallback(void* data, int argc, char** argv, char** azColName) {
    std::vector<Restaurant>* restaurants = static_cast<std::vector<Restaurant>*>(data);
    Restaurant restaurant;
    restaurant.name = argv[0] ? argv[0] : "";
    restaurants->push_back(restaurant);
    return 0;
}

// Static callback for loading food items
static int foodItemCallback(void* data, int argc, char** argv, char** azColName) {
    Restaurant* restaurant = static_cast<Restaurant*>(data);
    FoodItem food;
    food.name = argv[0] ? argv[0] : "";
    food.price = argv[1] ? atof(argv[1]) : 0.0;
    restaurant->foodMenu.push_back(food);
    return 0;
}

// Static callback for loading drink items
static int drinkItemCallback(void* data, int argc, char** argv, char** azColName) {
    Restaurant* restaurant = static_cast<Restaurant*>(data);
    FoodItem drink;
    drink.name = argv[0] ? argv[0] : "";
    drink.price = argv[1] ? atof(argv[1]) : 0.0;
    restaurant->drinkMenu.push_back(drink);
    return 0;
}

void loadDataFromDatabase(sqlite3* db, std::vector<User>& users, std::vector<Restaurant>& restaurants) {
    const char* selectUsers = "SELECT username, password FROM Users;";
    if (sqlite3_exec(db, selectUsers, userCallback, &users, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to load users: " << sqlite3_errmsg(db) << std::endl;
    }

    const char* selectRestaurants = "SELECT name FROM Restaurants;";
    if (sqlite3_exec(db, selectRestaurants, restaurantCallback, &restaurants, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to load restaurants: " << sqlite3_errmsg(db) << std::endl;
    }

    // For each restaurant, load food and drink items
    for (Restaurant& restaurant : restaurants) {
        std::string foodQuery = "SELECT name, price FROM FoodItems WHERE restaurant_id = (SELECT restaurant_id FROM Restaurants WHERE name = '" + restaurant.name + "');";
        if (sqlite3_exec(db, foodQuery.c_str(), foodItemCallback, &restaurant, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to load food items for " << restaurant.name << ": " << sqlite3_errmsg(db) << std::endl;
        }

        std::string drinkQuery = "SELECT name, price FROM DrinkItems WHERE restaurant_id = (SELECT restaurant_id FROM Restaurants WHERE name = '" + restaurant.name + "');";
        if (sqlite3_exec(db, drinkQuery.c_str(), drinkItemCallback, &restaurant, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to load drink items for " << restaurant.name << ": " << sqlite3_errmsg(db) << std::endl;
        }
    }
}

// Helper: insert an order into OrderHistory using subqueries to find user_id and restaurant_id
void insertOrder(sqlite3* db, const string& username, const string& restaurantName, double totalPrice) {
    string sql =
        "INSERT INTO OrderHistory (user_id, restaurant_id, total_price) "
        "VALUES ("
        "(SELECT user_id FROM Users WHERE username = '" + username + "'), "
        "(SELECT restaurant_id FROM Restaurants WHERE name = '" + restaurantName + "'), "
        + to_string(totalPrice) + ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Failed to insert order: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Main: loop through users and orders, calculate price, and insert into DB
void loadOrderHistory(sqlite3* db, const vector<User>& users) {
    for (const auto& user : users) {
        for (const auto& order : user.orderHistory) {
            const Restaurant& restaurant = order.first;
            const vector<pair<string, int>>& items = order.second;

            double totalPrice = 0.0;
            for (const auto& item : items) {
                const string& itemName = item.first;
                int quantity = item.second;
                bool found = false;

                // Check food menu
                for (const auto& food : restaurant.foodMenu) {
                    if (food.name == itemName) {
                        totalPrice += food.price * quantity;
                        found = true;
                        break;
                    }
                }

                // If not in food menu, check drink menu
                if (!found) {
                    for (const auto& drink : restaurant.drinkMenu) {
                        if (drink.name == itemName) {
                            totalPrice += drink.price * quantity;
                            break;
                        }
                    }
                }
            }

            insertOrder(db, user.username, restaurant.name, totalPrice);
        }
    }
}


// Function to close the database connection
void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
}