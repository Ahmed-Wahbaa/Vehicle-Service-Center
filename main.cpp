#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        DatabaseManager::getInstance("host=localhost port=5432 dbname=vehicle_db user=postgres password=Sugar1234#");
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Database Connection Error", e.what());
        return 1;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
