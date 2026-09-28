#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QFrame>
#include <QScrollBar>
#include "vehicle_service_center.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    std::unique_ptr<UserRole> currentUser;

    QStackedWidget* mainStack;
    QWidget* loginPage;
    QWidget* dashboardPage;

    QLineEdit* userEdit;
    QLineEdit* passEdit;
    QLabel* welcomeLabel;
    QTabWidget* roleTabs;

    void setupLoginUI();
    void setupDashboardUI();
    void configureRoleDashboard();
    void clearLayout(QLayout* layout);

    // Panel creation methods
    QWidget* createCustomersPanel();
    QWidget* createAppointmentsPanel();
    QWidget* createServiceOrdersPanel();
    QWidget* createInventoryPanel();
    QWidget* createBillingPanel();
    QWidget* createAdminPanel();

    // Helper methods
    void showError(const QString& title, const QString& message);
    void showInfo(const QString& title, const QString& message);
    bool confirmAction(const QString& title, const QString& message);
    void refreshCustomersTable(QTableWidget* table);
    void refreshVehiclesTable(QTableWidget* table);
    void refreshAppointmentsTable(QTableWidget* table);
    void refreshServiceOrdersTable(QTableWidget* table);
    void refreshPartsTable(QTableWidget* table);
    void refreshUsersTable(QTableWidget* table);
    void populateCustomerCombo(QComboBox* combo);
    void populateVehicleCombo(QComboBox* combo);
    void populateMechanicCombo(QComboBox* combo);
    void populatePartCombo(QComboBox* combo);
    void populateOrderCombo(QComboBox* combo);

private slots:
    void handleLogin();
    void handleLogout();

    // Action handlers (called from lambdas)
    void onAddCustomer();
    void onSearchCustomers();
    void onAddVehicle();
    void onScheduleAppointment();
    void onUpdateAppointmentStatus();
    void onCreateServiceOrder();
    void onUpdateOrderStatus();
    void onAddPart();
    void onUpdateStock();
    void onLogPartForOrder();
    void onCalculateBill();
    void onProcessPayment();
    void onCreateUser();
    void onViewReports();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
};

#endif // MAINWINDOW_H