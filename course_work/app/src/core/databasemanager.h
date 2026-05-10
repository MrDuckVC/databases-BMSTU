#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager &instance();
    bool connectToDatabase();
    void disconnectFromDatabase();
    QSqlDatabase getDatabase() const;

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
