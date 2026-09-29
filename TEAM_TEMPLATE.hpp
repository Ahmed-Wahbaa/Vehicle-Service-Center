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
// TODO: Implement the following classes

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
    // TODO: Implement getRole() -> return "Admin"
    // TODO: Override all permission methods to return true
};

class Receptionist : public UserRole {
public:
    Receptionist(int id, std::string u, std::string n) : UserRole(id, u, n) {}
    // TODO: Implement getRole() -> return "Receptionist"
    // TODO: Override canManageCustomers, canManageAppointments, canProcessBilling -> true
};

class Mechanic : public UserRole {
public:
    Mechanic(int id, std::string u, std::string n) : UserRole(id, u, n) {}
    // TODO: Implement getRole() -> return "Mechanic"
    // TODO: Override canUpdateServiceOrders, canManageInventory -> true
};

class UserFactory {
public:
    // TODO: Implement createUser(id, uname, name, role)
    // TODO: Return make_unique<Admin> if role == "Admin"
    // TODO: Return make_unique<Receptionist> if role == "Receptionist"
    // TODO: Return make_unique<Mechanic> if role == "Mechanic"
    static std::unique_ptr<UserRole> createUser(int id, const std::string& uname, const std::string& name, const std::string& role);
};

class UserRepository {
public:
    // TODO: Implement authenticate(username, password)
    // TODO: Query users table, return nullptr if not found
    std::unique_ptr<UserRole> authenticate(const std::string& username, const std::string& password);

    // TODO: Implement createUser(username, password, fullName, role)
    bool createUser(const std::string& username, const std::string& password, const std::string& fullName, const std::string& role);

    // TODO: Implement getAllUsers()
    std::vector<User> getAllUsers();
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
// TODO: Implement the following classes

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
    // TODO: Implement addCustomer(name, phone, email) -> bool
    bool addCustomer(const std::string& name, const std::string& phone, const std::string& email = "");

    // TODO: Implement getAllCustomers() -> vector<Customer>
    std::vector<Customer> getAllCustomers();

    // TODO: Implement searchCustomers(query) -> vector<Customer>
    std::vector<Customer> searchCustomers(const std::string& query);
};

class VehicleRepository {
public:
    // TODO: Implement addVehicle(customerId, plate, make, model, year) -> bool
    bool addVehicle(int customerId, const std::string& plate, const std::string& make, const std::string& model, int year);

    // TODO: Implement getAllVehicles() -> vector<Vehicle>
    std::vector<Vehicle> getAllVehicles();

    // TODO: Implement getVehiclesByCustomer(customerId) -> vector<Vehicle>
    std::vector<Vehicle> getVehiclesByCustomer(int customerId);
};


// ============================================================================
// MEMBER 4: Appointment Scheduling System
// ============================================================================
// TODO: Implement the following class

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
    // TODO: Implement createAppointment(vehicleId, customerId, dateStr) -> bool
    bool createAppointment(int vehicleId, int customerId, const std::string& dateStr);

    // TODO: Implement updateAppointmentStatus(appointmentId, status) -> bool
    bool updateAppointmentStatus(int appointmentId, const std::string& status);

    // TODO: Implement getAllAppointments() -> vector<Appointment>
    std::vector<Appointment> getAllAppointments();

    // TODO: Implement getAppointmentsByStatus(status) -> vector<Appointment>
    std::vector<Appointment> getAppointmentsByStatus(const std::string& status);
};


// ============================================================================
// MEMBER 5: Service Orders & Observer Pattern
// ============================================================================
// TODO: Implement the following classes

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
    // TODO: Declare pure virtual onStatusChange(orderId, newStatus)
    virtual void onStatusChange(int orderId, const std::string& newStatus) = 0;
};

class AuditLogger : public ServiceOrderObserver {
public:
    // TODO: Implement onStatusChange - print audit message to console
    void onStatusChange(int orderId, const std::string& newStatus) override;
};

class ServiceOrderManager {
private:
    std::vector<ServiceOrderObserver*> observers;
public:
    // TODO: Implement attach(observer)
    void attach(ServiceOrderObserver* observer);

    // TODO: Implement createServiceOrder(appointmentId, mechanicId, notes) -> bool
    bool createServiceOrder(int appointmentId, int mechanicId, const std::string& notes);

    // TODO: Implement updateOrderStatus(orderId, newStatus) - notifies observers
    void updateOrderStatus(int orderId, const std::string& newStatus);

    // TODO: Implement getAllServiceOrders() -> vector<ServiceOrder>
    std::vector<ServiceOrder> getAllServiceOrders();

    // TODO: Implement getServiceOrdersByMechanic(mechanicId) -> vector<ServiceOrder>
    std::vector<ServiceOrder> getServiceOrdersByMechanic(int mechanicId);

    // TODO: Implement getServiceOrdersByStatus(status) -> vector<ServiceOrder>
    std::vector<ServiceOrder> getServiceOrdersByStatus(const std::string& status);
};


// ============================================================================
// MEMBER 6: Inventory & Spare Parts Tracking
// ============================================================================
// TODO: Implement the following class

struct Part {
    int id;
    std::string name;
    std::string partNumber;
    double unitPrice;
    int stockQuantity;
};

class InventoryRepository {
public:
    // TODO: Implement addPart(name, partNumber, price, stockQuantity) -> bool
    bool addPart(const std::string& name, const std::string& partNumber, double price, int stockQuantity);

    // TODO: Implement consumePartForOrder(orderId, partId, quantity) -> bool
    // TODO: Insert into service_order_parts, then update parts stock
    bool consumePartForOrder(int orderId, int partId, int quantity);

    // TODO: Implement updatePartStock(partId, newQuantity) -> bool
    bool updatePartStock(int partId, int newQuantity);

    // TODO: Implement getAllParts() -> vector<Part>
    std::vector<Part> getAllParts();

    // TODO: Implement getPartsForOrder(orderId) -> vector<Part>
    std::vector<Part> getPartsForOrder(int orderId);
};


// ============================================================================
// MEMBER 7: Payment Strategy & Billing Calculation
// ============================================================================
// TODO: Implement the following classes

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
    // TODO: Declare pure virtual processPayment(orderId, amount)
    virtual bool processPayment(int orderId, double amount) = 0;
};

class CashPayment : public PaymentStrategy {
public:
    // TODO: Implement processPayment - insert into payments with method 'Cash'
    bool processPayment(int orderId, double amount) override;
};

class CardPayment : public PaymentStrategy {
public:
    // TODO: Implement processPayment - insert into payments with method 'Card'
    bool processPayment(int orderId, double amount) override;
};

class BillingService {
public:
    // TODO: Implement calculateTotalBill(orderId) -> double
    // TODO: SQL: SELECT COALESCE(SUM(p.unit_price * sop.quantity), 0.0)
    // TODO:        FROM service_order_parts sop JOIN parts p ON sop.part_id = p.id
    // TODO:        WHERE sop.service_order_id = $1
    double calculateTotalBill(int orderId);

    // TODO: Implement processPayment(orderId, amount, method) -> bool
    // TODO: Use CashPayment if method == "Cash", else CardPayment
    bool processPayment(int orderId, double amount, const std::string& method);

    // TODO: Implement getPaymentsForOrder(orderId) -> vector<Payment>
    std::vector<Payment> getPaymentsForOrder(int orderId);

    // TODO: Implement getTotalRevenue() -> double
    double getTotalRevenue();
};


#endif // VEHICLE_SERVICE_CENTER_HPP

// ============================================================================
// Notes:
// - All structs are complete, use them as-is
// - All class declarations are complete, just fill in the method bodies
// - Use DatabaseManager::getInstance()->getConnection() for DB access
// - Use PQexecParams for all queries with parameters
// - Check PQresultStatus after every query
// - Call PQclear after every query
// - Return false on failure, true on success
// ============================================================================