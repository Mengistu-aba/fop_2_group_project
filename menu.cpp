#include <iostream>
#include <vector>
#include <limits>

#include "menu.h"
#include "order.h"
#include "database/database.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int estimateDeliveryTime() {
    return 20 + rand() % 41; // Random time between 20 and 60
}

void displayFoodMenu(const Restaurant& restaurant) {
    cout << "\n=== " << restaurant.name << " Food Menu ===" << endl;
    for (size_t i = 0; i < restaurant.foodMenu.size(); ++i) {
        cout << i + 1 << ". " << restaurant.foodMenu[i].name << " - " << restaurant.foodMenu[i].price << " birr" << endl;
    }
}

void displayDrinkMenu(const Restaurant& restaurant) {
    cout << "\n=== " << restaurant.name << " Drink Menu ===" << endl;
    for (size_t i = 0; i < restaurant.drinkMenu.size(); ++i) {
        cout << i + 1 << ". " << restaurant.drinkMenu[i].name << " - " << restaurant.drinkMenu[i].price << " birr" << endl;
    }
}

void browseRestaurants(User* currentUser, const vector<Restaurant>& restaurants, sqlite3* db) {
    cout << "\nAvailable Restaurants:" << endl;
    for (size_t i = 0; i < restaurants.size(); ++i) {
        cout << i + 1 << ". " << restaurants[i].name << endl; // List all restaurants
    }

    int restChoice;
    cout << "Select a restaurant (0 to cancel): ";
    cin >> restChoice; // Get restaurant choice
    clearInput();

    if (restChoice > 0 && restChoice <= static_cast<int>(restaurants.size())) {
        const Restaurant& selectedRest = restaurants.at(restChoice - 1);
        char foodOrDrink;
        cout << "Do you want to order food (f) or drinks (d)? ";
        cin >> foodOrDrink; // Get choice for food or drink
        clearInput();

        vector<pair<string, int>> cart; // Vector to store ordered items

        if (foodOrDrink == 'f') {
            displayFoodMenu(selectedRest);
            while (true) {
                cout << "Enter the roll number of the item you want to order (0 to finish): ";
                int itemChoice;
                cin >> itemChoice; // Get food item choice
                clearInput();

                if (itemChoice == 0) break;

                cout << "Enter the quantity you want: ";
                int quantity;
                cin >> quantity; // Get quantity
                clearInput();

                if (itemChoice > 0 && itemChoice <= static_cast<int>(selectedRest.foodMenu.size())) {
                    string itemName = selectedRest.foodMenu[itemChoice - 1].name; // Get item name
                    cart.emplace_back(itemName, quantity); // Add to cart
                    cout << "Added " << quantity << " x " << itemName << " to cart." << endl;
                } else {
                    cout << "Invalid choice!" << endl;
                }
            }
        } else if (foodOrDrink == 'd') {
            displayDrinkMenu(selectedRest);
            while (true) {
                cout << "Enter the roll number of the drink you want to order (0 to finish): ";
                int itemChoice;
                cin >> itemChoice; // Get drink item choice
                clearInput();

                if (itemChoice == 0) break;

                cout << "Enter the quantity you want: ";
                int quantity;
                cin >> quantity; // Get quantity
                clearInput();

                if (itemChoice > 0 && itemChoice <= static_cast<int>(selectedRest.drinkMenu.size())) {
                    string itemName = selectedRest.drinkMenu[itemChoice - 1].name; // Get item name
                    cart.emplace_back(itemName, quantity); // Add to cart
                    cout << "Added " << quantity << " x " << itemName << " to cart." << endl;
                } else {
                    cout << "Invalid choice!" << endl;
                }
            }
        } else {
            cout << "Invalid choice!" << endl;
        }

        if (!cart.empty()) {
            processOrder(currentUser, selectedRest, cart, db);
        }
    }
}

void viewOrderHistory(const User* currentUser) {
    cout << "\n=== Order History ===" << endl;
    for (const auto& order : currentUser->orderHistory) {
        cout << "Restaurant: " << order.first.name << endl;
        for (const auto& item : order.second) {
            cout << "  - " << item.first << " (" << item.second << "x)" << endl;
        }
        cout << "---------------------" << endl;
    }
}

void registerUser(vector<User>& users, sqlite3* db) {
    string username, password, address;
    cout << "Enter username: ";
    getline(cin, username); // Get username
    cout << "Enter password (at least 6 characters): ";
    getline(cin, password); // Get password

    if (password.length() < 6) {
        cout << "Password must be at least 6 characters long!" << endl; // Password length validation
        return;
    }

    cout << "Enter delivery address: ";
    getline(cin, address); // Get delivery address

    users.push_back({username, password, address, {}}); // Create new user
    registerUserInDatabase(db, username, password, address); // Save user to the database
    cout << "Registration successful!" << endl;
}

User* loginUser(vector<User>& users) {
    if (users.empty()) {
        cout << "No users registered yet!" << endl; // Check if no users are registered
        return nullptr;
    }

    string username, password;
    cout << "Enter username: ";
    getline(cin, username); // Get username
    cout << "Enter password: ";
    getline(cin, password); // Get password

    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            cout << "Login successful!" << endl; // Confirm login
            return &user; // Return pointer to the logged-in user
        }
    }

    cout << "Invalid username or password!" << endl; // Invalid login handling
    return nullptr;
}

void browseAsGuest(const vector<Restaurant>& restaurants) {
    cout << "\nAvailable Restaurants:" << endl;
    for (size_t i = 0; i < restaurants.size(); ++i) {
        cout << i + 1 << ". " << restaurants[i].name << endl; // List all restaurants
    }

    int restChoice;
    cout << "Select a restaurant to view menu (0 to cancel): ";
    cin >> restChoice; // Get restaurant choice
    clearInput();

    if (restChoice > 0 && restChoice <= static_cast<int>(restaurants.size())) {
        displayFoodMenu(restaurants[restChoice - 1]); // Display food menu
        displayDrinkMenu(restaurants[restChoice - 1]); // Display drink menu
        cout << "\nPlease login or register to place an order." << endl; // Prompt for login
    }
}