#ifndef SUPPLIERSDIALOG_H
#define SUPPLIERSDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class SuppliersDialog;
}

class SuppliersDialog : public QDialog {
  Q_OBJECT
public:
  explicit SuppliersDialog(QWidget *parent = nullptr);
  ~SuppliersDialog();

private slots:
  void on_btnAdd_clicked();
  void on_btnDelete_clicked();
  void on_btnSave_clicked();

private:
  Ui::SuppliersDialog *ui;
  QSqlTableModel *m_model;
};

#endif // SUPPLIERSDIALOG_H
