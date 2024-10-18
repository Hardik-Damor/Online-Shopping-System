#include <iostream>
#include <mysql/mysql.h>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

class Product {
public:
    int id;
    string name;
    string category;
    double price;
    int stock;

    Product(int id, string name, string category, double price, int stock)
        : id(id), name(name), category(category), price(price), stock(stock) {}

    void display() const {
        cout << left << setw(5) << id 
             << setw(20) << name 
             << setw(15) << category 
             << setw(10) << price 
             << setw(10) << stock << endl;
    }
};

class User {
public:
    int id;
    string username;
    string password;

    User(int id, string username, string password)
        : id(id), username(username), password(password) {}
};

MYSQL* connectDatabase() {
    MYSQL* conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "Hardik@2511", "online_shopping", 3306, NULL, 0);
    if (conn) {
        cout << "Connected to MySQL successfully!\n";
    } else {
        cerr << "Database connection failed: " << mysql_error(conn) << endl;
        exit(1);
    }
    return conn;
}

void registerUser(MYSQL* conn) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if the username already exists
    string check_query = "SELECT COUNT(*) FROM users WHERE username='" + username + "';";
    if (mysql_query(conn, check_query.c_str())) {
        cerr << "User registration check failed: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* check_res = mysql_store_result(conn);
    MYSQL_ROW check_row = mysql_fetch_row(check_res);
    if (stoi(check_row[0]) > 0) {
        cout << "Username already exists! Please choose a different username.\n";
        mysql_free_result(check_res);
        return;
    }
    mysql_free_result(check_res);

    string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "');";
    if (mysql_query(conn, query.c_str())) {
        cerr << "User registration failed: " << mysql_error(conn) << endl;
    } else {
        cout << "User registered successfully!\n";
    }
}

User* loginUser(MYSQL* conn) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "';";
    if (mysql_query(conn, query.c_str())) {
        cerr << "Login failed: " << mysql_error(conn) << endl;
        return nullptr;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(res))) {
        return new User(stoi(row[0]), row[1], row[2]); // Returning user object
    } else {
        cout << "Invalid username or password!\n";
        return nullptr;
    }
}

void displayProducts(MYSQL* conn) {
    if (mysql_query(conn, "SELECT * FROM products")) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << left << setw(5) << "ID" 
         << setw(20) << "Name" 
         << setw(15) << "Category" 
         << setw(10) << "Price" 
         << setw(10) << "Stock" << endl;

    cout << string(60, '-') << endl;
    while ((row = mysql_fetch_row(res))) {
        Product p(stoi(row[0]), row[1], row[2], stod(row[3]), stoi(row[4]));
        p.display();
    }
    mysql_free_result(res);
}

void placeOrder(MYSQL* conn, User* user) {
    int productId, quantity;
    cout << "Enter product ID to order: ";
    cin >> productId;
    cout << "Enter quantity: ";
    cin >> quantity;

    // Check if product exists and has enough stock
    string check_query = "SELECT stock FROM products WHERE id = " + to_string(productId) + ";";
    if (mysql_query(conn, check_query.c_str())) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* check_res = mysql_store_result(conn);
    MYSQL_ROW check_row;
    if ((check_row = mysql_fetch_row(check_res))) {
        int available_stock = stoi(check_row[0]);
        if (quantity > available_stock) {
            cout << "Insufficient stock available! Only " << available_stock << " items in stock.\n";
            mysql_free_result(check_res);
            return;
        }
    } else {
        cout << "Product ID not found!\n";
        mysql_free_result(check_res);
        return;
    }
    mysql_free_result(check_res);

    // Place the order
    string query = "INSERT INTO orders (user_id, product_id, quantity) VALUES (" +
                   to_string(user->id) + ", " + to_string(productId) + ", " + to_string(quantity) + ");";
    if (mysql_query(conn, query.c_str())) {
        cerr << "Order placement failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Order placed successfully!\n";
        
        // Update stock
        string update_query = "UPDATE products SET stock = stock - " + to_string(quantity) + 
                              " WHERE id = " + to_string(productId) + ";";
        if (mysql_query(conn, update_query.c_str())) {
            cerr << "Stock update failed: " << mysql_error(conn) << endl;
        }
    }
}

int main() {
    MYSQL* conn = connectDatabase();

    int choice;
    User* currentUser = nullptr;

    do {
        cout << "\n1. Register\n2. Login\n3. View Products\n4. Place Order\n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser(conn);
                break;
            case 2:
                currentUser = loginUser(conn);
                break;
            case 3:
                displayProducts(conn);
                break;
            case 4:
                if (currentUser) {
                    placeOrder(conn, currentUser);
                } else {
                    cout << "Please login first!\n";
                }
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    mysql_close(conn);
    delete currentUser; // Free allocated memory for User object
    return 0;
}
