# VEHICLE SERVICE CENTER MANAGEMENT SYSTEM
    Complete Project Explanation & Jira Stories

#### PROJECT OVERVIEW:
-----------------
A full-stack desktop application for managing a vehicle service center.
Built with C++17, Qt6 (GUI), and PostgreSQL (Database).
The system handles customers, vehicles, appointments, service orders,
inventory/spare parts, billing/payments, and user administration.

#### ARCHITECTURE:
-------------
- Presentation Layer: Qt6 Widgets (mainwindow.cpp) — Professional dark theme
- Business Logic Layer: vehicle_service_center.hpp (header-only library)
- Data Access Layer: PostgreSQL via libpq
- Design Patterns: Singleton, Factory, Observer, Strategy

#### FILES:
------
- main.cpp — Application entry point, database connection
- mainwindow.h — MainWindow class declaration
- mainwindow.cpp — Complete GUI implementation with dark theme
- vehicle_service_center.hpp — Complete business logic (all 7 members)
- TEAM_TEMPLATE.hpp — Skeleton with TODOs for team members to implement
- schema.sql — Database schema and test data
- CMakeLists.txt — Build configuration

---

## JIRA STORIES


STORY 1: Database Connection Manager (Team Member 1 - Leader) — COMPLETED

EPIC: Infrastructure
PRIORITY: Critical
ASSIGNED TO: Member 1 (Team Leader)

DESCRIPTION:
As a system administrator, I need a reliable database connection manager
so that the application can securely connect to PostgreSQL and execute
queries.

ACCEPTANCE CRITERIA:
- Implement Singleton pattern for DatabaseManager class
- Thread-safe connection using std::mutex
- Connection string: host=localhost port=5432 dbname=vehicle_db
- Automatic connection validation (PQstatus check)
- Graceful error handling with descriptive messages
- Proper resource cleanup in destructor (PQfinish)

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Class: DatabaseManager
- Pattern: Singleton with lazy initialization
- Thread safety: std::mutex with std::lock_guard
- Error handling: throws std::runtime_error on connection failure

KEY CODE:
    class DatabaseManager {
        static DatabaseManager* getInstance(const std::string& conn_str);
        PGconn* getConnection();
    };


STORY 2: User Authentication & Role-Based Access Control (Team Member 2)

EPIC: Security
PRIORITY: Critical
ASSIGNED TO: Member 2

DESCRIPTION:
As a system, I need role-based authentication so that different staff
members (Admin, Receptionist, Mechanic) can access only their permitted
features.

ACCEPTANCE CRITERIA:
- Implement User hierarchy with virtual methods
- Admin: full access to all features
- Receptionist: customers, appointments, billing
- Mechanic: service orders, inventory
- Factory pattern for creating appropriate user type
- Secure password verification against database
- Permission checking methods (canManageUsers, canManageCustomers, etc.)

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Classes: UserRole (abstract), Admin, Receptionist, Mechanic, UserFactory, UserRepository
- Pattern: Factory Method + Inheritance
- Database: users table with role column

KEY CODE:
    class UserRole { virtual bool canManageUsers() const = 0; };
    class Admin : public UserRole { /* all permissions true */ };
    class Receptionist : public UserRole { /* limited permissions */ };
    class Mechanic : public UserRole { /* mechanic permissions */ };
    UserFactory::createUser(id, uname, name, role);


STORY 3: Customer & Vehicle Registry (Team Member 3)

EPIC: Customer Management
PRIORITY: High
ASSIGNED TO: Member 3

DESCRIPTION:
As a receptionist, I need to register customers and their vehicles
so that the service center can track ownership and service history.

ACCEPTANCE CRITERIA:
- Add new customer (name, phone, email)
- Add new vehicle (customer, plate, make, model, year)
- View all customers in a searchable table
- View all vehicles with customer names (JOIN query)
- Search customers by name, phone, or email
- Prevent duplicate phone numbers and license plates

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Classes: CustomerRepository, VehicleRepository
- Database: customers table, vehicles table
- Constraints: UNIQUE on phone and license_plate
- JOIN query to display customer name with vehicle

KEY CODE:
    CustomerRepository::addCustomer(name, phone, email);
    CustomerRepository::searchCustomers(query);
    VehicleRepository::addVehicle(customerId, plate, make, model, year);
    VehicleRepository::getAllVehicles(); // JOIN with customers


STORY 4: Appointment Scheduling System (Team Member 4)

EPIC: Scheduling
PRIORITY: High
ASSIGNED TO: Member 4

DESCRIPTION:
As a receptionist, I need to schedule service appointments so that
customers can book their vehicle service in advance.

ACCEPTANCE CRITERIA:
- Create appointment (vehicle, customer, date/time)
- Update appointment status (Pending, Confirmed, Cancelled, Completed)
- View all appointments with vehicle and customer details
- Filter appointments by status
- Prevent scheduling conflicts

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Class: AppointmentRepository
- Database: appointments table
- Status workflow: Pending -> Confirmed -> Completed/Cancelled
- JOIN queries for vehicle plate and customer name display

KEY CODE:
    AppointmentRepository::createAppointment(vehicleId, customerId, dateStr);
    AppointmentRepository::updateAppointmentStatus(apptId, status);
    AppointmentRepository::getAppointmentsByStatus(status);


STORY 5: Service Order Management with Observer Pattern (Team Member 5)

EPIC: Work Orders
PRIORITY: High
ASSIGNED TO: Member 5

DESCRIPTION:
As a mechanic, I need to manage repair service orders so that
I can track the status of each vehicle repair from start to finish.

ACCEPTANCE CRITERIA:
- Create service order from confirmed appointment
- Assign mechanic to service order
- Update order status (Pending, In Progress, Completed)
- Observer pattern: notify on status change (audit logging)
- View all service orders with mechanic and vehicle details
- Filter orders by status or by assigned mechanic

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Classes: ServiceOrderObserver, AuditLogger, ServiceOrderManager
- Pattern: Observer pattern for status change notifications
- Database: service_orders table
- Status workflow: Pending -> In Progress -> Completed

KEY CODE:
    ServiceOrderObserver::onStatusChange(orderId, newStatus);
    AuditLogger: logs to console
    ServiceOrderManager::attach(observer);
    ServiceOrderManager::updateOrderStatus(orderId, status);
    ServiceOrderManager::getServiceOrdersByMechanic(mechanicId);


STORY 6: Inventory & Spare Parts Management (Team Member 6)

EPIC: Inventory
PRIORITY: Medium
ASSIGNED TO: Member 6

DESCRIPTION:
As an inventory manager, I need to track spare parts stock so that
mechanics can use parts during repairs and stock levels stay accurate.

ACCEPTANCE CRITERIA:
- Add new spare part (name, part number, price, stock quantity)
- Update stock quantity manually
- Log part usage on repair orders (auto-deduct from stock)
- View all parts with current stock levels
- Prevent negative stock (check before deducting)
- View parts used for specific repair order

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Class: InventoryRepository
- Database: parts table, service_order_parts (link table)
- Transaction: INSERT into link table + UPDATE stock
- Constraints: stock_quantity >= 0, quantity > 0

KEY CODE:
    InventoryRepository::addPart(name, partNumber, price, qty);
    InventoryRepository::consumePartForOrder(orderId, partId, qty);
    InventoryRepository::updatePartStock(partId, newQty);
    InventoryRepository::getPartsForOrder(orderId);


STORY 7: Billing & Payment Processing (Team Member 7)

EPIC: Billing
PRIORITY: High
ASSIGNED TO: Member 7

DESCRIPTION:
As a receptionist, I need to calculate bills and process payments
so that customers can pay for their vehicle service.

ACCEPTANCE CRITERIA:
- Calculate total bill from parts used in service order
- Process payment (Cash or Card)
- View payment history for each order
- View total revenue across all payments
- Strategy pattern for different payment methods
- Prevent payment on orders with no parts

TECHNICAL DETAILS:
- File: vehicle_service_center.hpp
- Classes: PaymentStrategy (abstract), CashPayment, CardPayment, BillingService
- Pattern: Strategy pattern for payment methods
- Database: payments table
- Calculation: SUM(part_price * quantity) from service_order_parts

KEY CODE:
    PaymentStrategy::processPayment(orderId, amount);
    CashPayment: inserts with 'Cash' method
    CardPayment: inserts with 'Card' method
    BillingService::calculateTotalBill(orderId);
    BillingService::processPayment(orderId, amount, method);
    BillingService::getTotalRevenue();


STORY 8: GUI Integration & Main Application (Team Member 1 - You) — COMPLETED

EPIC: Presentation Layer
PRIORITY: Critical
ASSIGNED TO: You (Team Leader / Integrator)

DESCRIPTION:
As a user, I need a professional graphical interface so that I can
interact with all system features through an intuitive UI.

ACCEPTANCE CRITERIA:
- Login page with username/password authentication
- Role-based dashboard (different tabs per role)
- Customers & Vehicles panel: register, search, view tables
- Appointments panel: schedule, update status, filter
- Work Orders panel: create, assign mechanic, update status
- Inventory panel: add parts, update stock, log usage
- Billing panel: calculate bill, process payment
- Admin panel: create users, view reports
- Professional dark theme with color-coded buttons
- Error handling with user-friendly messages
- Real-time table refresh after database operations
- Scrollable tables with fixed maximum height
- Clear input fields with good contrast

TECHNICAL DETAILS:
- Files: main.cpp, mainwindow.cpp, mainwindow.h
- Framework: Qt6 Widgets
- Design: QStackedWidget for page navigation
- Styling: QSS (Qt Stylesheets) for dark theme
- Patterns: Signal/Slot for event handling
- Lambda functions for button click handlers

KEY CODE:
    MainWindow::setupLoginUI();
    MainWindow::setupDashboardUI();
    MainWindow::configureRoleDashboard();
    MainWindow::handleLogin();
    MainWindow::handleLogout();
    MainWindow::createCustomersPanel();
    MainWindow::createAppointmentsPanel();
    MainWindow::createServiceOrdersPanel();
    MainWindow::createInventoryPanel();
    MainWindow::createBillingPanel();
    MainWindow::createAdminPanel();
    MainWindow::refreshCustomersTable();
    MainWindow::refreshVehiclesTable();
    MainWindow::refreshAppointmentsTable();
    MainWindow::refreshServiceOrdersTable();
    MainWindow::refreshPartsTable();
    MainWindow::refreshUsersTable();
    MainWindow::populateCustomerCombo();
    MainWindow::populateVehicleCombo();
    MainWindow::populateMechanicCombo();
    MainWindow::populatePartCombo();
    MainWindow::populateOrderCombo();


## TEAM MEMBER SUMMARY


MEMBER 1 (YOU - LEADER):
- DatabaseManager (Singleton connection) — COMPLETED
- Complete GUI (all panels, login, dashboard) — COMPLETED
- Project integration and architecture — COMPLETED

MEMBER 2:
- User hierarchy (Admin, Receptionist, Mechanic)
- UserFactory pattern
- Authentication system
- Role-based permissions

MEMBER 3:
- CustomerRepository (add, search, list)
- VehicleRepository (add, list with JOIN)
- Customer-Vehicle relationship

MEMBER 4:
- AppointmentRepository (create, update status)
- Status filtering
- Date/time scheduling

MEMBER 5:
- ServiceOrderManager (create, update status)
- Observer pattern (AuditLogger)
- Mechanic assignment

MEMBER 6:
- InventoryRepository (add parts, stock management)
- Part usage logging
- Stock deduction on repair

MEMBER 7:
- BillingService (calculate bill)
- PaymentStrategy pattern (Cash, Card)
- Revenue reporting


## DESIGN PATTERNS USED


1. SINGLETON PATTERN
   - DatabaseManager: ensures single DB connection instance

2. FACTORY PATTERN
   - UserFactory: creates appropriate user type based on role

3. OBSERVER PATTERN
   - ServiceOrderObserver: notifies on status changes
   - AuditLogger: concrete observer for logging

4. STRATEGY PATTERN
   - PaymentStrategy: interchangeable payment methods
   - CashPayment, CardPayment: concrete strategies

5. REPOSITORY PATTERN
   - CustomerRepository, VehicleRepository, etc.
   - Encapsulates data access logic


## DATABASE SCHEMA


TABLES:
- users (id, username, password_hash, full_name, role)
- customers (id, name, phone, email)
- vehicles (id, customer_id, license_plate, make, model, year)
- appointments (id, vehicle_id, customer_id, scheduled_date, status)
- service_orders (id, appointment_id, mechanic_id, status, notes)
- parts (id, name, part_number, unit_price, stock_quantity)
- service_order_parts (id, service_order_id, part_id, quantity)
- payments (id, service_order_id, amount, payment_method, payment_date)

RELATIONSHIPS:
- customers 1:N vehicles
- vehicles 1:N appointments
- appointments 1:N service_orders
- users 1:N service_orders (mechanic)
- service_orders N:M parts (via service_order_parts)
- service_orders 1:N payments


## HOW TO PRESENT (VIDEO GUIDE)


EACH TEAM MEMBER SHOULD:

1. Show their code section in vehicle_service_center.hpp
2. Explain the design pattern they used
3. Demonstrate the feature working in the GUI
4. Show the database table their code interacts with
5. Explain error handling and edge cases

SUGGESTED VIDEO STRUCTURE (2-3 minutes per member):
- Introduction: "I'm Member X, I worked on [feature]"
- Code walkthrough: Show key classes and methods
- Pattern explanation: Why this pattern was chosen
- Live demo: Show the feature working in the application
- Database: Show the table structure and sample data
- Conclusion: How this integrates with the rest of the system


                         END OF DOCUMENT

