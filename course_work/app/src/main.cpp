#include "mainwindow.h"
#include "databasemanager.h"
#include "logindialog.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::instance().connectToDatabase()) {
        QMessageBox::critical(nullptr, "Ошибка запуска", "База данных недоступна!");
        return -1;
    }

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w(login.getUserName(), login.getUserRole());
        w.show();
        return a.exec();
    }

    // Завершаем работу, если пользователь закрыл окно авторизации крестиком
    return 0;
}
