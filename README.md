# **Online Shopping System**

## **Project Overview**
The Online Shopping System is a console-based application developed in C++ that allows users to register, log in, browse products, and place orders. The system is designed to simulate a simple e-commerce platform, providing essential features for user management and product handling. It utilizes MySQL as the database management system for storing user credentials, product information, and order details.

## **Features**
- **User Registration**: Allows new users to create an account by providing a unique username and password. The system checks for existing usernames to ensure uniqueness.
  
- **User Login**: Registered users can log in using their credentials. The system verifies the username and password against the database.
  
- **Product Display**: Users can view an extensive catalog of products, including details such as product ID, name, category, price, and available stock.
  
- **Place Orders**: Users can place orders by specifying the product ID and quantity. The system checks for product existence and stock availability before confirming the order.
  
- **Order Management**: Orders are recorded in the database and associated with the respective user. The system updates product stock automatically upon order placement.
  
- **Error Handling**: The application provides feedback for various scenarios, such as incorrect login attempts, insufficient stock, and non-existent product IDs.
  
- **User-Friendly Interface**: The console interface offers clear navigation options, enhancing the user experience.

## **Technologies Used**
- **Programming Language**: C++
- **Database Management System**: MySQL
- **Libraries**: MySQL Connector/C++ for database interactions
- **Development Environment**: Visual Studio Code (or any preferred IDE)
- **Operating System**: macOS

## **Installation and Setup**
To run this project, ensure you have the following prerequisites:
1. **MySQL Database**: Set up a MySQL server and create the necessary tables.
2. **C++ Compiler**: Install a C++ compiler that supports C++11 or later.
3. **MySQL Connector/C++**: Install the MySQL Connector/C++ library for database connectivity.

Clone the repository and compile the code. Update the database connection details in the code as needed.
