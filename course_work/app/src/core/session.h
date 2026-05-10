#ifndef SESSION_H
#define SESSION_H

#include <QString>

class Session {
public:
  static Session &instance();

  void start(int employeeId, const QString &fullName, const QString &role,
             const QString &login);
  void clear();

  int getEmployeeId() const;
  QString getFullName() const;
  QString getRole() const;
  QString getLogin() const;

private:
  Session() = default;
  ~Session() = default;

  // Запрет копирования для обеспечения паттерна Singleton
  Session(const Session &) = delete;
  Session &operator=(const Session &) = delete;

  int m_employeeId = -1;
  QString m_fullName;
  QString m_role;
  QString m_login;
};

#endif // SESSION_H
