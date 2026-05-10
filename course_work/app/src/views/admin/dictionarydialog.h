#ifndef DICTIONARYDIALOG_H
#define DICTIONARYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class DictionaryDialog;
}

class DictionaryDialog : public QDialog {
  Q_OBJECT

public:
  explicit DictionaryDialog(const QString &tableName, const QString &title,
                            QWidget *parent = nullptr);
  ~DictionaryDialog();

private slots:
  void on_btnAdd_clicked();
  void on_btnDelete_clicked();
  void on_btnSave_clicked();

private:
  Ui::DictionaryDialog *ui;
  QSqlTableModel *m_model;
  QString m_tableName;

  void setupHeaders();
};

#endif // DICTIONARYDIALOG_H
