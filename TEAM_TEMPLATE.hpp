#ifndef VEHICLE_SERVICE_CENTER_HPP
#define VEHICLE_SERVICE_CENTER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <libpq-fe.h>

// ============================================================================
// MEMBER 1 (LEADER): Database Singleton Manager — COMPLETED
// ============================================================================
class DatabaseManager {
private:
    static DatabaseManager* instance;
    static std::mutex mutex;
    PGconn* conn;

    DatabaseManager(const std::string& conn_str) {
        conn = PQconnectdb(conn_str.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
            std::string err = PQerrorMessage(conn);
            PQfinish(conn);
            conn = nullptr;
            throw std::runtime_error("PostgreSQL Connection Failed: " + err);
        }
    }

public:
    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;

    ~DatabaseManager() {
        if (conn) PQfinish(conn);
    }

    static DatabaseManager* getInstance(const std::string& conn_str = "") {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            if (conn_str.empty()) {
                throw std::runtime_error("Connection string required for initial DB creation!");
            }
            instance = new DatabaseManager(conn_str);
        }
        return instance;
    }

    PGconn* getConnection() { return conn; }
};

inline DatabaseManager* DatabaseManager::instance = nullptr;
inline std::mutex DatabaseManager::mutex;


// ============================================================================
// MEMBER 2: User Hierarchy & Factory Pattern
// ============================================================================
class UserRole {
protected:
    int id;
    std::string username;
    std::string fullName;
public:
    UserRole(int id, std::string uname, std::string name)
        : id(id), username(std::move(uname)), fullName(std::move(name)) {}
    virtual ~UserRole() = default;

    virtual std::string getRole() const = 0;
    virtual bool canManageUsers() const { return false; }
    virtual bool canManageCustomers() const { return false; }
    virtual bool canManageAppointments() const { return false; }
    virtual bool canUpdateServiceOrders() const { return false; }
    virtual bool canManageInventory() const { return false; }
    virtual bool canProcessBilling() const { return false; }

    std::string getUsername() const { return username; }
    std::string getFullName() const { return fullName; }
    int getId() const { return id; }
};

class Admin : public UserRole {
public:
    Admin(int id, std::string u, std::string n) : UserRole(id, u, n) {}
    std::string getRole() const override { return "Admin"; }
    bool canManageUsers() const override { return true; }
    bool canManageCustomers() const override { return true; }
    bool canManageAppointments() const override { return true; }
    bool canUpdateServiceOrders() const override { return true; }
    bool canManageInventory() const override { return true; }
    bool canProcessBilling() const override { return true; }
};

class Receptionist : public UserRole {
public:
    Receptionist(int id, std::string u, std::string n) : UserRole(id, u, n) {}
    std::string getRole() const override { return "Receptionist"; }
    bool canManageCustomers() const override { return true; }
    bool canManageAppointments() const override { return true; }
    bool canProcessBilling() const override { return true; }
};

class Mechanic : public UserRole {
public:
    Mechanic(int id, std::string u, std::string n) : UserRole(id, u, n) {}
    std::string getRole() const override { return "Mechanic"; }
    bool canUpdateServiceOrders() const override { return true; }
    bool canManageInventory() const override { return true; }
};

class UserFactory {
public:
    static std::unique_ptr<UserRole> createUser(int id, const std::string& uname, const std::string& name, const std::string& role) {
        if (role == "Admin") return std::make_unique<Admin>(id, uname, name);
        if (role == "Receptionist") return std::make_unique<Receptionist>(id, uname, name);
        if (role == "Mechanic") return std::make_unique<Mechanic>(id, uname, name);
        return nullptr;
    }
};

class UserRepository {
public:
    std::unique_ptr<UserRole> authenticate(const std::string& username, const std::string& password) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT id, username, full_name, role FROM users WHERE username = $1 AND password_hash = $2;";
        const char* paramValues[2] = { username.c_str(), password.c_str() };

        PGresult* res = PQexecParams(conn, query, 2, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
            PQclear(res);
            return nullptr;
        }

        int id = std::stoi(PQgetvalue(res, 0, 0));
        std::string uname = PQgetvalue(res, 0, 1);
        std::string fullName = PQgetvalue(res, 0, 2);
        std::string role = PQgetvalue(res, 0, 3);

        PQclear(res);
        return UserFactory::createUser(id, uname, fullName, role);
    }

    bool createUser(const std::string& username, const std::string& password, const std::string& fullName, const std::string& role) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "INSERT INTO users (username, password_hash, full_name, role) VALUES ($1, $2, $3, $4);";
        const char* paramValues[4] = { username.c_str(), password.c_str(), fullName.c_str(), role.c_str() };

        PGresult* res = PQexecParams(conn, query, 4, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    std::vector<User> getAllUsers() {
        std::vector<User> users;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT id, username, full_name, role FROM users ORDER BY id;";
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return users; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            users.push_back({ std::stoi(PQgetvalue(res, i, 0)), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3) });
        }
        PQclear(res);
        return users;
    }
};

struct User {
    int id;
    std::string username;
    std::string fullName;
    std::string role;
};


// ============================================================================
// MEMBER 3: Customer & Vehicle Registry
// ============================================================================
struct Customer {
    int id;
    std::string name;
    std::string phone;
    std::string email;
};

struct Vehicle {
    int id;
    int customerId;
    std::string licensePlate;
    std::string make;
    std::string model;
    int year;
    std::string customerName;
};

class CustomerRepository {
public:
    bool addCustomer(const std::string& name, const std::string& phone, const std::string& email = "") {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "INSERT INTO customers (name, phone, email) VALUES ($1, $2, $3);";
        const char* paramValues[3] = { name.c_str(), phone.c_str(), email.c_str() };

        PGresult* res = PQexecParams(conn, query, 3, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    std::vector<Customer> getAllCustomers() {
        std::vector<Customer> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT id, name, phone, email FROM customers ORDER BY name;";
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3) });
        }
        PQclear(res);
        return list;
    }

    std::vector<Customer> searchCustomers(const std::string& query) {
        std::vector<Customer> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string q = "%" + query + "%";
        const char* sql = "SELECT id, name, phone, email FROM customers WHERE name ILIKE $1 OR phone ILIKE $1 OR email ILIKE $1 ORDER BY name;";
        const char* paramValues[1] = { q.c_str() };
        PGresult* res = PQexecParams(conn, sql, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3) });
        }
        PQclear(res);
        return list;
    }
};

class VehicleRepository {
public:
    bool addVehicle(int customerId, const std::string& plate, const std::string& make, const std::string& model, int year) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string custIdStr = std::to_string(customerId);
        std::string yearStr = std::to_string(year);

        const char* query = "INSERT INTO vehicles (customer_id, license_plate, make, model, year) VALUES ($1, $2, $3, $4, $5);";
        const char* paramValues[5] = { custIdStr.c_str(), plate.c_str(), make.c_str(), model.c_str(), yearStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 5, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    std::vector<Vehicle> getAllVehicles() {
        std::vector<Vehicle> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT v.id, v.customer_id, v.license_plate, v.make, v.model, v.year, c.name "
                            "FROM vehicles v JOIN customers c ON v.customer_id = c.id ORDER BY v.id;";
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), std::stoi(PQgetvalue(res, i, 5)), PQgetvalue(res, i, 6) });
        }
        PQclear(res);
        return list;
    }
};


// ============================================================================
// MEMBER 4: Appointment Scheduling System
// ============================================================================
struct Appointment {
    int id;
    int vehicleId;
    int customerId;
    std::string scheduledDate;
    std::string status;
    std::string vehiclePlate;
    std::string customerName;
};

class AppointmentRepository {
public:
    bool createAppointment(int vehicleId, int customerId, const std::string& dateStr) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string vIdStr = std::to_string(vehicleId);
        std::string cIdStr = std::to_string(customerId);

        const char* query = "INSERT INTO appointments (vehicle_id, customer_id, scheduled_date, status) VALUES ($1, $2, $3, 'Pending');";
        const char* paramValues[3] = { vIdStr.c_str(), cIdStr.c_str(), dateStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 3, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    bool updateAppointmentStatus(int appointmentId, const std::string& status) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string apptIdStr = std::to_string(appointmentId);

        const char* query = "UPDATE appointments SET status = $1 WHERE id = $2;";
        const char* paramValues[2] = { status.c_str(), apptIdStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 2, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    std::vector<Appointment> getAllAppointments() {
        std::vector<Appointment> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT a.id, a.vehicle_id, a.customer_id, a.scheduled_date, a.status, v.license_plate, c.name "
                            "FROM appointments a JOIN vehicles v ON a.vehicle_id = v.id JOIN customers c ON a.customer_id = c.id ORDER BY a.scheduled_date;";
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), std::stoi(PQgetvalue(res, i, 2)), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5), PQgetvalue(res, i, 6) });
        }
        PQclear(res);
        return list;
    }

    std::vector<Appointment> getAppointmentsByStatus(const std::string& status) {
        std::vector<Appointment> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT a.id, a.vehicle_id, a.customer_id, a.scheduled_date, a.status, v.license_plate, c.name "
                            "FROM appointments a JOIN vehicles v ON a.vehicle_id = v.id JOIN customers c ON a.customer_id = c.id "
                            "WHERE a.status = $1 ORDER BY a.scheduled_date;";
        const char* paramValues[1] = { status.c_str() };
        PGresult* res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), std::stoi(PQgetvalue(res, i, 2)), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5), PQgetvalue(res, i, 6) });
        }
        PQclear(res);
        return list;
    }
};


// ============================================================================
// MEMBER 5: Service Orders & Observer Pattern
// ============================================================================
struct ServiceOrder {
    int id;
    int appointmentId;
    int mechanicId;
    std::string status;
    std::string creationDate;
    std::string notes;
    std::string mechanicName;
    std::string vehiclePlate;
};

class ServiceOrderObserver {
public:
    virtual ~ServiceOrderObserver() = default;
    virtual void onStatusChange(int orderId, const std::string& newStatus) = 0;
};

class AuditLogger : public ServiceOrderObserver {
public:
    void onStatusChange(int orderId, const std::string& newStatus) override {
        std::cout << "[AUDIT LOG] Service Order #" << orderId << " status changed to: " << newStatus << "\n";
    }
};

class ServiceOrderManager {
private:
    std::vector<ServiceOrderObserver*> observers;
public:
    void attach(ServiceOrderObserver* observer) { observers.push_back(observer); }

    bool createServiceOrder(int appointmentId, int mechanicId, const std::string& notes) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string aIdStr = std::to_string(appointmentId);
        std::string mIdStr = std::to_string(mechanicId);
        const char* query = "INSERT INTO service_orders (appointment_id, mechanic_id, status, notes) VALUES ($1, $2, 'Pending', $3);";
        const char* paramValues[3] = { aIdStr.c_str(), mIdStr.c_str(), notes.c_str() };
        PGresult* res = PQexecParams(conn, query, 3, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    void updateOrderStatus(int orderId, const std::string& newStatus) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string orderIdStr = std::to_string(orderId);

        const char* query = "UPDATE service_orders SET status = $1 WHERE id = $2;";
        const char* paramValues[2] = { newStatus.c_str(), orderIdStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 2, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) == PGRES_COMMAND_OK) {
            for (auto* obs : observers) {
                if (obs) obs->onStatusChange(orderId, newStatus);
            }
        }
        PQclear(res);
    }

    std::vector<ServiceOrder> getAllServiceOrders() {
        std::vector<ServiceOrder> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT so.id, so.appointment_id, so.mechanic_id, so.status, so.creation_date, so.notes, u.full_name, v.license_plate "
                            "FROM service_orders so "
                            "JOIN appointments a ON so.appointment_id = a.id "
                            "JOIN vehicles v ON a.vehicle_id = v.id "
                            "LEFT JOIN users u ON so.mechanic_id = u.id "
                            "ORDER BY so.id DESC;";
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), std::stoi(PQgetvalue(res, i, 2)), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5), PQgetvalue(res, i, 6), PQgetvalue(res, i, 7) });
        }
        PQclear(res);
        return list;
    }

    std::vector<ServiceOrder> getServiceOrdersByMechanic(int mechanicId) {
        std::vector<ServiceOrder> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string mIdStr = std::to_string(mechanicId);
        const char* query = "SELECT so.id, so.appointment_id, so.mechanic_id, so.status, so.creation_date, so.notes, u.full_name, v.license_plate "
                            "FROM service_orders so "
                            "JOIN appointments a ON so.appointment_id = a.id "
                            "JOIN vehicles v ON a.vehicle_id = v.id "
                            "LEFT JOIN users u ON so.mechanic_id = u.id "
                            "WHERE so.mechanic_id = $1 ORDER BY so.id DESC;";
        const char* paramValues[1] = { mIdStr.c_str() };
        PGresult* res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), std::stoi(PQgetvalue(res, i, 2)), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5), PQgetvalue(res, i, 6), PQgetvalue(res, i, 7) });
        }
        PQclear(res);
        return list;
    }

    std::vector<ServiceOrder> getServiceOrdersByStatus(const std::string& status) {
        std::vector<ServiceOrder> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT so.id, so.appointment_id, so.mechanic_id, so.status, so.creation_date, so.notes, u.full_name, v.license_plate "
                            "FROM service_orders so "
                            "JOIN appointments a ON so.appointment_id = a.id "
                            "JOIN vehicles v ON a.vehicle_id = v.id "
                            "LEFT JOIN users u ON so.mechanic_id = u.id "
                            "WHERE so.status = $1 ORDER BY so.id DESC;";
        const char* paramValues[1] = { status.c_str() };
        PGresult* res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), std::stoi(PQgetvalue(res, i, 2)), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5), PQgetvalue(res, i, 6), PQgetvalue(res, i, 7) });
        }
        PQclear(res);
        return list;
    }
};


// ============================================================================
// MEMBER 6: Inventory & Spare Parts Tracking
// ============================================================================
struct Part {
    int id;
    std::string name;
    std::string partNumber;
    double unitPrice;
    int stockQuantity;
};

class InventoryRepository {
public:
    bool addPart(const std::string& name, const std::string& partNumber, double price, int stockQuantity) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string priceStr = std::to_string(price);
        std::string qtyStr = std::to_string(stockQuantity);

        const char* query = "INSERT INTO parts (name, part_number, unit_price, stock_quantity) VALUES ($1, $2, $3, $4);";
        const char* paramValues[4] = { name.c_str(), partNumber.c_str(), priceStr.c_str(), qtyStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 4, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    bool consumePartForOrder(int orderId, int partId, int quantity) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string orderIdStr = std::to_string(orderId);
        std::string partIdStr = std::to_string(partId);
        std::string qtyStr = std::to_string(quantity);

        const char* query1 = "INSERT INTO service_order_parts (service_order_id, part_id, quantity) VALUES ($1, $2, $3);";
        const char* paramValues1[3] = { orderIdStr.c_str(), partIdStr.c_str(), qtyStr.c_str() };

        PGresult* res1 = PQexecParams(conn, query1, 3, NULL, paramValues1, NULL, NULL, 0);
        if (PQresultStatus(res1) != PGRES_COMMAND_OK) {
            PQclear(res1);
            return false;
        }
        PQclear(res1);

        const char* query2 = "UPDATE parts SET stock_quantity = stock_quantity - $1 WHERE id = $2;";
        const char* paramValues2[2] = { qtyStr.c_str(), partIdStr.c_str() };

        PGresult* res2 = PQexecParams(conn, query2, 2, NULL, paramValues2, NULL, NULL, 0);
        bool success = (PQresultStatus(res2) == PGRES_COMMAND_OK);
        PQclear(res2);
        return success;
    }

    bool updatePartStock(int partId, int newQuantity) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string partIdStr = std::to_string(partId);
        std::string qtyStr = std::to_string(newQuantity);
        const char* query = "UPDATE parts SET stock_quantity = $1 WHERE id = $2;";
        const char* paramValues[2] = { qtyStr.c_str(), partIdStr.c_str() };
        PGresult* res = PQexecParams(conn, query, 2, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    std::vector<Part> getAllParts() {
        std::vector<Part> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT id, name, part_number, unit_price, stock_quantity FROM parts ORDER BY name;";
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), std::stod(PQgetvalue(res, i, 3)), std::stoi(PQgetvalue(res, i, 4)) });
        }
        PQclear(res);
        return list;
    }

    std::vector<Part> getPartsForOrder(int orderId) {
        std::vector<Part> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string oIdStr = std::to_string(orderId);
        const char* query = "SELECT p.id, p.name, p.part_number, p.unit_price, sop.quantity "
                            "FROM service_order_parts sop JOIN parts p ON sop.part_id = p.id "
                            "WHERE sop.service_order_id = $1;";
        const char* paramValues[1] = { oIdStr.c_str() };
        PGresult* res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), std::stod(PQgetvalue(res, i, 3)), std::stoi(PQgetvalue(res, i, 4)) });
        }
        PQclear(res);
        return list;
    }
};


// ============================================================================
// MEMBER 7: Payment Strategy & Billing Calculation
// ============================================================================
struct Payment {
    int id;
    int serviceOrderId;
    double amount;
    std::string paymentMethod;
    std::string paymentDate;
};

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(int orderId, double amount) = 0;
};

class CashPayment : public PaymentStrategy {
public:
    bool processPayment(int orderId, double amount) override {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string orderIdStr = std::to_string(orderId);
        std::string amountStr = std::to_string(amount);

        const char* query = "INSERT INTO payments (service_order_id, amount, payment_method) VALUES ($1, $2, 'Cash');";
        const char* paramValues[2] = { orderIdStr.c_str(), amountStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 2, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }
};

class CardPayment : public PaymentStrategy {
public:
    bool processPayment(int orderId, double amount) override {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string orderIdStr = std::to_string(orderId);
        std::string amountStr = std::to_string(amount);

        const char* query = "INSERT INTO payments (service_order_id, amount, payment_method) VALUES ($1, $2, 'Card');";
        const char* paramValues[2] = { orderIdStr.c_str(), amountStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 2, NULL, paramValues, NULL, NULL, 0);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }
};

class BillingService {
public:
    double calculateTotalBill(int orderId) {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string orderIdStr = std::to_string(orderId);

        const char* query = "SELECT COALESCE(SUM(p.unit_price * sop.quantity), 0.0) FROM service_order_parts sop JOIN parts p ON sop.part_id = p.id WHERE sop.service_order_id = $1;";
        const char* paramValues[1] = { orderIdStr.c_str() };

        PGresult* res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
        double total = 0.0;
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
            total = std::stod(PQgetvalue(res, 0, 0));
        }
        PQclear(res);
        return total;
    }

    bool processPayment(int orderId, double amount, const std::string& method) {
        if (method == "Cash") {
            CashPayment strategy;
            return strategy.processPayment(orderId, amount);
        } else {
            CardPayment strategy;
            return strategy.processPayment(orderId, amount);
        }
    }

    std::vector<Payment> getPaymentsForOrder(int orderId) {
        std::vector<Payment> list;
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        std::string oIdStr = std::to_string(orderId);
        const char* query = "SELECT id, service_order_id, amount, payment_method, payment_date FROM payments WHERE service_order_id = $1 ORDER BY payment_date;";
        const char* paramValues[1] = { oIdStr.c_str() };
        PGresult* res = PQexecParams(conn, query, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) { PQclear(res); return list; }
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            list.push_back({ std::stoi(PQgetvalue(res, i, 0)), std::stoi(PQgetvalue(res, i, 1)), std::stod(PQgetvalue(res, i, 2)), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4) });
        }
        PQclear(res);
        return list;
    }

    double getTotalRevenue() {
        PGconn* conn = DatabaseManager::getInstance()->getConnection();
        const char* query = "SELECT COALESCE(SUM(amount), 0.0) FROM payments;";
        PGresult* res = PQexec(conn, query);
        double total = 0.0;
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
            total = std::stod(PQgetvalue(res, 0, 0));
        }
        PQclear(res);
        return total;
    }
};


#endif // VEHICLE_SERVICE_CENTER_HPP