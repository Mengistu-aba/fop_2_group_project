#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "menu.h"
#include "user.h"
#include "restaurant.h"
#include "order.h"
#include "Database/database.h"

using namespace std;

int main() {
    cout << "\n------WELCOME TO AASTU FOOD DELIVERY SYSTEM--------" << endl;
    srand(time(0)); // Seed random for delivery time

    sqlite3* db = connectToDatabase("food_delivery.sqlite3");
    createTables(db); // Create tables in the database

    vector<User> users;              // Vector to store users
    vector<Restaurant> restaurants;
    loadDataFromDatabase(db, users, restaurants);
    // loadRestaurantsFromDatabase(db, users, restaurants); // Load restaurants from the database

    User* currentUser = nullptr;     // Pointer to the currently logged-in user

    // Main loop for the program
    while (true) {
        cout << "\n===== Online Food Ordering System =====" << endl;
        if (currentUser) {
            cout << "Logged in as: " << currentUser->username << endl;
            cout << "1. Browse Restaurants\n2. View Order History\n3. Logout\n4. Exit\n";
        } else {
            cout << "1. Register\n2. Login\n3. Browse Restaurants (Guest)\n4. Exit\n";
        }

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInput();

        if (currentUser) {
            switch (choice) {
                case 1:
                    browseRestaurants(currentUser, restaurants, db);
                    break;
                case 2:
                    viewOrderHistory(currentUser);
                    break;
                case 3:
                    currentUser = nullptr; // Clear current user
                    cout << "Logged out successfully." << endl;
                    break;
                case 4:
                    cout << "Thank you for using our service!" << endl;
                    closeDatabase(db);
                    return 0; // Exit the program
                default:
                    cout << "Invalid choice!" << endl;
            }
        } else {
            switch (choice) {
                case 1:
                    registerUser(users, db);
                    break;
                case 2:
                    currentUser = loginUser(users);
                    break;
                case 3:
                    browseAsGuest(restaurants);
                    break;
                case 4:
                    cout << "Thank you for visiting!" << endl;
                    closeDatabase(db);
                    return 0; // Exit the program
                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    }

    closeDatabase(db);
    return 0; // Program finished
}