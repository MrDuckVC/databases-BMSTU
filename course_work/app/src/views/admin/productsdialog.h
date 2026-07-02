#ifndef PRODUCTSDIALOG_H
#define PRODUCTSDIALOG_H

#include <QDialog>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>

namespace Ui {
class ProductsDialog;
}

class ProductsDialog : public QDialog {
  Q_OBJECT

public:
  explicit ProductsDialog(QWidget *parent = nullptr);
  ~ProductsDialog();

private slots:
  void on_btnAdd_clicked();
  void on_btnDelete_clicked();
  void on_btnSave_clicked();
  void on_leSearch_textChanged(const QString &text);

private:
  Ui::ProductsDialog *ui;
  QSqlRelationalTableModel *m_model;

  void setupModel();
};

#endif // PRODUCTSDIALOG_H
