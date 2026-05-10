#include "session.h"

Session& Session::instance() {
    static Session instance;
    return instance;
}

void Session::start(int employeeId, const QString& fullName, const QString& role, const QString& login) {
    m_employeeId = employeeId;
    m_fullName = fullName;
    m_role = role;
    m_login = login;
}

void Session::clear() {
    m_employeeId = -1;
    m_fullName.clear();
    m_role.clear();
    m_login.clear();
}

int Session::getEmployeeId() const { return m_employeeId; }
QString Session::getFullName() const { return m_fullName; }
QString Session::getRole() const { return m_role; }
QString Session::getLogin() const { return m_login; }
