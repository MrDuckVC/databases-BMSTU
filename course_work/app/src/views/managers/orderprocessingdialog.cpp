#include "orderprocessingdialog.h"
#include "ui_orderprocessingdialog.h"
#include "databasemanager.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlRecord>

OrderProcessingDialog::OrderProcessingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderProcessingDialog),
    m_model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowTitle("Управление заказами");
    resize(850, 500);

    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->cbStatus->addItems({"Принят", "Оформляется", "Оплачен", "Доставлен", "Отменен"});
    ui->btnUpdateStatus->setEnabled(false);

    loadOrders();
}

OrderProcessingDialog::~OrderProcessingDialog() {
    delete ui;
}

void OrderProcessingDialog::loadOrders() {
    m_model->setQuery("SELECT order_id AS \"№ Заказа\", created_at AS \"Дата оформления\", "
                      "client_name AS \"Клиент\", status AS \"Статус\", order_total AS \"Сумма (руб)\" "
                      "FROM v_order_statuses ORDER BY order_id DESC");

    if (m_model->lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка загрузки", Utils::formatSqlError(m_model->lastError()));
    }
}

void OrderProcessingDialog::on_tableView_clicked(const QModelIndex &index) {
    int row = index.row();

    m_selectedOrderId = m_model->record(row).value("№ Заказа").toInt();
    QString currentStatus = m_model->record(row).value("Статус").toString();

    ui->lblSelectedOrder->setText(QString("Выбран заказ: № %1").arg(m_selectedOrderId));
    ui->cbStatus->setCurrentText(currentStatus);
    ui->btnUpdateStatus->setEnabled(true);
}

void OrderProcessingDialog::on_btnUpdateStatus_clicked() {
    if (m_selectedOrderId == -1) return;

    QString newStatus = ui->cbStatus->currentText();

    QSqlQuery q;

    q.prepare("UPDATE orders SET status = CAST(:status AS order_status_item) WHERE id = :id");

    q.bindValue(":status", newStatus);
    q.bindValue(":id", m_selectedOrderId);

    if (!q.exec()) {
        QMessageBox::critical(this, "Ошибка БД", Utils::formatSqlError(q.lastError()));
    } else {
        QMessageBox::information(this, "Успех", QString("Статус заказа №%1 изменен на «%2»").arg(m_selectedOrderId).arg(newStatus));
        loadOrders();

        m_selectedOrderId = -1;
        ui->lblSelectedOrder->setText("Заказ не выбран");
        ui->btnUpdateStatus->setEnabled(false);
    }
}
