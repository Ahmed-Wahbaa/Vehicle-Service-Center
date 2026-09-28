-- ============================================================================
-- 1. DATABASE SCHEMA CREATION
-- ============================================================================

-- Drop tables if they exist (Clean slate)
DROP TABLE IF EXISTS payments CASCADE;
DROP TABLE IF EXISTS service_order_parts CASCADE;
DROP TABLE IF EXISTS parts CASCADE;
DROP TABLE IF EXISTS service_orders CASCADE;
DROP TABLE IF EXISTS appointments CASCADE;
DROP TABLE IF EXISTS vehicles CASCADE;
DROP TABLE IF EXISTS customers CASCADE;
DROP TABLE IF EXISTS users CASCADE;

-- Users Table (Member 2)
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    full_name VARCHAR(100) NOT NULL,
    role VARCHAR(20) CHECK (role IN ('Admin', 'Receptionist', 'Mechanic')) NOT NULL
);

-- Customers Table (Member 3)
CREATE TABLE customers (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(20) UNIQUE NOT NULL,
    email VARCHAR(100)
);

-- Vehicles Table (Member 3)
CREATE TABLE vehicles (
    id SERIAL PRIMARY KEY,
    customer_id INT REFERENCES customers(id) ON DELETE CASCADE,
    license_plate VARCHAR(20) UNIQUE NOT NULL,
    make VARCHAR(50) NOT NULL,
    model VARCHAR(50) NOT NULL,
    year INT NOT NULL
);

-- Appointments Table (Member 4)
CREATE TABLE appointments (
    id SERIAL PRIMARY KEY,
    vehicle_id INT REFERENCES vehicles(id) ON DELETE CASCADE,
    customer_id INT REFERENCES customers(id) ON DELETE CASCADE,
    scheduled_date TIMESTAMP NOT NULL,
    status VARCHAR(20) DEFAULT 'Pending' CHECK (status IN ('Pending', 'Confirmed', 'Cancelled', 'Completed'))
);

-- Service Orders Table (Member 5)
CREATE TABLE service_orders (
    id SERIAL PRIMARY KEY,
    appointment_id INT REFERENCES appointments(id) ON DELETE CASCADE,
    mechanic_id INT REFERENCES users(id),
    status VARCHAR(20) DEFAULT 'Pending' CHECK (status IN ('Pending', 'In Progress', 'Completed')),
    creation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    notes TEXT
);

-- Parts Inventory Table (Member 6)
CREATE TABLE parts (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    part_number VARCHAR(50) UNIQUE NOT NULL,
    unit_price NUMERIC(10, 2) NOT NULL CHECK (unit_price >= 0),
    stock_quantity INT NOT NULL CHECK (stock_quantity >= 0)
);

-- Service Order Parts Link Table (Member 6)
CREATE TABLE service_order_parts (
    id SERIAL PRIMARY KEY,
    service_order_id INT REFERENCES service_orders(id) ON DELETE CASCADE,
    part_id INT REFERENCES parts(id) ON DELETE CASCADE,
    quantity INT NOT NULL CHECK (quantity > 0)
);

-- Payments Table (Member 7)
CREATE TABLE payments (
    id SERIAL PRIMARY KEY,
    service_order_id INT REFERENCES service_orders(id) ON DELETE CASCADE,
    amount NUMERIC(10, 2) NOT NULL,
    payment_method VARCHAR(20) CHECK (payment_method IN ('Cash', 'Card')),
    payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- 2. MOCK DATA INSERTION
-- ============================================================================

-- Users (Password is 'password123' for test accounts)
INSERT INTO users (username, password_hash, full_name, role) VALUES
('admin_user', 'password123', 'Alice Smith (Admin)', 'Admin'),
('reception_user', 'password123', 'Bob Jones (Receptionist)', 'Receptionist'),
('mechanic_john', 'password123', 'John Doe (Mechanic)', 'Mechanic'),
('mechanic_mike', 'password123', 'Mike Ross (Mechanic)', 'Mechanic');

-- Customers
INSERT INTO customers (name, phone, email) VALUES
('Samy Ahmed', '+201001112233', 'samy@example.com'),
('Mona Zaki', '+201004445566', 'mona@example.com'),
('Khaled Omar', '+201007778899', 'khaled@example.com');

-- Vehicles
INSERT INTO vehicles (customer_id, license_plate, make, model, year) VALUES
(1, 'ABC-1234', 'Toyota', 'Corolla', 2020),
(2, 'XYZ-9876', 'Hyundai', 'Elantra', 2022),
(3, 'LMN-5555', 'BMW', '320i', 2021),
(1, 'EGY-7777', 'Nissan', 'Sunny', 2019);

-- Parts Inventory
INSERT INTO parts (name, part_number, unit_price, stock_quantity) VALUES
('Synthetic Engine Oil 5W-30 (1L)', 'OIL-5W30', 25.00, 100),
('Oil Filter', 'FLT-OIL-01', 12.50, 50),
('Front Brake Pads Set', 'BRK-PAD-F', 85.00, 30),
('Rear Brake Disc', 'BRK-DSC-R', 110.00, 20),
('Air Filter', 'FLT-AIR-02', 18.00, 40),
('Spark Plug (Set of 4)', 'SPK-PLG-04', 45.00, 25),
('Car Battery 12V 60Ah', 'BAT-12V60', 130.00, 15);

-- Appointments
INSERT INTO appointments (vehicle_id, customer_id, scheduled_date, status) VALUES
(1, 1, '2026-10-01 10:00:00', 'Confirmed'),
(2, 2, '2026-10-01 11:30:00', 'Pending'),
(3, 3, '2026-10-02 09:00:00', 'Confirmed');

-- Service Orders
INSERT INTO service_orders (appointment_id, mechanic_id, status, notes) VALUES
(1, 3, 'In Progress', 'Customer requested full oil change and brake inspection.');