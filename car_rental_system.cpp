#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/statement.h> 
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream> 
#include <ctime>
#include <sstream>
#include <iomanip>
#include <mysql_connection.h> 
#include <mysql_driver.h>

using namespace std;

class users {
public:
    string id;
    string password;
    string name;
    sql::Connection* con;

    bool login(int identity) {
        sql::Statement* stmt = con->createStatement();
        cout << "\n ***Login Page***";
        cout << "\n Enter your user id : ";
        cin >> id;
        cout << "\n Enter your password : ";
        cin >> password;
        if (identity == 1) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE customer_id = '" + id + "' AND password = '" + password + "'");  // Query to fetch id and password from customer table
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n Logged in!!";
                return true;
            }
            else {
                cout << "\n Wrong user id or password";
                return false;
            }
            delete res;
        }
        else {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE employee_id = '" + id + "' AND password = '" + password + "'");    // // Query to fetch id and password from employee table
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n Logged in!!";
                return true;
            }
            else {
                cout << "\n Wrong user id or password";
                return false;
            }
            delete res;
        }
        delete stmt;
    }

    bool login() {          // For manager
        sql::Statement* stmt = con->createStatement();
        cout << "\n ***Login Page***";
        cout << "\n Enter your user id : ";
        cin >> id;
        cout << "\n Enter your user password : ";
        cin >> password;
        if (id == "manager" && password == "1234") {
            cout << "\n Logged in!!";
            return true;
        }
        else {
            cout << "\n Wrong user id or password";
            return false;
        }
        delete stmt;
    }
};

class manager : public users {
public:
    manager(sql::Connection* con) {
        this->con = con;
    }

    void viewCarData() {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars");    // Query to select the car data
        cout << "\n ***All Cars***" << endl;
        while (res->next()) {
            cout << "\n Car ID : " << res->getString("car_id")
                << "\n Company : " << res->getString("company")
                << "\n model : " << res->getString("model")
                << "\n Price : " << res->getInt("price")
                << "\n Condition : " << res->getString("car_condition")
                << "\n Rented to : " << res->getString("user_id");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void viewEmployeeData() {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees");    // Query to select the employee data
        cout << "\n ***All Employees***" << endl;
        while (res->next()) {
            cout << "\n Name : " << res->getString("name")
                << "\n UserID : " << res->getString("employee_id")
                << "\n Fine : " << res->getInt("fine")
                << "\n Employee Record : " << res->getString("record")
                << "\n Rented Cars : " << res->getInt("rented_cars");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void viewCustomerData() {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM customers");    // Query to select the customer data
        cout << "\n ***All Customers***" << endl;
        while (res->next()) {
            cout << "\n Name : " << res->getString("name")
                << "\n UserID : " << res->getString("customer_id")
                << "\n Fine : " << res->getInt("fine")
                << "\n Employee Record : " << res->getString("record")
                << "\n Rented Cars : " << res->getInt("rented_cars");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void addCar() {
        sql::Statement* stmt = con->createStatement();
        string model, company, car_condition;
        int price;
        cout << "\n ***Adding a Car***";
        cout << "\n What is the company of the car : ";
        cin >> company;
        cout << "\n Enter the car model : ";
        cin >> model;
        cout << "\n Enter the price :";
        cin >> price;
        cout << "\n Enter the condition (new/secondhand) : ";
        cin >> car_condition;
        cout << "\n Car has been added!!";
        stmt->execute("INSERT INTO cars (company, model, price, car_condition) VALUES ('" + company + "', '" + model + "', " + to_string(price) + ", '" + car_condition + "')");  // Query to add car data in the cars table
        delete stmt;
    }

    void deleteCarData() {
        sql::Statement* stmt = con->createStatement();
        int car_id;
        cout << "\n ***Deleting a Car***";
        cout << "\n Enter the ID of the car you want to delete :";
        cin >> car_id;
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE car_id = " + to_string(car_id) + ")");    // Query to select the car id from car data
        int count = res->rowsCount();
        if (count > 0) {
            cout << "\n Car has been deleted!!";
            stmt->execute("DELETE FROM cars WHERE car_id = " + to_string(car_id) + ")");  // Query to delete car data and reduce no from employee of customer
        }
        else {
            cout << "\n No car of this ID found";
        }
        delete res;
        delete stmt;
    }

    void updateCarData() {
        sql::Statement* stmt = con->createStatement();
        string car_condition;
        int car_id, price;
        cout << "\n ***Updating a Car***";
        cout << "\n Enter the car ID : ";
        cin >> car_id;
        cout << "\n Enter the new price :";
        cin >> price;
        cout << "\n Enter the condition (new,second hand) : ";
        cin >> car_condition;
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE car_id = " + car_id);    // Query to select the car id from car data
        int count = res->rowsCount();
        if (count > 0) {
            cout << "\n Car has been updated!!";
            stmt->execute("UPDATE cars SET price = " + to_string(price) + ", car_condition = '" + car_condition + "' WHERE car_id = " + to_string(car_id) + ")");  // Query to edit price and condition of the car
        }
        else {
            cout << "\n No car of this number found";
        }
        delete res;
        delete stmt;
    }

    void addUser() {
        sql::Statement* stmt = con->createStatement();
        string user_name, user_id;
        int identity;
        cout << "\n ***Adding a User***";
        cout << "\n What is the name of the user : ";
        cin >> user_name;
        cout << "\n Enter the user id : ";
        cin >> user_id;
        cout << "\n Which type of user you are adding??\n 1 - (customer)\n 2 - (employee)";
        cin >> identity;
        if (identity == 1) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE customer_id = '" + user_id + "'");    // Query to (SELECT) the user to check existing customer
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n There is already a customer of this user id";
            }
            else {
                cout << "\n Customer has been added!!";
                stmt->execute("INSERT INTO customers (customer_id, name, password, fine, record, rented_cars) VALUES ('" + user_id + "', '" + user_name + "', 'pass', 0, TRUE, 0)");  // Query to add customer with 0 fine, good record, 0 rented cars, 1234 password
            }
            delete res;
        }
        else if (identity == 2) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE employee_id = '" + user_id + "'");    // Query to (SELECT) the user to check existing employee
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n There is already a employee of this user id";
            }
            else {
                cout << "\n Employee has been added!!";
                stmt->execute("INSERT INTO employees (employee_id, name, password, fine, record, rented_cars) VALUES ('" + user_id + "', '" + user_name + "', 'pass', 0, TRUE, 0)");  // Query to add employee with 0 fine, good record, 0 rented cars
            }
            delete res;
        }
        else {
            cout << "\n Enter a valid type for user";
        }
        delete stmt;
    }

    void deleteUserData() {
        sql::Statement* stmt = con->createStatement();
        string user_id;
        int identity;
        cout << "\n ***Deleting a User***";
        cout << "\n Enter the user id of the user you want to delete :";
        cin >> user_id;
        cout << "\n Which type of user you are deleting??\n 1 - (customer)\n 2 - (employee)";
        cin >> identity;
        if (identity == 1) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE customer_id = '" + user_id + "'");    // Query to (SELECT) the user to check existing customer
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n Customer has been deleted!!";
                stmt->execute("DELETE FROM customers WHERE customer_id = '" + user_id + "'");  // Query to delete the customer delete from car data also
            }
            else {
                cout << "\n There is no customer of this user id";
            }
            delete res;
        }
        else if (identity == 2) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE employee_id = '" + user_id + "'");    // Query to (SELECT) the user to check existing employee
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n Employee has been deleted!!";
                stmt->execute("DELETE FROM employees WHERE employee_id = '" + user_id + "'");  // Query to delete the employee delete from car data also
            }
            else {
                cout << "\n There no employee of this user id";
            }
            delete res;
        }
        else {
            cout << "\n Enter a valid type for user";
        }
        delete stmt;
    }

    void updateUserData() {
        sql::Statement* stmt = con->createStatement();
        string user_id;
        bool record;
        int fine;
        int identity;
        cout << "\n ***Updating a User***";
        cout << "\n Enter the user id : ";
        cin >> user_id;
        cout << "\n Which type of user you are updating??\n 1 - (customer)\n 2 - (employee)";
        cin >> identity;
        cout << "\n Enter the new fine : ";
        cin >> fine;
        if (fine > 50000)  record = false;
        else record = true;
        if (identity == 1) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE customer_id = '" + user_id + "'");    // Query to (SELECT) the user to check existing customer
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n Customer has been updated!!";
                stmt->execute("UPDATE customers SET fine = " + to_string(fine) + ", record = " + to_string(record) + " WHERE customer_id = '" + user_id + "'"); 
            }
            else {
                cout << "\n There is no customer of this user id";
            }
            delete res;
        }
        else if (identity == 2) {
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE employee_id = '" + user_id + "'");    // Query to (SELECT) the user to check existing employee
            int count = res->rowsCount();
            if (count > 0) {
                cout << "\n Employee has been updated!!";
                stmt->execute("UPDATE employees SET  = fine " + to_string(fine) + ", record = " + to_string(record) + " WHERE employee_id = '" + user_id + "'");  // Query to update employee fine and record
            }
            else {
                cout << "\n There is no employee of this user id";
            }
            delete res;
        }
        else {
            cout << "\n Enter a valid type for user";
        }
        delete stmt;
    }
};

class employee : public users {
public:
    employee(sql::Connection* con) {
        this->con = con;
    }

    void viewAvailableCars() {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE user_id IS NULL");    // Query to select available car data
        cout << "\n ***Available Cars***" << endl;
        while (res->next()) {
            cout << "\n Car ID : " << res->getInt("car_id")
                << "\n Company : " << res->getString("company")
                << "\n Model : " << res->getString("model")
                << "\n Price : " << res->getInt("price")
                << "\n Condition : " << res->getString("car_condition");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void viewRentedCars(string user_id) {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE user_id = '" + user_id + "'");    // Query to select rented cars data from car table with userid
        cout << "\n ***Rented Cars***" << endl;
        while (res->next()) {
            cout << "\n Car ID : " << res->getInt("id")
                << "\n Company : " << res->getString("company")
                << "\n Model : " << res->getString("model")
                << "\n Rented Date : " << res->getString("rented_date");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void rentRequest(string user_id) {
        sql::Statement* stmt = con->createStatement();
        int car_id;
        cout << "\n ***Renting a car***";
        cout << "\n Enter the number of the car you want to rent";
        cin >> car_id;
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE car_id = " + to_string(car_id)+ "");    // Query to (SELECT) the car with carNo and empty userid string
        int count = res->rowsCount();
        if (count > 0) {
            cout << "\n The car has been rented!!";
            stmt->execute("");  // Query to rent a car and increase rented cars in employee
        }
        else {
            cout << "\n The car id you provided is not available. Please rent another car";
        }
        delete stmt;
        delete res;
    }

    void clearDues(string user_id) {
        sql::Statement* stmt = con->createStatement();
        int car_id;
        string entered_date;
        cout << "\n ***Clearing dues***";
        cout << "\n Enter the car ID : ";
        cin >> car_id;
        cout << "Enter the entered_date (YYYY-MM-DD): ";
        cin>>entered_date;
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE DATEDIFF('" + entered_date + "',due_date)>0");
        if ((res->rowsCount()) > 0) {
            stmt->executeQuery("U * FROM cars WHERE DATEDIFF('" + entered_date + "',due_date)>0");
        }
        else {

        }
        cout << "\n Dues cleared!!";
        delete stmt;
    }
};

class customer : public users {
public:
    customer(sql::Connection* con) {
        this->con = con;
    }

    void registerCustomer() {
        sql::Statement* stmt = con->createStatement();
        string name, id, password;
        cout << "\n ***Register Page***";
        cout << "\n Enter your name : ";
        cin >> name;
        cout << "\n Make a new id : ";
        cin >> id;
        cout << "\n Enter a password : ";
        cin >> password;
        sql::ResultSet* res1 = stmt->executeQuery("SELECT * FROM customers WHERE customer_id = '"+ id +"'");    // Query to (SELECT) the user to check existing customer
        sql::ResultSet* res2 = stmt->executeQuery("SELECT * FROM employees WHERE employee_id = '"+ id + "'");    // Query to (SELECT) the user to check existing employee
        int count1 = res1->rowsCount(), count2 = res2->rowsCount();
        if (count1 > 0 || count2 > 0) {
            cout << "\n There is already a user of this user id";
        }
        else {
            cout << "\n You have been registered!!";
            stmt->execute("INSERT INTO customers(customer_id, name, password, fine, record, rented_cars) VALUES('" + id + "', '" + name + "', '" + password + "', 0, TRUE, 0)");  // Query to register user with 0 fine, good record, 0 rented cars
        }
        delete res1;
        delete res2;
        delete stmt;
    }

    void viewAvailableCars() {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE user_id is NULL");    // Query to select available car data
        cout << "\n ***Available Cars***" << endl;
        while (res->next()) {
            cout << "\n Car ID : " << res->getInt("car_id")
                << "\n Company : " << res->getString("company")
                << "\n Model : " << res->getString("model")
                << "\n Price : " << res->getInt("price")
                << "\n Condition : " << res->getString("car_condition");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void viewRentedCars(string user_id) {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE user_id = '" + user_id + "'");    // Query to select rented cars data from car table with userid
        cout << "\n ***Rented Cars***" << endl;
        while (res->next()) {
            cout << "\n Car ID : " << res->getInt("id")
                << "\n Company : " << res->getString("company")
                << "\n Model : " << res->getString("model")
                << "\n Rented Date : " << res->getString("rented_date");
            cout << "\n";
            for (int i = 0; i < 25; i++)   cout << "-";
        }
        delete stmt;
        delete res;
    }

    void rentRequest(string user_id) {
        sql::Statement* stmt = con->createStatement();
        string carNo;
        cout << "\n ***Renting a car***";
        cout << "\n Enter the number of the car you want to rent";
        cin >> carNo;
        sql::ResultSet* res = stmt->executeQuery("");    // Query to (SELECT) the car with carNo and empty userid string
        int count = res->rowsCount();
        // if (count > 0) {
        //     cout << "\n The car has been rented!!";
        //     stmt->execute("");  // Query to rent a car and increase rented cars in customer
        // }
        // else {
        //     cout << "\n The car number you provided is not available. Please rent another car";
        // }
        delete res;
        delete stmt;
    }

    void clearDues(string user_id) {
        sql::Statement* stmt = con->createStatement();
        cout << "\n ***Clearing dues***";
        cout << "\n Dues cleared!!";
        delete stmt;
    }
};

void managerPage(sql::Connection* con) {
    manager mag(con);

    if (mag.login()) {
        char task = '0';
        while (task != 'x' && task != 'X') {
            cout << "\n ***Manager***";
            cout << "\n What do you want to do??";
            cout << "\n a - (View information of all cars)";
            cout << "\n b - (View information of customers)";
            cout << "\n c - (View information of employees)";
            cout << "\n d - (Add a new car)";
            cout << "\n e - (Delete a car)";
            cout << "\n f - (Update information of a car)";
            cout << "\n g - (Add a new user)";
            cout << "\n h - (Delete existing user)";
            cout << "\n i - (Update information of a user)";
            cout << "\n x - (Exit)\n";
            cin >> task;
            if (task == 'a' || task == 'A') {
                mag.viewCarData();
            }
            else if (task == 'b' || task == 'B') {
                mag.viewCustomerData();
            }
            else if (task == 'c' || task == 'C') {
                mag.viewEmployeeData();
            }
            else if (task == 'd' || task == 'D') {
                mag.addCar();
            }
            else if (task == 'e' || task == 'E') {
                mag.deleteCarData();
            }
            else if (task == 'f' || task == 'F') {
                mag.updateCarData();
            }
            else if (task == 'g' || task == 'G') {
                mag.addUser();
            }
            else if (task == 'h' || task == 'H') {
                mag.deleteUserData();
            }
            else if (task == 'i' || task == 'I') {
                mag.updateUserData();
            }
            else if (task != 'x' && task != 'X') {
                cout << "\n Enter a valid key";
            }
            cout << "\n";
            for (int i = 0; i < 30; i++)   cout << "*";
            cout << "\n";
        }
    }
}

void employeePage(sql::Connection* con) {
    employee emp(con);

    char ch = '0';
    while (ch != 'x' && ch != 'X') {
        cout << "\n ***Employee***";
        cout << "\n So what do you want to do??";
        cout << "\n l - (login)";
        cout << "\n x - (Exit)\n";
        cin >> ch;
        if (ch == 'l' || ch == 'L') {
            char task = '0';
            while (task != 'x' && task != 'X') {
                if (emp.login(2)) {
                    cout << "\n ***Welcome***";
                    cout << "\n What do you want to do today??";
                    cout << "\n a - (View available cars)";
                    cout << "\n b - (Your Rented Cars)";
                    cout << "\n c - (Rent a car)";
                    cout << "\n d - (Clear Dues)";
                    cout << "\n x - (Exit)\n";
                    cin >> task;
                    if (task == 'a' || task == 'A') {
                        emp.viewAvailableCars();
                    }
                    else if (task == 'b' || task == 'B') {
                        emp.viewRentedCars(emp.id);
                    }
                    else if (task == 'c' || task == 'C') {
                        emp.rentRequest(emp.id);
                    }
                    else if (task == 'd' || task == 'D') {
                        emp.clearDues(emp.id);
                    }
                    else if (task != 'x' && task != 'X') {
                        cout << "\n Please enter a valid key";
                    }
                }
            }
            cout << "\n";
            for (int i = 0; i < 30; i++)   cout << "*";
            cout << "\n";
        }
        else if (ch != 'x' && ch != 'X') {
            cout << "\n Please enter a valid key";
        }
        cout << "\n";
        for (int i = 0; i < 30; i++)   cout << "*";
        cout << "\n";
    }
}

void customerPage(sql::Connection* con) {
    customer cust(con);

    char ch = '0';
    while (ch != 'x' && ch != 'X') {
        cout << "\n ***Customer***";
        cout << "\n So what do you want to do??";
        cout << "\n l - (login)";
        cout << "\n r - (Register)";
        cout << "\n x - (Exit)\n";
        cin >> ch;
        if (ch == 'l' || ch == 'L') {
            if (cust.login(1)) {
                char task = '0';
                while (task != 'x' && task != 'X') {
                    cout << "\n ***Welcome***";
                    cout << "\n What do you want to do today??";
                    cout << "\n a - (View available cars)";
                    cout << "\n b - (Your Rented Cars)";
                    cout << "\n c - (Rent a car)";
                    cout << "\n d - (Clear Dues)";
                    cout << "\n x - (Exit)\n";
                    cin >> task;
                    if (task == 'a' || task == 'A') {
                        cust.viewAvailableCars();
                    }
                    else if (task == 'b' || task == 'B') {
                        cust.viewRentedCars(cust.id);
                    }
                    else if (task == 'c' || task == 'C') {
                        cust.rentRequest(cust.id);
                    }
                    else if (task == 'd' || task == 'D') {
                        cust.clearDues(cust.id);
                    }
                    else if (task != 'x' && task != 'X') {
                        cout << "\n Please enter a valid key";
                    }
                }
            }
            cout << "\n";
            for (int i = 0; i < 30; i++)   cout << "*";
            cout << "\n";
        }
        else if (ch == 'r' || ch == 'R') {
            cust.registerCustomer();
        }
        else if (ch != 'x' && ch != 'X') {
            cout << "\n Please enter a valid key";
        }
        cout << "\n";
        for (int i = 0; i < 30; i++)   cout << "*";
        cout << "\n";
    }
}

using namespace std;
int main()
{
    try {
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;

        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://localhost:3306",
            "root", "Love24@1380");   // Enter your local host password here

        sql::Statement* stmt = con->createStatement();

        stmt->execute("CREATE DATABASE IF NOT EXISTS car_rental_system");
        con->setSchema("car_rental_system");

        sql::ResultSet* res = stmt->executeQuery("SHOW TABLES LIKE 'cars'");
        int count = 0;
        while (res->next()) {
            count++;
        }

        if (count == 0) {
            sql::Statement* stmt = con->createStatement();
            stmt->execute("CREATE TABLE IF NOT EXISTS cars (car_id INT AUTO_INCREMENT PRIMARY KEY, company VARCHAR(255), model VARCHAR(255), user_id VARCHAR(255), price INT, car_condition VARCHAR(255), due_date DATE)");
            stmt->execute("CREATE TABLE IF NOT EXISTS customers (customer_id VARCHAR(255) PRIMARY KEY, name VARCHAR(255), password VARCHAR(255), fine INT, record BOOL, rented_cars INT)");
            stmt->execute("CREATE TABLE IF NOT EXISTS employees (employee_id VARCHAR(255) PRIMARY KEY, name VARCHAR(255), password VARCHAR(255), fine INT, record BOOL, rented_cars INT)");

            stmt->execute("INSERT cars (company, model, price, car_condition) VALUES ('CompanyA', 'CarModel1', 50000, 'new'), ('CompanyA', 'CarModel2', 50000, 'new'), ('CompanyA', 'CarModel3', 50000, 'new'), ('CompanyA', 'CarModel4', 50000, 'secondhand'), ('CompanyA', 'CarModel5', 50000, 'secondhand')");
            stmt->execute("INSERT customers (customer_id, name, password, fine, record, rented_cars) VALUES ('cust001', 'Customer1', 'pass', 0, TRUE, 0), ('cust002', 'Customer2', 'pass', 0, TRUE, 0), ('cust003', 'Customer3', 'pass', 0, TRUE, 0), ('cust004', 'Customer4', 'pass', 0, TRUE, 0), ('cust005', 'Customer5', 'pass', 0, TRUE, 0)");
            stmt->execute("INSERT employees (employee_id, name, password, fine, record, rented_cars) VALUES ('emp001', 'Employee1', 'pass', 0, TRUE, 0), ('emp002', 'Employee2', 'pass', 0, TRUE, 0), ('emp003', 'Employee3', 'pass', 0, TRUE, 0), ('emp004', 'Employee4', 'pass', 0, TRUE, 0), ('emp005', 'Employee5', 'pass', 0, TRUE, 0)");
        }

        char ch = '0';
        while (ch != 'x' && ch != 'X') {
            cout << "\n ****Welcome to Car Rental System***";
            cout << "\n Cars are rented for 30 days only";
            cout << "\n Who you are?? Press Key to let me know";
            cout << "\n m - (Manager)";
            cout << "\n e - (Employee)";
            cout << "\n c - (Customer)";
            cout << "\n x - (Exit)\n";
            cin >> ch;
            if (ch == 'm' || ch == 'M') {
                managerPage(con);
            }
            else if (ch == 'e' || ch == 'E') {
                employeePage(con);
            }
            else if (ch == 'c' || ch == 'C') {
                customerPage(con);
            }
            else if (ch!='x' && ch!='X') {
                cout << "\n Please enter a correct key";
            }
            cout << "\n";
            for (int i = 0; i < 30; i++)   cout << "*";
            cout << "\n";
        }
        con->close();
        delete stmt;    //Delete pointers
        delete con;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}