#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
  Q_OBJECT

public:
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog();

  QString getUserRole() const;
  QString getUserName() const;

private slots:
  void on_btnLogin_clicked();

private:
  Ui::LoginDialog *ui;
  QString m_userRole;
  QString m_userName;
};

#endif // LOGINDIALOG_H
