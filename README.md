# AASTU Food Delivery System

## Group Members

| Member Name           | ID Number  |
| --------------------- | ---------- |
| Addis Shiferaw        | ETS0099/16 |
| Addisalem Mengistu    | ETS0101/16 |
| Amir Yimam            | ETS0169/16 |
| Bemigbar Yehuwalawork | ETS0243/16 |
| Betelhem Kassaye      | ETS0265/16 |
| Lidia Aliso           | ETS0830/15 |

## Project Description

The AASTU Food Delivery System is a C++ console application that allows users to order food and drinks from various restaurants. The system supports user registration, login, browsing restaurants, placing orders, and viewing order history. It uses SQLite for data persistence.

## Features

- User registration and login
- Browse restaurants as a guest or logged-in user
- View food and drink menus for each restaurant
- Place orders with multiple items
- Choose payment method (Cash or Card)
- View order history
- Estimated delivery time calculation

## Requirements

- C++ compiler (g++ recommended)
- SQLite3 library
- Windows environment (for the current configuration)

## How to Run

### Using Visual Studio Code

1. Clone the repository
2. Open the project folder in VS Code
3. Make sure you have the C/C++ extension installed
4. Use the built-in tasks to build the project:
   - Press `Ctrl+Shift+B` to build
   - Or run the task manually via Terminal > Run Build Task
5. Run the compiled executable:
   ```
   ./MyFoodDelivery.exe
   ```

### Using Command Line

1. Navigate to the project directory
2. Compile the project:
   ```
   g++ -o MyFoodDelivery.exe code/*.cpp database/*.cpp -I database
   ```
3. Run the executable:
   ```
   ./MyFoodDelivery.exe
   ```

## Project Structure

- `main.cpp` - Entry point of the application
- `menu.h/cpp` - Functions for displaying menus and user interactions
- `user.h/cpp` - User-related functionality
- `restaurant.h/cpp` - Restaurant-related functionality
- `order.h/cpp` - Order processing functionality
- `database/` - Database-related files
  - `database.h/cpp` - Database connection and operations
  - `sqlite3.h` - SQLite library header

## Database Schema

The application uses SQLite with the following tables:

- Users - Stores user information
- Restaurants - Stores restaurant information
- FoodItems - Stores food menu items
- DrinkItems - Stores drink menu items
- OrderHistory - Stores order history
