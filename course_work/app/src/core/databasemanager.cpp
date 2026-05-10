#include "databasemanager.h"
#include <QDebug>
#include <QFile>
#include <QProcessEnvironment>
#include <QSqlError>
#include <QTextStream>

// Вспомогательная функция для парсинга .env файла
void loadEnvFile() {
  // Ищем файл .env в текущей директории запуска
  QFile file(".env");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Файл .env не найден. Будут использованы системные переменные "
                "или значения по умолчанию.";
    return;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();

    // Игнорируем пустые строки и комментарии
    if (line.isEmpty() || line.startsWith("#"))
      continue;

    int equalPos = line.indexOf('=');
    if (equalPos > 0) {
      QString key = line.left(equalPos).trimmed();
      QString value = line.mid(equalPos + 1).trimmed();

      // Убираем кавычки, если они есть (например: DB_PASS="123")
      if (value.startsWith('"') && value.endsWith('"')) {
        value = value.mid(1, value.length() - 2);
      }

      // Записываем в среду выполнения (C-style)
      qputenv(key.toLocal8Bit(), value.toLocal8Bit());
    }
  }
  file.close();
}

DatabaseManager &DatabaseManager::instance() {
  static DatabaseManager instance;
  return instance;
}

DatabaseManager::DatabaseManager() {
  // Загружаем переменные из .env файла ДО того, как начнем их читать
  loadEnvFile();

  db = QSqlDatabase::addDatabase("QPSQL");

  // Читаем переменные (они теперь подтянутся либо из ОС, либо из .env файла)
  // qEnvironmentVariable - удобная функция Qt для чтения переменных окружения
  QString host = qEnvironmentVariable("DB_HOST", "localhost");
  int port = qEnvironmentVariable("DB_PORT", "5432").toInt();
  QString dbName = qEnvironmentVariable("DB_NAME", "course_work");
  QString user = qEnvironmentVariable("DB_USER", "shop_app_user");
  QString pass = qEnvironmentVariable("DB_PASS", "");

  db.setHostName(host);
  db.setPort(port);
  db.setDatabaseName(dbName);
  db.setUserName(user);
  db.setPassword(pass);
}

DatabaseManager::~DatabaseManager() { disconnectFromDatabase(); }

bool DatabaseManager::connectToDatabase() {
  if (db.isOpen())
    return true;

  if (!db.open()) {
    qCritical() << "Ошибка подключения к БД:" << db.lastError().text();
    return false;
  }

  qDebug() << "Успешное подключение к PostgreSQL!";
  return true;
}

void DatabaseManager::disconnectFromDatabase() {
  if (db.isOpen()) {
    db.close();
    qDebug() << "Отключено от БД.";
  }
}

QSqlDatabase DatabaseManager::getDatabase() const { return db; }
