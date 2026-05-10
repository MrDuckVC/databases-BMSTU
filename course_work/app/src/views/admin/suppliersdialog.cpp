#include "suppliersdialog.h"
#include "databasemanager.h"
#include "ui_suppliersdialog.h"
#include "utils.h"
#include <QMessageBox>
#include <QSqlError>

SuppliersDialog::SuppliersDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SuppliersDialog)
{
    ui->setupUi(this);
    setWindowTitle("Справочник поставщиков");
    resize(800, 400);

    m_model = new QSqlTableModel(this, DatabaseManager::instance().getDatabase());
    m_model->setTable("suppliers");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    m_model->setHeaderData(1, Qt::Horizontal, "Название компании");
    m_model->setHeaderData(2, Qt::Horizontal, "Телефон");
    m_model->setHeaderData(3, Qt::Horizontal, "Email");

    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

SuppliersDialog::~SuppliersDialog() {
    delete ui;
}

void SuppliersDialog::on_btnAdd_clicked() {
    m_model->insertRow(m_model->rowCount());
}

void SuppliersDialog::on_btnDelete_clicked() {
    int row = ui->tableView->currentIndex().row();
    if (row >= 0) {
        m_model->removeRow(row);
    } else {
        QMessageBox::warning(this, "Внимание", "Сначала выделите поставщика для удаления.");
    }
}

void SuppliersDialog::on_btnSave_clicked() {
    if (!m_model->submitAll()) {
        QMessageBox::critical(this, "Ошибка сохранения",
                               Utils::formatSqlError(m_model->lastError()));
        m_model->revertAll();
    } else {
        QMessageBox::information(this, "Успех", "Данные поставщиков обновлены.");
    }
}
