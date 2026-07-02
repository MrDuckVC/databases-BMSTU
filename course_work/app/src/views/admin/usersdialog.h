#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class UsersDialog;
}

class UsersDialog : public QDialog {
  Q_OBJECT

public:
  explicit UsersDialog(QWidget *parent = nullptr);
  ~UsersDialog();

private slots:
  void on_btnSave_clicked();
  void on_btnDelete_clicked();
  void on_tvUsers_clicked(const QModelIndex &index);

private:
  Ui::UsersDialog *ui;
  QSqlQueryModel *m_usersModel;

  void loadEmployees();
  void updateUsersTable();
};

#endif // USERSDIALOG_H
