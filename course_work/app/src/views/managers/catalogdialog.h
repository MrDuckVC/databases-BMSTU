#ifndef CATALOGDIALOG_H
#define CATALOGDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class CatalogDialog;
}

class CatalogDialog : public QDialog {
  Q_OBJECT

public:
  explicit CatalogDialog(QWidget *parent = nullptr);
  ~CatalogDialog();

private slots:
  void on_leSearch_textChanged(const QString &text);

private:
  Ui::CatalogDialog *ui;
  QSqlQueryModel *m_model;

  void loadData(const QString &filter = "");
};

#endif // CATALOGDIALOG_H
