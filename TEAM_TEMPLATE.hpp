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
// MEMBER 1 (LEADER): Database Singleton Manager
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
// MEMBER 2: User Hierarchy & Factory Pattern (Role-Based Access)
// ============================================================================
// Hey this section is for you. We need the user system with roles.
// Basically we have 3 types of users: Admin, Receptionist, and Mechanic.
// Each one has different permissions. Use inheritance for this.
//
// The users table looks like this:
//   id | username | password_hash | full_name | role
// role is either 'Admin', 'Receptionist', or 'Mechanic'
//
// Admin can do everything. Receptionist handles customers, appointments, and billing.
// Mechanic only deals with service orders and inventory.
//
// You need to make:
//   - UserRole base class (abstract) with virtual permission methods
//   - Admin, Receptionist, Mechanic classes that inherit from UserRole
//   - UserFactory to create the right user type based on role string
//   - UserRepository with authenticate() method that checks username/password
//
// Use PQexecParams for the login query so we don't get SQL injected.
// The permission methods should just return true/false based on role.

class UserRole {
    // TODO: base class with id, username, fullName
    // TODO: virtual getRole() = 0
    // TODO: virtual bool canManageUsers() const { return false; }
    // TODO: virtual bool canManageCustomers() const { return false; }
    // TODO: virtual bool canManageAppointments() const { return false; }
    // TODO: virtual bool canUpdateServiceOrders() const { return false; }
    // TODO: virtual bool canManageInventory() const { return false; }
    // TODO: virtual bool canProcessBilling() const { return false; }
    // TODO: getters for username, fullName, id
};

class Admin : public UserRole {
    // TODO: all permissions return true
};

class Receptionist : public UserRole {
    // TODO: canManageCustomers, canManageAppointments, canProcessBilling = true
};

class Mechanic : public UserRole {
    // TODO: canUpdateServiceOrders, canManageInventory = true
};

class UserFactory {
    // TODO: static createUser(id, uname, name, role) -> unique_ptr<UserRole>
    // TODO: if role == "Admin" return make_unique<Admin>(...)
    // TODO: if role == "Receptionist" return make_unique<Receptionist>(...)
    // TODO: if role == "Mechanic" return make_unique<Mechanic>(...)
};

class UserRepository {
    // TODO: authenticate(username, password) -> unique_ptr<UserRole>
    // TODO: query users table where username = $1 AND password_hash = $2
    // TODO: if found, use UserFactory to create the right type
    // TODO: if not found, return nullptr
};


// ============================================================================
// MEMBER 3: Customer & Vehicle Registry
// ============================================================================
// This part handles customers and their vehicles. Pretty straightforward CRUD stuff.
//
// customers table: id, name, phone (unique), email
// vehicles table: id, customer_id (FK), license_plate (unique), make, model, year
//
// For the vehicle list, we need to show the customer name too, so use a JOIN.
// Search should be case-insensitive, use ILIKE for that.
//
// Make structs for Customer and Vehicle to pass data around.

struct Customer {
    // TODO: int id, string name, string phone, string email
};

struct Vehicle {
    // TODO: int id, int customerId, string licensePlate, string make, string model, int year
    // TODO: string customerName (from JOIN)
};

class CustomerRepository {
    // TODO: addCustomer(name, phone, email) -> bool
    // TODO: getAllCustomers() -> vector<Customer>
    // TODO: searchCustomers(query) -> vector<Customer> using ILIKE
};

class VehicleRepository {
    // TODO: addVehicle(customerId, plate, make, model, year) -> bool
    // TODO: getAllVehicles() -> vector<Vehicle> (JOIN customers to get name)
    // TODO: getVehiclesByCustomer(customerId) -> vector<Vehicle>
};


// ============================================================================
// MEMBER 4: Appointment Scheduling System
// ============================================================================
// Appointments link a vehicle and a customer to a date/time.
// Status flow: Pending -> Confirmed -> Completed (or Cancelled)
//
// appointments table: id, vehicle_id (FK), customer_id (FK), scheduled_date, status
//
// When showing appointments, we need vehicle plate and customer name,
// so JOIN with vehicles and customers tables.

struct Appointment {
    // TODO: int id, int vehicleId, int customerId, string scheduledDate, string status
    // TODO: string vehiclePlate, string customerName (from JOINs)
};

class AppointmentRepository {
    // TODO: createAppointment(vehicleId, customerId, dateStr) -> bool
    // TODO:   insert with status 'Pending'
    // TODO: updateAppointmentStatus(appointmentId, status) -> bool
    // TODO: getAllAppointments() -> vector<Appointment> (JOIN vehicles + customers)
    // TODO: getAppointmentsByStatus(status) -> vector<Appointment>
};


// ============================================================================
// MEMBER 5: Service Orders & Observer Pattern
// ============================================================================
// Service orders are the repair jobs. They come from confirmed appointments.
// A mechanic is assigned and the status goes: Pending -> In Progress -> Completed
//
// We also need the Observer pattern here. When status changes, we notify
// observers. AuditLogger is one observer that just prints to console.
//
// service_orders table: id, appointment_id (FK), mechanic_id (FK), status, creation_date, notes
//
// For the list, JOIN with appointments, vehicles, and users to get names.

struct ServiceOrder {
    // TODO: int id, int appointmentId, int mechanicId, string status
    // TODO: string creationDate, string notes
    // TODO: string mechanicName, string vehiclePlate (from JOINs)
};

class ServiceOrderObserver {
    // TODO: virtual void onStatusChange(int orderId, const string& newStatus) = 0
};

class AuditLogger : public ServiceOrderObserver {
    // TODO: onStatusChange prints "[AUDIT LOG] Order #X status changed to: Y"
};

class ServiceOrderManager {
    // TODO: vector<ServiceOrderObserver*> observers
    // TODO: void attach(ServiceOrderObserver* observer)
    // TODO: bool createServiceOrder(appointmentId, mechanicId, notes)
    // TODO: void updateOrderStatus(orderId, newStatus) - notifies all observers
    // TODO: getAllServiceOrders() -> vector<ServiceOrder> (JOIN appointments, vehicles, users)
    // TODO: getServiceOrdersByMechanic(mechanicId) -> vector<ServiceOrder>
    // TODO: getServiceOrdersByStatus(status) -> vector<ServiceOrder>
};


// ============================================================================
// MEMBER 6: Inventory & Spare Parts Tracking
// ============================================================================
// Parts inventory. Mechanics use parts during repairs, stock goes down.
// We track which parts are used on which orders.
//
// parts table: id, name, part_number (unique), unit_price, stock_quantity
// service_order_parts table: id, service_order_id (FK), part_id (FK), quantity
//
// consumePartForOrder is the tricky one: it needs to insert into service_order_parts
// AND update the parts stock. Both need to work or we have a problem.

struct Part {
    // TODO: int id, string name, string partNumber, double unitPrice, int stockQuantity
};

class InventoryRepository {
    // TODO: addPart(name, partNumber, price, stockQuantity) -> bool
    // TODO: consumePartForOrder(orderId, partId, quantity) -> bool
    // TODO:   first INSERT into service_order_parts
    // TODO:   then UPDATE parts SET stock_quantity = stock_quantity - quantity
    // TODO:   if either fails, return false
    // TODO: updatePartStock(partId, newQuantity) -> bool
    // TODO: getAllParts() -> vector<Part>
    // TODO: getPartsForOrder(orderId) -> vector<Part> (JOIN service_order_parts)
};


// ============================================================================
// MEMBER 7: Payment Strategy & Billing Calculation
// ============================================================================
// Billing. We calculate the total from parts used on a service order,
// then process payment as Cash or Card.
//
// payments table: id, service_order_id (FK), amount, payment_method, payment_date
// payment_method is either 'Cash' or 'Card'
//
// Use the Strategy pattern here. PaymentStrategy is the interface,
// CashPayment and CardPayment are the concrete implementations.
//
// calculateTotalBill does this SQL:
//   SELECT COALESCE(SUM(p.unit_price * sop.quantity), 0.0)
//   FROM service_order_parts sop
//   JOIN parts p ON sop.part_id = p.id
//   WHERE sop.service_order_id = $1

struct Payment {
    // TODO: int id, int serviceOrderId, double amount, string paymentMethod, string paymentDate
};

class PaymentStrategy {
    // TODO: virtual bool processPayment(int orderId, double amount) = 0
};

class CashPayment : public PaymentStrategy {
    // TODO: processPayment inserts into payments with method 'Cash'
};

class CardPayment : public PaymentStrategy {
    // TODO: processPayment inserts into payments with method 'Card'
};

class BillingService {
    // TODO: double calculateTotalBill(int orderId)
    // TODO: bool processPayment(int orderId, double amount, const string& method)
    // TODO:   if method == "Cash" use CashPayment
    // TODO:   else use CardPayment
    // TODO: getPaymentsForOrder(orderId) -> vector<Payment>
    // TODO: getTotalRevenue() -> double (SELECT SUM(amount) FROM payments)
};


#endif // VEHICLE_SERVICE_CENTER_HPP

// ============================================================================
// Notes for everyone:
//
// - Always use PQexecParams for anything with user input. Don't concatenate strings into queries.
// - Check PQresultStatus after every query. PGRES_COMMAND_OK for INSERT/UPDATE/DELETE, PGRES_TUPLES_OK for SELECT.
// - Always PQclear your results when done.
// - Use std::stoi / std::stod to convert string results to numbers.
// - Use std::to_string to convert numbers to strings for queries.
// - Get the DB connection with DatabaseManager::getInstance()->getConnection()
// - If something goes wrong, return false. The GUI will show an error message.
//
// Here's a basic pattern for a repository method:
//
// bool SomeClass::someMethod(int param1, const string& param2) {
//     PGconn* conn = DatabaseManager::getInstance()->getConnection();
//     string p1 = std::to_string(param1);
//
//     const char* query = "INSERT INTO table (col1, col2) VALUES ($1, $2);";
//     const char* params[2] = { p1.c_str(), param2.c_str() };
//
//     PGresult* res = PQexecParams(conn, query, 2, NULL, params, NULL, NULL, 0);
//     bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
//     PQclear(res);
//     return ok;
// }
//
// ============================================================================