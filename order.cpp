#include <iostream>
#include "order.h"
#include "database/database.h"

using namespace std;

void processOrder(User* currentUser, const Restaurant& selectedRest, const vector<pair<string, int>>& cart, sqlite3* db) {
    double total = 0.0;
    cout << "\n=== Your Order ===" << endl;

    for (const auto& item : cart) {
        double itemPrice = 0.0;
        for (const auto& food : selectedRest.foodMenu) {
            if (food.name == item.first) {
                itemPrice = food.price; // Get food price
                break;
            }
        }
        for (const auto& drink : selectedRest.drinkMenu) {
            if (drink.name == item.first) {
                itemPrice = drink.price; // Get drink price
                break;
            }
        }
        total += itemPrice * item.second; // Calculate total
        cout << item.first << " - " << item.second << "x" << endl; // Display item with quantity
    }

    cout << "Total: " << total << " birr" << endl; // Show total
    cout << "Delivery to: " << currentUser->address << endl; // Show delivery address

    // Payment Method
    cout << "Select payment method (1 for Cash, 2 for Card): ";
    int paymentMethod;
    cin >> paymentMethod; // Get payment method choice
    clearInput();

    if (paymentMethod == 1) {
        cout << "Cash payment confirmed." << endl; // Confirm cash payment
    } else if (paymentMethod == 2) {
        cout << "Card payment confirmed." << endl; // Confirm card payment
    } else {
        cout << "Invalid payment method!" << endl; // Invalid payment method handling
    }

    currentUser->orderHistory.emplace_back(selectedRest, cart); // Save order to history
    int estTime = estimateDeliveryTime(); // Estimate delivery time
    cout << "Estimated delivery time: " << estTime << " minutes." << endl; // Show estimated time
}