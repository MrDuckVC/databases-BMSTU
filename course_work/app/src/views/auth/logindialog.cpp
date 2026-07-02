#include "logindialog.h"
#include "session.h"
#include "ui_logindialog.h"
#include "utils.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);
  ui->lePassword->setEchoMode(QLineEdit::Password);
}

LoginDialog::~LoginDialog() { delete ui; }

QString LoginDialog::getUserRole() const { return m_userRole; }
QString LoginDialog::getUserName() const { return m_userName; }

void LoginDialog::on_btnLogin_clicked() {
  QString login = ui->leLogin->text();
  QString rawPassword = ui->lePassword->text();

  if (login.isEmpty() || rawPassword.isEmpty()) {
    QMessageBox::warning(this, "Внимание", "Заполните все поля!");
    return;
  }

  QString hashedPassword = Utils::hashPassword(rawPassword);

  QSqlQuery query;
  query.prepare("SELECT u.role, e.full_name, e.id, u.login "
                "FROM app_users u "
                "JOIN employees e ON u.employee_id = e.id "
                "WHERE u.login = :login AND u.password_hash = :password");

  query.bindValue(":login", login);
  query.bindValue(":password", hashedPassword);

  if (!query.exec()) {
    QMessageBox::critical(this, "Ошибка БД", query.lastError().text());
    return;
  }

  if (query.next()) {
    m_userRole = query.value(0).toString();
    m_userName = query.value(1).toString();

    // Инициализация глобальной сессии данными из БД
    Session::instance().start(query.value(2).toInt(), // employee_id
                              m_userName, m_userRole,
                              query.value(3).toString() // login
    );

    accept();
  } else {
    QMessageBox::warning(this, "Ошибка авторизации",
                         "Неверный логин или пароль!");
  }
}
