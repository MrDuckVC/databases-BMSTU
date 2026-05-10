#include "dictionarydialog.h"
#include "databasemanager.h"
#include "ui_dictionarydialog.h"
#include "utils.h"
#include <QMessageBox>
#include <QSqlError>

DictionaryDialog::DictionaryDialog(const QString &tableName,
                                   const QString &title, QWidget *parent)
    : QDialog(parent), ui(new Ui::DictionaryDialog), m_tableName(tableName) {
  ui->setupUi(this);
  setWindowTitle(title);

  m_model = new QSqlTableModel(this, DatabaseManager::instance().getDatabase());
  m_model->setTable(m_tableName);
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_model->select();

  setupHeaders();

  ui->tableView->setModel(m_model);
  ui->tableView->hideColumn(0);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

DictionaryDialog::~DictionaryDialog() { delete ui; }

void DictionaryDialog::setupHeaders() {
  if (m_tableName == "categories") {
    m_model->setHeaderData(1, Qt::Horizontal, "Название категории");
  } else if (m_tableName == "employees") {
    m_model->setHeaderData(1, Qt::Horizontal, "ФИО Сотрудника");
    m_model->setHeaderData(2, Qt::Horizontal, "Должность");
  }
}

void DictionaryDialog::on_btnAdd_clicked() {
  m_model->insertRow(m_model->rowCount());
}

void DictionaryDialog::on_btnDelete_clicked() {
  int row = ui->tableView->currentIndex().row();
  if (row >= 0) {
    m_model->removeRow(row);
  } else {
    QMessageBox::warning(this, "Внимание", "Выберите строку для удаления.");
  }
}

void DictionaryDialog::on_btnSave_clicked() {
  if (!m_model->submitAll()) {
    QMessageBox::critical(this, "Ошибка сохранения",
                          Utils::formatSqlError(m_model->lastError()));
    m_model->revertAll();
  } else {
    QMessageBox::information(this, "Успех", "Данные успешно обновлены.");
  }
}
