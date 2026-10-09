#include "mainwindow.h"

// ============================================================================
// STYLESHEET CONSTANTS
// ============================================================================
static const char* LOGIN_STYLE = R"(
    QFrame#loginFrame {
        background-color: #1e293b;
        border-radius: 16px;
        border: 1px solid #334155;
    }
    QLabel#loginTitle {
        color: #f1f5f9;
        font-size: 28px;
        font-weight: bold;
        padding: 10px;
    }
    QLabel#loginSubtitle {
        color: #94a3b8;
        font-size: 13px;
        padding-bottom: 10px;
    }
    QLabel#fieldLabel {
        color: #cbd5e1;
        font-size: 13px;
        font-weight: bold;
        padding-top: 8px;
    }
    QLineEdit#fieldInput {
        background-color: #0f172a;
        border: 2px solid #334155;
        border-radius: 8px;
        padding: 10px 14px;
        color: #f1f5f9;
        font-size: 14px;
        selection-background-color: #3b82f6;
    }
    QLineEdit#fieldInput:focus {
        border: 2px solid #3b82f6;
    }
    QLineEdit#fieldInput:hover {
        border: 2px solid #475569;
    }
    QPushButton#loginBtn {
        background-color: #3b82f6;
        color: white;
        border: none;
        border-radius: 8px;
        padding: 12px;
        font-size: 15px;
        font-weight: bold;
        margin-top: 16px;
    }
    QPushButton#loginBtn:hover {
        background-color: #2563eb;
    }
    QPushButton#loginBtn:pressed {
        background-color: #1d4ed8;
    }
    QLabel#hintLabel {
        color: #64748b;
        font-size: 11px;
        padding-top: 12px;
    }
)";

static const char* DASHBOARD_STYLE = R"(
    QFrame#headerFrame {
        background-color: #1e293b;
        border-radius: 10px;
        border: 1px solid #334155;
    }
    QLabel#welcomeLabel {
        color: #f1f5f9;
        font-size: 15px;
        font-weight: bold;
    }
    QPushButton#logoutBtn {
        background-color: #ef4444;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 8px 20px;
        font-size: 13px;
        font-weight: bold;
    }
    QPushButton#logoutBtn:hover {
        background-color: #dc2626;
    }
    QPushButton#logoutBtn:pressed {
        background-color: #b91c1c;
    }
    QTabWidget::pane {
        border: 1px solid #334155;
        border-radius: 8px;
        background-color: #0f172a;
    }
    QTabBar::tab {
        background-color: #1e293b;
        color: #94a3b8;
        padding: 10px 24px;
        margin-right: 4px;
        border-top-left-radius: 6px;
        border-top-right-radius: 6px;
        font-size: 13px;
        font-weight: bold;
    }
    QTabBar::tab:selected {
        background-color: #3b82f6;
        color: white;
    }
    QTabBar::tab:hover:!selected {
        background-color: #334155;
        color: #e2e8f0;
    }
    QGroupBox {
        background-color: #1e293b;
        border: 1px solid #334155;
        border-radius: 10px;
        margin-top: 12px;
        padding-top: 12px;
        font-size: 14px;
        font-weight: bold;
        color: #f1f5f9;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 16px;
        padding: 0 8px;
        color: #3b82f6;
    }
    QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit, QDateTimeEdit {
        background-color: #475569;
        border: 1px solid #94a3b8;
        border-radius: 6px;
        padding: 8px 12px;
        color: #ffffff;
        font-size: 14px;
        font-weight: 500;
        selection-background-color: #3b82f6;
    }
    QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus, QTextEdit:focus, QDateTimeEdit:focus {
        border: 2px solid #3b82f6;
    }
    QLineEdit:hover, QComboBox:hover, QSpinBox:hover, QDoubleSpinBox:hover, QTextEdit:hover, QDateTimeEdit:hover {
        border: 1px solid #cbd5e1;
    }
    QLineEdit::placeholder, QTextEdit::placeholder {
        color: #e2e8f0;
    }
    QComboBox::drop-down {
        border: none;
        width: 30px;
    }
    QComboBox QAbstractItemView {
        background-color: #1e293b;
        color: #f1f5f9;
        selection-background-color: #3b82f6;
        border: 1px solid #334155;
    }
    QPushButton {
        background-color: #3b82f6;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 8px 16px;
        font-size: 13px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #2563eb;
    }
    QPushButton:pressed {
        background-color: #1d4ed8;
    }
    QPushButton#dangerBtn {
        background-color: #ef4444;
    }
    QPushButton#dangerBtn:hover {
        background-color: #dc2626;
    }
    QPushButton#successBtn {
        background-color: #22c55e;
    }
    QPushButton#successBtn:hover {
        background-color: #16a34a;
    }
    QPushButton#warningBtn {
        background-color: #f59e0b;
    }
    QPushButton#warningBtn:hover {
        background-color: #d97706;
    }
    QPushButton#purpleBtn {
        background-color: #8b5cf6;
    }
    QPushButton#purpleBtn:hover {
        background-color: #7c3aed;
    }
    QTableWidget {
        background-color: #0f172a;
        border: 1px solid #334155;
        border-radius: 8px;
        gridline-color: #1e293b;
        color: #e2e8f0;
        font-size: 13px;
        alternate-background-color: #1e293b;
    }
    QTableWidget::item {
        padding: 10px 8px;
        border-bottom: 1px solid #1e293b;
    }
    QTableWidget::item:selected {
        background-color: #3b82f6;
        color: white;
    }
    QHeaderView::section {
        background-color: #1e293b;
        color: #94a3b8;
        padding: 10px;
        border: none;
        border-bottom: 2px solid #334155;
        font-size: 12px;
        font-weight: bold;
    }
    QHeaderView::section:hover {
        background-color: #334155;
    }
    QScrollBar:vertical {
        background-color: #0f172a;
        width: 12px;
        border-radius: 6px;
        margin: 0;
    }
    QScrollBar::handle:vertical {
        background-color: #475569;
        border-radius: 6px;
        min-height: 30px;
    }
    QScrollBar::handle:vertical:hover {
        background-color: #64748b;
    }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        height: 0;
    }
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
        background: none;
    }
    QScrollBar:horizontal {
        background-color: #0f172a;
        height: 12px;
        border-radius: 6px;
        margin: 0;
    }
    QScrollBar::handle:horizontal {
        background-color: #475569;
        border-radius: 6px;
        min-width: 30px;
    }
    QScrollBar::handle:horizontal:hover {
        background-color: #64748b;
    }
    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
        width: 0;
    }
    QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
        background: none;
    }
    QLabel {
        color: #cbd5e1;
        font-size: 13px;
    }
    QFrame#separator {
        background-color: #334155;
        max-height: 1px;
        min-height: 1px;
    }
    QScrollArea {
        border: none;
        background-color: transparent;
    }
    QWidget#panelContainer {
        background-color: #0f172a;
    }
)";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Vehicle Service Center System");
    resize(1200, 800);

    mainStack = new QStackedWidget(this);
    setCentralWidget(mainStack);

    setupLoginUI();
    setupDashboardUI();

    mainStack->addWidget(loginPage);
    mainStack->addWidget(dashboardPage);
    mainStack->setCurrentWidget(loginPage);
}

void MainWindow::clearLayout(QLayout* layout) {
    if (!layout) return;
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        if (item->layout()) clearLayout(item->layout());
        delete item;
    }
}

// ============================================================================
// LOGIN PAGE
// ============================================================================
void MainWindow::setupLoginUI() {
    loginPage = new QWidget();
    loginPage->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* layout = new QVBoxLayout(loginPage);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(40, 40, 40, 40);

    QFrame* loginFrame = new QFrame();
    loginFrame->setObjectName("loginFrame");
    loginFrame->setFixedWidth(420);
    loginFrame->setStyleSheet(LOGIN_STYLE);

    QVBoxLayout* frameLayout = new QVBoxLayout(loginFrame);
    frameLayout->setContentsMargins(40, 40, 40, 40);
    frameLayout->setSpacing(4);

    // Icon/Logo area
    QLabel* iconLabel = new QLabel();
    iconLabel->setFixedSize(64, 64);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("background-color: #3b82f6; border-radius: 32px; color: white; font-size: 28px; font-weight: bold;");
    iconLabel->setText("VS");
    iconLabel->setObjectName("iconLabel");

    QLabel* title = new QLabel("Vehicle Service Center");
    title->setObjectName("loginTitle");
    title->setAlignment(Qt::AlignCenter);

    QLabel* subtitle = new QLabel("Management System");
    subtitle->setObjectName("loginSubtitle");
    subtitle->setAlignment(Qt::AlignCenter);

    // Username
    QLabel* userLabel = new QLabel("Username");
    userLabel->setObjectName("fieldLabel");
    userEdit = new QLineEdit();
    userEdit->setObjectName("fieldInput");
    userEdit->setPlaceholderText("Enter your username");
    userEdit->setMinimumHeight(40);

    // Password
    QLabel* passLabel = new QLabel("Password");
    passLabel->setObjectName("fieldLabel");
    passEdit = new QLineEdit();
    passEdit->setObjectName("fieldInput");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setPlaceholderText("Enter your password");
    passEdit->setMinimumHeight(40);

    // Login button
    QPushButton* loginBtn = new QPushButton("Sign In");
    loginBtn->setObjectName("loginBtn");
    loginBtn->setMinimumHeight(44);
    loginBtn->setCursor(Qt::PointingHandCursor);

    // Hint
    QLabel* hintLabel = new QLabel("Default accounts: admin_user / reception_user / mechanic_john\nPassword: password123");
    hintLabel->setObjectName("hintLabel");
    hintLabel->setAlignment(Qt::AlignCenter);

    // Assemble
    frameLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    frameLayout->addWidget(title);
    frameLayout->addWidget(subtitle);
    frameLayout->addSpacing(16);
    frameLayout->addWidget(userLabel);
    frameLayout->addWidget(userEdit);
    frameLayout->addSpacing(8);
    frameLayout->addWidget(passLabel);
    frameLayout->addWidget(passEdit);
    frameLayout->addWidget(loginBtn);
    frameLayout->addWidget(hintLabel);
    frameLayout->addStretch();

    layout->addWidget(loginFrame, 0, Qt::AlignCenter);

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::handleLogin);
}

// ============================================================================
// DASHBOARD PAGE
// ============================================================================
void MainWindow::setupDashboardUI() {
    dashboardPage = new QWidget();
    dashboardPage->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* layout = new QVBoxLayout(dashboardPage);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // Header
    QFrame* headerFrame = new QFrame();
    headerFrame->setObjectName("headerFrame");
    headerFrame->setStyleSheet(DASHBOARD_STYLE);
    headerFrame->setFixedHeight(60);

    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 10, 20, 10);

    welcomeLabel = new QLabel();
    welcomeLabel->setObjectName("welcomeLabel");
    welcomeLabel->setStyleSheet("color: #f1f5f9; font-size: 15px; font-weight: bold;");

    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("logoutBtn");
    logoutBtn->setCursor(Qt::PointingHandCursor);

    headerLayout->addWidget(welcomeLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(logoutBtn);

    // Tabs
    roleTabs = new QTabWidget();
    roleTabs->setStyleSheet(DASHBOARD_STYLE);

    layout->addWidget(headerFrame);
    layout->addWidget(roleTabs);

    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);
}

void MainWindow::handleLogin() {

    std::string username = userEdit->text().trimmed().toStdString();
    std::string password = passEdit->text().toStdString();

    if (username.empty() || password.empty()) {
        showError("Login Error", "Please enter both username and password.");
        return;
    }

    UserRepository repo;
    currentUser = repo.authenticate(username, password);

    if (!currentUser) {
        showError("Login Failed", "Invalid username or password!");
        return;
    }

    userEdit->clear();
    passEdit->clear();
    configureRoleDashboard();
    mainStack->setCurrentWidget(dashboardPage);
}

void MainWindow::handleLogout() {
    currentUser.reset();
    mainStack->setCurrentWidget(loginPage);
}

void MainWindow::configureRoleDashboard() {
    roleTabs->clear();
    welcomeLabel->setText(QString("Welcome, %1  |  Role: %2")
                          .arg(QString::fromStdString(currentUser->getFullName()))
                          .arg(QString::fromStdString(currentUser->getRole())));

    if (currentUser->canManageCustomers()) {
        roleTabs->addTab(createCustomersPanel(), "Customers & Vehicles");
    }
    if (currentUser->canManageAppointments()) {
        roleTabs->addTab(createAppointmentsPanel(), "Appointments");
    }
    if (currentUser->canUpdateServiceOrders()) {
        roleTabs->addTab(createServiceOrdersPanel(), "Work Orders");
    }
    if (currentUser->canManageInventory()) {
        roleTabs->addTab(createInventoryPanel(), "Inventory");
    }
    if (currentUser->canProcessBilling()) {
        roleTabs->addTab(createBillingPanel(), "Billing");
    }
    if (currentUser->canManageUsers()) {
        roleTabs->addTab(createAdminPanel(), "Admin Panel");
    }
}

// ============================================================================
// CUSTOMERS & VEHICLES PANEL
// ============================================================================
QWidget* MainWindow::createCustomersPanel() {
    QWidget* panel = new QWidget();
    panel->setObjectName("panelContainer");
    panel->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // --- Customer Registration ---
    QGroupBox* custGroup = new QGroupBox("Register New Customer");
    custGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* custForm = new QFormLayout(custGroup);
    custForm->setSpacing(8);

    QLineEdit* custNameEdit = new QLineEdit();
    custNameEdit->setPlaceholderText("Full Name");
    QLineEdit* custPhoneEdit = new QLineEdit();
    custPhoneEdit->setPlaceholderText("Phone Number");
    QLineEdit* custEmailEdit = new QLineEdit();
    custEmailEdit->setPlaceholderText("Email (optional)");

    QPushButton* addCustBtn = new QPushButton("Add Customer");
    addCustBtn->setObjectName("successBtn");
    addCustBtn->setMinimumHeight(36);

    custForm->addRow("Name:", custNameEdit);
    custForm->addRow("Phone:", custPhoneEdit);
    custForm->addRow("Email:", custEmailEdit);
    custForm->addRow("", addCustBtn);

    // --- Vehicle Registration ---
    QGroupBox* vehGroup = new QGroupBox("Register New Vehicle");
    vehGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* vehForm = new QFormLayout(vehGroup);
    vehForm->setSpacing(8);

    QComboBox* vehCustomerCombo = new QComboBox();
    populateCustomerCombo(vehCustomerCombo);

    QLineEdit* vehPlateEdit = new QLineEdit();
    vehPlateEdit->setPlaceholderText("License Plate");
    QLineEdit* vehMakeEdit = new QLineEdit();
    vehMakeEdit->setPlaceholderText("Make (e.g., Toyota)");
    QLineEdit* vehModelEdit = new QLineEdit();
    vehModelEdit->setPlaceholderText("Model (e.g., Corolla)");
    QSpinBox* vehYearSpin = new QSpinBox();
    vehYearSpin->setRange(1900, 2030);
    vehYearSpin->setValue(2024);

    QPushButton* addVehBtn = new QPushButton("Add Vehicle");
    addVehBtn->setObjectName("successBtn");
    addVehBtn->setMinimumHeight(36);

    vehForm->addRow("Customer:", vehCustomerCombo);
    vehForm->addRow("License Plate:", vehPlateEdit);
    vehForm->addRow("Make:", vehMakeEdit);
    vehForm->addRow("Model:", vehModelEdit);
    vehForm->addRow("Year:", vehYearSpin);
    vehForm->addRow("", addVehBtn);

    // --- Customers Table ---
    QGroupBox* custTableGroup = new QGroupBox("Registered Customers");
    custTableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* custTableLayout = new QVBoxLayout(custTableGroup);
    custTableLayout->setSpacing(8);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLineEdit* custSearchEdit = new QLineEdit();
    custSearchEdit->setPlaceholderText("Search by name, phone, or email...");
    QPushButton* custSearchBtn = new QPushButton("Search");
    QPushButton* custRefreshBtn = new QPushButton("Refresh");
    searchLayout->addWidget(custSearchEdit, 1);
    searchLayout->addWidget(custSearchBtn);
    searchLayout->addWidget(custRefreshBtn);

    QTableWidget* custTable = new QTableWidget();
    custTable->setColumnCount(4);
    custTable->setHorizontalHeaderLabels({"ID", "Name", "Phone", "Email"});
    custTable->horizontalHeader()->setStretchLastSection(true);
    custTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    custTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    custTable->setAlternatingRowColors(true);
    custTable->setMaximumHeight(220);
    custTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    custTableLayout->addLayout(searchLayout);
    custTableLayout->addWidget(custTable, 1);

    // --- Vehicles Table ---
    QGroupBox* vehTableGroup = new QGroupBox("Registered Vehicles");
    vehTableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* vehTableLayout = new QVBoxLayout(vehTableGroup);
    vehTableLayout->setSpacing(8);

    QPushButton* vehRefreshBtn = new QPushButton("Refresh Vehicles");

    QTableWidget* vehTable = new QTableWidget();
    vehTable->setColumnCount(6);
    vehTable->setHorizontalHeaderLabels({"ID", "Plate", "Make", "Model", "Year", "Customer"});
    vehTable->horizontalHeader()->setStretchLastSection(true);
    vehTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    vehTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vehTable->setAlternatingRowColors(true);
    vehTable->setMaximumHeight(220);
    vehTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    vehTableLayout->addWidget(vehRefreshBtn);
    vehTableLayout->addWidget(vehTable, 1);

    // --- Layout ---
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(custGroup);
    topLayout->addWidget(vehGroup);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(custTableGroup);
    mainLayout->addWidget(vehTableGroup, 1);

    // --- Connections ---
    connect(addCustBtn, &QPushButton::clicked, this, [this, custNameEdit, custPhoneEdit, custEmailEdit, custTable, vehCustomerCombo]() {
        std::string name = custNameEdit->text().toStdString();
        std::string phone = custPhoneEdit->text().toStdString();
        std::string email = custEmailEdit->text().toStdString();
        if (name.empty() || phone.empty()) { showError("Validation Error", "Name and Phone are required."); return; }
        CustomerRepository repo;
        if (repo.addCustomer(name, phone, email)) {
            showInfo("Success", "Customer added successfully!");
            custNameEdit->clear(); custPhoneEdit->clear(); custEmailEdit->clear();
            refreshCustomersTable(custTable);
            populateCustomerCombo(vehCustomerCombo);
        } else { showError("Error", "Failed to add customer. Phone may already exist."); }
    });

    connect(addVehBtn, &QPushButton::clicked, this, [this, vehCustomerCombo, vehPlateEdit, vehMakeEdit, vehModelEdit, vehYearSpin, vehTable]() {
        int customerId = vehCustomerCombo->currentData().toInt();
        std::string plate = vehPlateEdit->text().toStdString();
        std::string make = vehMakeEdit->text().toStdString();
        std::string model = vehModelEdit->text().toStdString();
        int year = vehYearSpin->value();
        if (customerId <= 0 || plate.empty() || make.empty() || model.empty()) { showError("Validation Error", "All vehicle fields are required."); return; }
        VehicleRepository repo;
        if (repo.addVehicle(customerId, plate, make, model, year)) {
            showInfo("Success", "Vehicle added successfully!");
            vehPlateEdit->clear(); vehMakeEdit->clear(); vehModelEdit->clear();
            refreshVehiclesTable(vehTable);
        } else { showError("Error", "Failed to add vehicle. License plate may already exist."); }
    });

    connect(custSearchBtn, &QPushButton::clicked, this, [this, custSearchEdit, custTable]() {
        std::string query = custSearchEdit->text().toStdString();
        CustomerRepository repo;
        auto customers = repo.searchCustomers(query);
        custTable->setRowCount(customers.size());
        for (size_t i = 0; i < customers.size(); i++) {
            custTable->setItem(i, 0, new QTableWidgetItem(QString::number(customers[i].id)));
            custTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(customers[i].name)));
            custTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(customers[i].phone)));
            custTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(customers[i].email)));
        }
    });

    connect(custRefreshBtn, &QPushButton::clicked, this, [this, custTable, vehCustomerCombo]() {
        refreshCustomersTable(custTable);
        populateCustomerCombo(vehCustomerCombo);
    });

    connect(vehRefreshBtn, &QPushButton::clicked, this, [this, vehTable]() {
        refreshVehiclesTable(vehTable);
    });

    refreshCustomersTable(custTable);
    refreshVehiclesTable(vehTable);
    return panel;
}

// ============================================================================
// APPOINTMENTS PANEL
// ============================================================================
QWidget* MainWindow::createAppointmentsPanel() {
    QWidget* panel = new QWidget();
    panel->setObjectName("panelContainer");
    panel->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    QGroupBox* schedGroup = new QGroupBox("Schedule New Appointment");
    schedGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* schedForm = new QFormLayout(schedGroup);
    schedForm->setSpacing(8);

    QComboBox* apptVehicleCombo = new QComboBox();
    populateVehicleCombo(apptVehicleCombo);
    QComboBox* apptCustomerCombo = new QComboBox();
    populateCustomerCombo(apptCustomerCombo);
    QDateTimeEdit* apptDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    apptDateTimeEdit->setCalendarPopup(true);
    apptDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");

    QPushButton* schedBtn = new QPushButton("Schedule Appointment");
    schedBtn->setObjectName("successBtn");
    schedBtn->setMinimumHeight(36);

    schedForm->addRow("Vehicle:", apptVehicleCombo);
    schedForm->addRow("Customer:", apptCustomerCombo);
    schedForm->addRow("Date & Time:", apptDateTimeEdit);
    schedForm->addRow("", schedBtn);

    QGroupBox* statusGroup = new QGroupBox("Update Appointment Status");
    statusGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* statusForm = new QFormLayout(statusGroup);
    statusForm->setSpacing(8);

    QComboBox* apptStatusCombo = new QComboBox();
    apptStatusCombo->addItems({"Pending", "Confirmed", "Cancelled", "Completed"});
    QPushButton* updateStatusBtn = new QPushButton("Update Status");
    updateStatusBtn->setObjectName("warningBtn");
    updateStatusBtn->setMinimumHeight(36);

    statusForm->addRow("New Status:", apptStatusCombo);
    statusForm->addRow("", updateStatusBtn);

    QGroupBox* tableGroup = new QGroupBox("All Appointments");
    tableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setSpacing(8);

    QHBoxLayout* filterLayout = new QHBoxLayout();
    QComboBox* filterCombo = new QComboBox();
    filterCombo->addItems({"All", "Pending", "Confirmed", "Cancelled", "Completed"});
    QPushButton* filterBtn = new QPushButton("Filter");
    QPushButton* apptRefreshBtn = new QPushButton("Refresh");
    filterLayout->addWidget(new QLabel("Status:"));
    filterLayout->addWidget(filterCombo);
    filterLayout->addWidget(filterBtn);
    filterLayout->addWidget(apptRefreshBtn);
    filterLayout->addStretch();

    QTableWidget* apptTable = new QTableWidget();
    apptTable->setColumnCount(7);
    apptTable->setHorizontalHeaderLabels({"ID", "Vehicle", "Customer", "Date & Time", "Status", "Plate", "Customer Name"});
    apptTable->horizontalHeader()->setStretchLastSection(true);
    apptTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    apptTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableLayout->addLayout(filterLayout);
    tableLayout->addWidget(apptTable, 1);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(schedGroup);
    topLayout->addWidget(statusGroup);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tableGroup, 1);

    connect(schedBtn, &QPushButton::clicked, this, [this, apptVehicleCombo, apptCustomerCombo, apptDateTimeEdit, apptTable]() {
        int vehicleId = apptVehicleCombo->currentData().toInt();
        int customerId = apptCustomerCombo->currentData().toInt();
        std::string dateStr = apptDateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
        if (vehicleId <= 0 || customerId <= 0) { showError("Validation Error", "Please select a vehicle and customer."); return; }
        AppointmentRepository repo;
        if (repo.createAppointment(vehicleId, customerId, dateStr)) {
            showInfo("Success", "Appointment scheduled successfully!");
            refreshAppointmentsTable(apptTable);
        } else { showError("Error", "Failed to schedule appointment."); }
    });

    connect(updateStatusBtn, &QPushButton::clicked, this, [this, apptTable, apptStatusCombo]() {
        int row = apptTable->currentRow();
        if (row < 0) { showError("Selection Error", "Please select an appointment from the table."); return; }
        int apptId = apptTable->item(row, 0)->text().toInt();
        std::string status = apptStatusCombo->currentText().toStdString();
        AppointmentRepository repo;
        if (repo.updateAppointmentStatus(apptId, status)) {
            showInfo("Success", "Appointment status updated!");
            refreshAppointmentsTable(apptTable);
        } else { showError("Error", "Failed to update appointment status."); }
    });

    connect(filterBtn, &QPushButton::clicked, this, [this, filterCombo, apptTable]() {
        std::string status = filterCombo->currentText().toStdString();
        AppointmentRepository repo;
        auto appointments = (status == "All") ? repo.getAllAppointments() : repo.getAppointmentsByStatus(status);
        apptTable->setRowCount(appointments.size());
        for (size_t i = 0; i < appointments.size(); i++) {
            apptTable->setItem(i, 0, new QTableWidgetItem(QString::number(appointments[i].id)));
            apptTable->setItem(i, 1, new QTableWidgetItem(QString::number(appointments[i].vehicleId)));
            apptTable->setItem(i, 2, new QTableWidgetItem(QString::number(appointments[i].customerId)));
            apptTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(appointments[i].scheduledDate)));
            apptTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(appointments[i].status)));
            apptTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(appointments[i].vehiclePlate)));
            apptTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(appointments[i].customerName)));
        }
    });

    connect(apptRefreshBtn, &QPushButton::clicked, this, [this, apptTable]() { refreshAppointmentsTable(apptTable); });

    refreshAppointmentsTable(apptTable);
    return panel;
}

// ============================================================================
// SERVICE ORDERS PANEL
// ============================================================================
QWidget* MainWindow::createServiceOrdersPanel() {
    QWidget* panel = new QWidget();
    panel->setObjectName("panelContainer");
    panel->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    QGroupBox* createGroup = new QGroupBox("Create Service Order");
    createGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* createForm = new QFormLayout(createGroup);
    createForm->setSpacing(8);

    QComboBox* orderApptCombo = new QComboBox();
    AppointmentRepository apptRepo;
    auto appointments = apptRepo.getAppointmentsByStatus("Confirmed");
    for (const auto& a : appointments) {
        orderApptCombo->addItem(QString("#%1 - %2 - %3").arg(a.id).arg(QString::fromStdString(a.vehiclePlate)).arg(QString::fromStdString(a.customerName)), a.id);
    }
    QComboBox* orderMechanicCombo = new QComboBox();
    populateMechanicCombo(orderMechanicCombo);
    QTextEdit* orderNotesEdit = new QTextEdit();
    orderNotesEdit->setMaximumHeight(80);
    orderNotesEdit->setPlaceholderText("Enter service notes...");

    QPushButton* createOrderBtn = new QPushButton("Create Service Order");
    createOrderBtn->setObjectName("successBtn");
    createOrderBtn->setMinimumHeight(36);

    createForm->addRow("Appointment:", orderApptCombo);
    createForm->addRow("Assign Mechanic:", orderMechanicCombo);
    createForm->addRow("Notes:", orderNotesEdit);
    createForm->addRow("", createOrderBtn);

    QGroupBox* statusGroup = new QGroupBox("Update Order Status");
    statusGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* statusForm = new QFormLayout(statusGroup);
    statusForm->setSpacing(8);

    QComboBox* orderStatusCombo = new QComboBox();
    orderStatusCombo->addItems({"Pending", "In Progress", "Completed"});
    QPushButton* updateOrderStatusBtn = new QPushButton("Update Status");
    updateOrderStatusBtn->setObjectName("warningBtn");
    updateOrderStatusBtn->setMinimumHeight(36);

    statusForm->addRow("New Status:", orderStatusCombo);
    statusForm->addRow("", updateOrderStatusBtn);

    QGroupBox* tableGroup = new QGroupBox("Service Orders");
    tableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setSpacing(8);

    QHBoxLayout* filterLayout = new QHBoxLayout();
    QComboBox* orderFilterCombo = new QComboBox();
    orderFilterCombo->addItems({"All", "Pending", "In Progress", "Completed"});
    QPushButton* orderFilterBtn = new QPushButton("Filter");
    QPushButton* orderRefreshBtn = new QPushButton("Refresh");
    filterLayout->addWidget(new QLabel("Status:"));
    filterLayout->addWidget(orderFilterCombo);
    filterLayout->addWidget(orderFilterBtn);
    filterLayout->addWidget(orderRefreshBtn);
    filterLayout->addStretch();

    QTableWidget* ordersTable = new QTableWidget();
    ordersTable->setColumnCount(7);
    ordersTable->setHorizontalHeaderLabels({"ID", "Appointment", "Mechanic", "Status", "Created", "Notes", "Vehicle"});
    ordersTable->horizontalHeader()->setStretchLastSection(true);
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableLayout->addLayout(filterLayout);
    tableLayout->addWidget(ordersTable, 1);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(createGroup);
    topLayout->addWidget(statusGroup);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tableGroup, 1);

    connect(createOrderBtn, &QPushButton::clicked, this, [this, orderApptCombo, orderMechanicCombo, orderNotesEdit, ordersTable]() {
        int apptId = orderApptCombo->currentData().toInt();
        int mechId = orderMechanicCombo->currentData().toInt();
        std::string notes = orderNotesEdit->toPlainText().toStdString();
        if (apptId <= 0 || mechId <= 0) { showError("Validation Error", "Please select an appointment and mechanic."); return; }
        ServiceOrderManager mgr;
        if (mgr.createServiceOrder(apptId, mechId, notes)) {
            showInfo("Success", "Service order created successfully!");
            orderNotesEdit->clear();
            refreshServiceOrdersTable(ordersTable);
        } else { showError("Error", "Failed to create service order."); }
    });

    connect(updateOrderStatusBtn, &QPushButton::clicked, this, [this, ordersTable, orderStatusCombo]() {
        int row = ordersTable->currentRow();
        if (row < 0) { showError("Selection Error", "Please select a service order from the table."); return; }
        int orderId = ordersTable->item(row, 0)->text().toInt();
        std::string status = orderStatusCombo->currentText().toStdString();
        ServiceOrderManager mgr;
        mgr.updateOrderStatus(orderId, status);
        showInfo("Success", "Order status updated!");
        refreshServiceOrdersTable(ordersTable);
    });

    connect(orderFilterBtn, &QPushButton::clicked, this, [this, orderFilterCombo, ordersTable]() {
        std::string status = orderFilterCombo->currentText().toStdString();
        ServiceOrderManager mgr;
        auto orders = (status == "All") ? mgr.getAllServiceOrders() : mgr.getServiceOrdersByStatus(status);
        ordersTable->setRowCount(orders.size());
        for (size_t i = 0; i < orders.size(); i++) {
            ordersTable->setItem(i, 0, new QTableWidgetItem(QString::number(orders[i].id)));
            ordersTable->setItem(i, 1, new QTableWidgetItem(QString::number(orders[i].appointmentId)));
            ordersTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(orders[i].mechanicName)));
            ordersTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(orders[i].status)));
            ordersTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(orders[i].creationDate)));
            ordersTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(orders[i].notes)));
            ordersTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(orders[i].vehiclePlate)));
        }
    });

    connect(orderRefreshBtn, &QPushButton::clicked, this, [this, ordersTable]() { refreshServiceOrdersTable(ordersTable); });

    refreshServiceOrdersTable(ordersTable);
    return panel;
}

// ============================================================================
// INVENTORY PANEL
// ============================================================================
QWidget* MainWindow::createInventoryPanel() {
    QWidget* panel = new QWidget();
    panel->setObjectName("panelContainer");
    panel->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    QGroupBox* addPartGroup = new QGroupBox("Add New Spare Part");
    addPartGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* addPartForm = new QFormLayout(addPartGroup);
    addPartForm->setSpacing(8);

    QLineEdit* partNameEdit = new QLineEdit();
    partNameEdit->setPlaceholderText("Part Name");
    QLineEdit* partNumberEdit = new QLineEdit();
    partNumberEdit->setPlaceholderText("Part Number");
    QDoubleSpinBox* partPriceSpin = new QDoubleSpinBox();
    partPriceSpin->setRange(0, 10000);
    partPriceSpin->setDecimals(2);
    partPriceSpin->setPrefix("$ ");
    QSpinBox* partQtySpin = new QSpinBox();
    partQtySpin->setRange(0, 10000);

    QPushButton* addPartBtn = new QPushButton("Add Part");
    addPartBtn->setObjectName("successBtn");
    addPartBtn->setMinimumHeight(36);

    addPartForm->addRow("Name:", partNameEdit);
    addPartForm->addRow("Part Number:", partNumberEdit);
    addPartForm->addRow("Unit Price:", partPriceSpin);
    addPartForm->addRow("Stock Quantity:", partQtySpin);
    addPartForm->addRow("", addPartBtn);

    QGroupBox* stockGroup = new QGroupBox("Update Stock Quantity");
    stockGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* stockForm = new QFormLayout(stockGroup);
    stockForm->setSpacing(8);

    QComboBox* stockPartCombo = new QComboBox();
    populatePartCombo(stockPartCombo);
    QSpinBox* newStockSpin = new QSpinBox();
    newStockSpin->setRange(0, 10000);

    QPushButton* updateStockBtn = new QPushButton("Update Stock");
    updateStockBtn->setObjectName("warningBtn");
    updateStockBtn->setMinimumHeight(36);

    stockForm->addRow("Part:", stockPartCombo);
    stockForm->addRow("New Quantity:", newStockSpin);
    stockForm->addRow("", updateStockBtn);

    QGroupBox* logPartGroup = new QGroupBox("Log Used Part on Repair Order");
    logPartGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* logPartForm = new QFormLayout(logPartGroup);
    logPartForm->setSpacing(8);

    QComboBox* logOrderCombo = new QComboBox();
    populateOrderCombo(logOrderCombo);
    QComboBox* logPartCombo = new QComboBox();
    populatePartCombo(logPartCombo);
    QSpinBox* logQtySpin = new QSpinBox();
    logQtySpin->setRange(1, 100);

    QPushButton* logPartBtn = new QPushButton("Log Part Usage");
    logPartBtn->setObjectName("purpleBtn");
    logPartBtn->setMinimumHeight(36);

    logPartForm->addRow("Repair Order:", logOrderCombo);
    logPartForm->addRow("Part:", logPartCombo);
    logPartForm->addRow("Quantity:", logQtySpin);
    logPartForm->addRow("", logPartBtn);

    QGroupBox* tableGroup = new QGroupBox("Inventory");
    tableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setSpacing(8);

    QPushButton* partsRefreshBtn = new QPushButton("Refresh Inventory");

    QTableWidget* partsTable = new QTableWidget();
    partsTable->setColumnCount(5);
    partsTable->setHorizontalHeaderLabels({"ID", "Name", "Part Number", "Unit Price", "Stock Qty"});
    partsTable->horizontalHeader()->setStretchLastSection(true);
    partsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    partsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableLayout->addWidget(partsRefreshBtn);
    tableLayout->addWidget(partsTable, 1);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(addPartGroup);
    topLayout->addWidget(stockGroup);
    topLayout->addWidget(logPartGroup);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tableGroup, 1);

    connect(addPartBtn, &QPushButton::clicked, this, [this, partNameEdit, partNumberEdit, partPriceSpin, partQtySpin, partsTable, stockPartCombo, logPartCombo]() {
        std::string name = partNameEdit->text().toStdString();
        std::string partNum = partNumberEdit->text().toStdString();
        double price = partPriceSpin->value();
        int qty = partQtySpin->value();
        if (name.empty() || partNum.empty()) { showError("Validation Error", "Part name and number are required."); return; }
        InventoryRepository repo;
        if (repo.addPart(name, partNum, price, qty)) {
            showInfo("Success", "Part added successfully!");
            partNameEdit->clear(); partNumberEdit->clear(); partPriceSpin->setValue(0); partQtySpin->setValue(0);
            refreshPartsTable(partsTable);
            populatePartCombo(stockPartCombo); populatePartCombo(logPartCombo);
        } else { showError("Error", "Failed to add part. Part number may already exist."); }
    });

    connect(updateStockBtn, &QPushButton::clicked, this, [this, stockPartCombo, newStockSpin, partsTable]() {
        int partId = stockPartCombo->currentData().toInt();
        int newQty = newStockSpin->value();
        if (partId <= 0) { showError("Validation Error", "Please select a part."); return; }
        InventoryRepository repo;
        if (repo.updatePartStock(partId, newQty)) {
            showInfo("Success", "Stock quantity updated!");
            refreshPartsTable(partsTable);
        } else { showError("Error", "Failed to update stock."); }
    });

    connect(logPartBtn, &QPushButton::clicked, this, [this, logOrderCombo, logPartCombo, logQtySpin, partsTable]() {
        int orderId = logOrderCombo->currentData().toInt();
        int partId = logPartCombo->currentData().toInt();
        int qty = logQtySpin->value();
        if (orderId <= 0 || partId <= 0 || qty <= 0) { showError("Validation Error", "Please select order, part and quantity."); return; }
        InventoryRepository repo;
        if (repo.consumePartForOrder(orderId, partId, qty)) {
            showInfo("Success", "Part usage logged successfully!");
            refreshPartsTable(partsTable);
        } else { showError("Error", "Failed to log part usage. Check stock availability."); }
    });

    connect(partsRefreshBtn, &QPushButton::clicked, this, [this, partsTable, stockPartCombo, logPartCombo]() {
        refreshPartsTable(partsTable);
        populatePartCombo(stockPartCombo); populatePartCombo(logPartCombo);
    });

    refreshPartsTable(partsTable);
    return panel;
}

// ============================================================================
// BILLING PANEL
// ============================================================================
QWidget* MainWindow::createBillingPanel() {
    QWidget* panel = new QWidget();
    panel->setObjectName("panelContainer");
    panel->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    QGroupBox* calcGroup = new QGroupBox("Calculate Total Bill");
    calcGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* calcForm = new QFormLayout(calcGroup);
    calcForm->setSpacing(8);

    QComboBox* billOrderCombo = new QComboBox();
    populateOrderCombo(billOrderCombo);
    QLabel* billAmountLabel = new QLabel("$0.00");
    billAmountLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #22c55e; padding: 8px 0;");
    QPushButton* calcBtn = new QPushButton("Calculate Bill");
    calcBtn->setObjectName("successBtn");
    calcBtn->setMinimumHeight(36);

    calcForm->addRow("Service Order:", billOrderCombo);
    calcForm->addRow("Total Amount:", billAmountLabel);
    calcForm->addRow("", calcBtn);

    QGroupBox* payGroup = new QGroupBox("Process Payment");
    payGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* payForm = new QFormLayout(payGroup);
    payForm->setSpacing(8);

    QComboBox* payMethodCombo = new QComboBox();
    payMethodCombo->addItems({"Cash", "Card"});
    QPushButton* payBtn = new QPushButton("Process Payment");
    payBtn->setObjectName("successBtn");
    payBtn->setMinimumHeight(36);

    payForm->addRow("Payment Method:", payMethodCombo);
    payForm->addRow("", payBtn);

    QGroupBox* tableGroup = new QGroupBox("Payment History");
    tableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setSpacing(8);

    QPushButton* payRefreshBtn = new QPushButton("Refresh Payments");
    QTableWidget* paymentsTable = new QTableWidget();
    paymentsTable->setColumnCount(5);
    paymentsTable->setHorizontalHeaderLabels({"ID", "Order ID", "Amount", "Method", "Date"});
    paymentsTable->horizontalHeader()->setStretchLastSection(true);
    paymentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    paymentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableLayout->addWidget(payRefreshBtn);
    tableLayout->addWidget(paymentsTable, 1);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(calcGroup);
    topLayout->addWidget(payGroup);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tableGroup, 1);

    connect(calcBtn, &QPushButton::clicked, this, [this, billOrderCombo, billAmountLabel]() {
        int orderId = billOrderCombo->currentData().toInt();
        if (orderId <= 0) { showError("Validation Error", "Please select a service order."); return; }
        BillingService billing;
        double total = billing.calculateTotalBill(orderId);
        billAmountLabel->setText(QString("$%1").arg(total, 0, 'f', 2));
    });

    connect(payBtn, &QPushButton::clicked, this, [this, billOrderCombo, payMethodCombo, paymentsTable, billAmountLabel]() {
        int orderId = billOrderCombo->currentData().toInt();
        std::string method = payMethodCombo->currentText().toStdString();
        if (orderId <= 0) { showError("Validation Error", "Please select a service order."); return; }
        BillingService billing;
        double total = billing.calculateTotalBill(orderId);
        if (total <= 0) { showError("Error", "No parts found for this order. Cannot process payment."); return; }
        if (!confirmAction("Confirm Payment", QString("Process payment of $%1 via %2?").arg(total, 0, 'f', 2).arg(QString::fromStdString(method)))) return;
        if (billing.processPayment(orderId, total, method)) {
            showInfo("Success", "Payment processed successfully!");
            billAmountLabel->setText("$0.00");
            refreshServiceOrdersTable(paymentsTable);
        } else { showError("Error", "Failed to process payment."); }
    });

    connect(payRefreshBtn, &QPushButton::clicked, this, [this, paymentsTable]() { refreshServiceOrdersTable(paymentsTable); });
    return panel;
}

// ============================================================================
// ADMIN PANEL
// ============================================================================
QWidget* MainWindow::createAdminPanel() {
    QWidget* panel = new QWidget();
    panel->setObjectName("panelContainer");
    panel->setStyleSheet("background-color: #0f172a;");
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    QGroupBox* userGroup = new QGroupBox("Create New Staff User");
    userGroup->setStyleSheet(DASHBOARD_STYLE);
    QFormLayout* userForm = new QFormLayout(userGroup);
    userForm->setSpacing(8);

    QLineEdit* newUsernameEdit = new QLineEdit();
    newUsernameEdit->setPlaceholderText("Username");
    QLineEdit* newPasswordEdit = new QLineEdit();
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setPlaceholderText("Password");
    QLineEdit* newFullNameEdit = new QLineEdit();
    newFullNameEdit->setPlaceholderText("Full Name");
    QComboBox* newRoleCombo = new QComboBox();
    newRoleCombo->addItems({"Admin", "Receptionist", "Mechanic"});

    QPushButton* createUserBtn = new QPushButton("Create User");
    createUserBtn->setObjectName("successBtn");
    createUserBtn->setMinimumHeight(36);

    userForm->addRow("Username:", newUsernameEdit);
    userForm->addRow("Password:", newPasswordEdit);
    userForm->addRow("Full Name:", newFullNameEdit);
    userForm->addRow("Role:", newRoleCombo);
    userForm->addRow("", createUserBtn);

    QGroupBox* reportsGroup = new QGroupBox("Reports & Analytics");
    reportsGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* reportsLayout = new QVBoxLayout(reportsGroup);
    reportsLayout->setSpacing(8);

    QPushButton* revenueBtn = new QPushButton("View Total Revenue");
    revenueBtn->setMinimumHeight(36);
    QPushButton* usersBtn = new QPushButton("View All Users");
    usersBtn->setMinimumHeight(36);
    QPushButton* auditBtn = new QPushButton("View Audit Logs");
    auditBtn->setMinimumHeight(36);

    reportsLayout->addWidget(revenueBtn);
    reportsLayout->addWidget(usersBtn);
    reportsLayout->addWidget(auditBtn);

    QGroupBox* usersTableGroup = new QGroupBox("System Users");
    usersTableGroup->setStyleSheet(DASHBOARD_STYLE);
    QVBoxLayout* usersTableLayout = new QVBoxLayout(usersTableGroup);
    usersTableLayout->setSpacing(8);

    QPushButton* usersRefreshBtn = new QPushButton("Refresh Users");
    QTableWidget* usersTable = new QTableWidget();
    usersTable->setColumnCount(4);
    usersTable->setHorizontalHeaderLabels({"ID", "Username", "Full Name", "Role"});
    usersTable->horizontalHeader()->setStretchLastSection(true);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    usersTableLayout->addWidget(usersRefreshBtn);
    usersTableLayout->addWidget(usersTable, 1);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(userGroup);
    topLayout->addWidget(reportsGroup);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(usersTableGroup, 1);

    connect(createUserBtn, &QPushButton::clicked, this, [this, newUsernameEdit, newPasswordEdit, newFullNameEdit, newRoleCombo, usersTable]() {
        std::string username = newUsernameEdit->text().toStdString();
        std::string password = newPasswordEdit->text().toStdString();
        std::string fullName = newFullNameEdit->text().toStdString();
        std::string role = newRoleCombo->currentText().toStdString();
        if (username.empty() || password.empty() || fullName.empty()) { showError("Validation Error", "All fields are required."); return; }
        UserRepository repo;
        if (repo.createUser(username, password, fullName, role)) {
            showInfo("Success", "User created successfully!");
            newUsernameEdit->clear(); newPasswordEdit->clear(); newFullNameEdit->clear();
            refreshUsersTable(usersTable);
        } else { showError("Error", "Failed to create user. Username may already exist."); }
    });

    connect(usersRefreshBtn, &QPushButton::clicked, this, [this, usersTable]() { refreshUsersTable(usersTable); });
    connect(revenueBtn, &QPushButton::clicked, this, [this]() {
        BillingService billing;
        double revenue = billing.getTotalRevenue();
        showInfo("Total Revenue", QString("Total Revenue: $%1").arg(revenue, 0, 'f', 2));
    });
    connect(usersBtn, &QPushButton::clicked, this, [this, usersTable]() { refreshUsersTable(usersTable); });
    connect(auditBtn, &QPushButton::clicked, this, [this]() { showInfo("Audit Logs", "Audit logs are printed to the console output."); });

    refreshUsersTable(usersTable);
    return panel;
}

// ============================================================================
// HELPER METHODS
// ============================================================================
void MainWindow::showError(const QString& title, const QString& message) {
    QMessageBox::critical(this, title, message);
}

void MainWindow::showInfo(const QString& title, const QString& message) {
    QMessageBox::information(this, title, message);
}

bool MainWindow::confirmAction(const QString& title, const QString& message) {
    return QMessageBox::question(this, title, message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void MainWindow::refreshCustomersTable(QTableWidget* table) {
    CustomerRepository repo;
    auto customers = repo.getAllCustomers();
    table->setRowCount(customers.size());
    for (size_t i = 0; i < customers.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(customers[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(customers[i].name)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(customers[i].phone)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(customers[i].email)));
    }
}

void MainWindow::refreshVehiclesTable(QTableWidget* table) {
    VehicleRepository repo;
    auto vehicles = repo.getAllVehicles();
    table->setRowCount(vehicles.size());
    for (size_t i = 0; i < vehicles.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(vehicles[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(vehicles[i].licensePlate)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(vehicles[i].make)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(vehicles[i].model)));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(vehicles[i].year)));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(vehicles[i].customerName)));
    }
}

void MainWindow::refreshAppointmentsTable(QTableWidget* table) {
    AppointmentRepository repo;
    auto appointments = repo.getAllAppointments();
    table->setRowCount(appointments.size());
    for (size_t i = 0; i < appointments.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(appointments[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(appointments[i].vehicleId)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(appointments[i].customerId)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(appointments[i].scheduledDate)));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(appointments[i].status)));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(appointments[i].vehiclePlate)));
        table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(appointments[i].customerName)));
    }
}

void MainWindow::refreshServiceOrdersTable(QTableWidget* table) {
    ServiceOrderManager mgr;
    auto orders = mgr.getAllServiceOrders();
    table->setRowCount(orders.size());
    for (size_t i = 0; i < orders.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(orders[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(orders[i].appointmentId)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(orders[i].mechanicName)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(orders[i].status)));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(orders[i].creationDate)));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(orders[i].notes)));
        table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(orders[i].vehiclePlate)));
    }
}

void MainWindow::refreshPartsTable(QTableWidget* table) {
    InventoryRepository repo;
    auto parts = repo.getAllParts();
    table->setRowCount(parts.size());
    for (size_t i = 0; i < parts.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(parts[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(parts[i].name)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(parts[i].partNumber)));
        table->setItem(i, 3, new QTableWidgetItem(QString("$%1").arg(parts[i].unitPrice, 0, 'f', 2)));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(parts[i].stockQuantity)));
    }
}

void MainWindow::refreshUsersTable(QTableWidget* table) {
    UserRepository repo;
    auto users = repo.getAllUsers();
    table->setRowCount(users.size());
    for (size_t i = 0; i < users.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(users[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(users[i].username)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(users[i].fullName)));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(users[i].role)));
    }
}

void MainWindow::populateCustomerCombo(QComboBox* combo) {
    combo->clear();
    CustomerRepository repo;
    auto customers = repo.getAllCustomers();
    for (const auto& c : customers) {
        combo->addItem(QString("%1 (%2)").arg(QString::fromStdString(c.name)).arg(QString::fromStdString(c.phone)), c.id);
    }
}

void MainWindow::populateVehicleCombo(QComboBox* combo) {
    combo->clear();
    VehicleRepository repo;
    auto vehicles = repo.getAllVehicles();
    for (const auto& v : vehicles) {
        combo->addItem(QString("%1 - %2 %3").arg(QString::fromStdString(v.licensePlate)).arg(QString::fromStdString(v.make)).arg(QString::fromStdString(v.model)), v.id);
    }
}

void MainWindow::populateMechanicCombo(QComboBox* combo) {
    combo->clear();
    UserRepository repo;
    auto users = repo.getAllUsers();
    for (const auto& u : users) {
        if (u.role == "Mechanic") {
            combo->addItem(QString::fromStdString(u.fullName), u.id);
        }
    }
}

void MainWindow::populatePartCombo(QComboBox* combo) {
    combo->clear();
    InventoryRepository repo;
    auto parts = repo.getAllParts();
    for (const auto& p : parts) {
        combo->addItem(QString("%1 (%2) - $%3").arg(QString::fromStdString(p.name)).arg(QString::fromStdString(p.partNumber)).arg(p.unitPrice, 0, 'f', 2), p.id);
    }
}

void MainWindow::populateOrderCombo(QComboBox* combo) {
    combo->clear();
    ServiceOrderManager mgr;
    auto orders = mgr.getAllServiceOrders();
    for (const auto& o : orders) {
        combo->addItem(QString("#%1 - %2 - %3").arg(o.id).arg(QString::fromStdString(o.vehiclePlate)).arg(QString::fromStdString(o.status)), o.id);
    }
}

// ============================================================================
// ACTION HANDLERS (called from lambdas)
// ============================================================================
void MainWindow::onAddCustomer() { /* Implemented in lambda */ }
void MainWindow::onSearchCustomers() { /* Implemented in lambda */ }
void MainWindow::onAddVehicle() { /* Implemented in lambda */ }
void MainWindow::onScheduleAppointment() { /* Implemented in lambda */ }
void MainWindow::onUpdateAppointmentStatus() { /* Implemented in lambda */ }
void MainWindow::onCreateServiceOrder() { /* Implemented in lambda */ }
void MainWindow::onUpdateOrderStatus() { /* Implemented in lambda */ }
void MainWindow::onAddPart() { /* Implemented in lambda */ }
void MainWindow::onUpdateStock() { /* Implemented in lambda */ }
void MainWindow::onLogPartForOrder() { /* Implemented in lambda */ }
void MainWindow::onCalculateBill() { /* Implemented in lambda */ }
void MainWindow::onProcessPayment() { /* Implemented in lambda */ }
void MainWindow::onCreateUser() { /* Implemented in lambda */ }
void MainWindow::onViewReports() {
    BillingService billing;
    double revenue = billing.getTotalRevenue();
    showInfo("Total Revenue", QString("Total Revenue: $%1").arg(revenue, 0, 'f', 2));
}
