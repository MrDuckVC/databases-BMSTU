#include "clientsdialog.h"
#include "databasemanager.h"
#include "ui_clientsdialog.h"
#include "utils.h"
#include <QMessageBox>
#include <QSqlError>

ClientsDialog::ClientsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ClientsDialog) {
  ui->setupUi(this);
  setWindowTitle("Справочник клиентов");
  resize(800, 400);

  m_model = new QSqlTableModel(this, DatabaseManager::instance().getDatabase());
  m_model->setTable("clients");
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_model->select();

  m_model->setHeaderData(1, Qt::Horizontal, "ФИО Клиента");
  m_model->setHeaderData(2, Qt::Horizontal, "Телефон");
  m_model->setHeaderData(3, Qt::Horizontal, "Email");

  ui->tableView->setModel(m_model);
  ui->tableView->hideColumn(0);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

ClientsDialog::~ClientsDialog() { delete ui; }

void ClientsDialog::on_btnAdd_clicked() {
  m_model->insertRow(m_model->rowCount());
}

void ClientsDialog::on_btnDelete_clicked() {
  int row = ui->tableView->currentIndex().row();
  if (row >= 0) {
    m_model->removeRow(row);
  } else {
    QMessageBox::warning(this, "Внимание", "Выберите клиента для удаления.");
  }
}

void ClientsDialog::on_btnSave_clicked() {
  if (!m_model->submitAll()) {
    QString cleanError = Utils::formatSqlError(m_model->lastError());
    QMessageBox::critical(this, "Ошибка сохранения",
                          "Не удалось сохранить изменения.\n\nПричина: " +
                              cleanError);
    m_model->revertAll();
  } else {
    QMessageBox::information(this, "Успех", "Изменения успешно сохранены!");
  }
}
