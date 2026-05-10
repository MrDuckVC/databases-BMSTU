#include "ordersdialog.h"
#include "ui_ordersdialog.h"
#include "databasemanager.h"
#include "utils.h"
#include "session.h"
#include "reportexporter.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

OrdersDialog::OrdersDialog(const QString& employeeName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrdersDialog),
    m_employeeName(employeeName)
{
    ui->setupUi(this);
    setWindowTitle("Оформление нового заказа");

    m_cartModel = new QStandardItemModel(0, 4, this);
    m_cartModel->setHorizontalHeaderLabels({"ID", "Наименование", "Кол-во", "Сумма (руб)"});

    ui->tvCart->setModel(m_cartModel);
    ui->tvCart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvCart->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvCart->horizontalHeader()->setStretchLastSection(true);

    loadClients();
    loadProducts();
}

OrdersDialog::~OrdersDialog() {
    delete ui;
}

void OrdersDialog::loadClients() {
    QSqlQuery q("SELECT id, full_name, phone FROM clients ORDER BY full_name");
    while (q.next()) {
        QString text = QString("%1 (%2)").arg(q.value(1).toString(), q.value(2).toString());
        ui->cbClients->addItem(text, q.value(0));
    }
}

void OrdersDialog::loadProducts() {
    ui->cbProducts->clear();
    QSqlQuery q("SELECT id, product_name, retail_price, stock FROM products WHERE stock > 0");
    while (q.next()) {
        QString text = QString("%1 — %2 руб. (Запас: %3)")
                           .arg(q.value(1).toString())
                           .arg(q.value(2).toDouble(), 0, 'f', 2)
                           .arg(q.value(3).toInt());
        ui->cbProducts->addItem(text, q.value(0).toInt());
    }
}

void OrdersDialog::on_btnAddItem_clicked() {
    if (ui->cbProducts->currentIndex() == -1) return;

    int productId = ui->cbProducts->currentData().toInt();
    QString productStr = ui->cbProducts->currentText();

    // Парсинг данных из строки ComboBox
    QString productName = productStr.split(" —").first();
    double price = productStr.split(" — ")[1].split(" руб.").first().toDouble();
    int qty = ui->sbQuantity->value();

    for (int i = 0; i < m_cartModel->rowCount(); ++i) {
        if (m_cartModel->item(i, 0)->text() == QString::number(productId)) {
            QMessageBox::warning(this, "Внимание", "Товар уже в корзине. Обновите позицию, если нужно изменить количество.");
            return;
        }
    }

    QList<QStandardItem*> row;
    row << new QStandardItem(QString::number(productId))
        << new QStandardItem(productName)
        << new QStandardItem(QString::number(qty))
        << new QStandardItem(QString::number(qty * price, 'f', 2));
    m_cartModel->appendRow(row);

    updateCartTotal();
}

void OrdersDialog::updateCartTotal() {
    double total = 0;
    for (int i = 0; i < m_cartModel->rowCount(); ++i) {
        total += m_cartModel->item(i, 3)->text().toDouble();
    }
    ui->lblTotal->setText(QString("Итого: %1 руб.").arg(total, 0, 'f', 2));
}

void OrdersDialog::on_btnRemoveItem_clicked() {
    int row = ui->tvCart->currentIndex().row();
    if (row >= 0) {
        m_cartModel->removeRow(row);
        updateCartTotal();
    } else {
        QMessageBox::warning(this, "Внимание", "Выберите товар для удаления из корзины.");
    }
}

void OrdersDialog::on_btnCreateOrder_clicked() {
    if (m_cartModel->rowCount() == 0 || ui->leAddress->text().isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните адрес и добавьте товары в корзину.");
        return;
    }

    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    db.transaction();

    QSqlQuery query;
    int employeeId = Session::instance().getEmployeeId();
    int clientId = ui->cbClients->currentData().toInt();

    // 1. Создание шапки заказа
    query.prepare("INSERT INTO orders (address, client_id, employee_id) "
                  "VALUES (:address, :client, :emp) RETURNING id");
    query.bindValue(":address", ui->leAddress->text());
    query.bindValue(":client", clientId);
    query.bindValue(":emp", employeeId);

    if (!query.exec() || !query.next()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка", Utils::formatSqlError(query.lastError()));
        return;
    }

    int newOrderId = query.value(0).toInt();

    // 2. Сохранение позиций заказа
    query.prepare("INSERT INTO order_items (order_id, product_id, quantity) "
                  "VALUES (:order_id, :pid, :quantity)");

    for (int i = 0; i < m_cartModel->rowCount(); ++i) {
        query.bindValue(":order_id", newOrderId);
        query.bindValue(":pid", m_cartModel->item(i, 0)->text().toInt());
        query.bindValue(":quantity", m_cartModel->item(i, 2)->text().toInt());

        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "Ошибка", Utils::formatSqlError(query.lastError()));
            return;
        }
    }

    db.commit();

    // Опциональная печать чека
    auto res = QMessageBox::question(this, "Успех",
                                     QString("Заказ №%1 оформлен. Сформировать чек в PDF?").arg(newOrderId));

    if (res == QMessageBox::Yes) {
        QString clientName = ui->cbClients->currentText().split(" (").first();
        QString summary = QString("Клиент: %1<br>Адрес: %2<br><br><b>%3</b>")
                              .arg(clientName, ui->leAddress->text(), ui->lblTotal->text());

        ReportExporter::exportToPdf(QString("Товарный чек №%1").arg(newOrderId), summary, m_cartModel);
    }

    accept();
}
