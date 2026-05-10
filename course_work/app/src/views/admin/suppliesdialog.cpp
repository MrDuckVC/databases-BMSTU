#include "suppliesdialog.h"
#include "ui_suppliesdialog.h"
#include "databasemanager.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

SuppliesDialog::SuppliesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SuppliesDialog)
{
    ui->setupUi(this);
    setWindowTitle("Регистрация поставки");

    m_supplyModel = new QStandardItemModel(0, 3, this);
    m_supplyModel->setHorizontalHeaderLabels({"ID Товара", "Наименование", "Кол-во"});

    ui->tvSupply->setModel(m_supplyModel);
    ui->tvSupply->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvSupply->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvSupply->horizontalHeader()->setStretchLastSection(true);

    loadSuppliers();
    loadProducts();
}

SuppliesDialog::~SuppliesDialog() {
    delete ui;
}

void SuppliesDialog::loadSuppliers() {
    ui->cbSuppliers->clear();
    QSqlQuery q("SELECT id, supplier_name FROM suppliers ORDER BY supplier_name");
    while(q.next()) {
        ui->cbSuppliers->addItem(q.value(1).toString(), q.value(0));
    }
}

void SuppliesDialog::loadProducts() {
    ui->cbProducts->clear();
    QSqlQuery q("SELECT id, product_name FROM products ORDER BY product_name");
    while(q.next()) {
        ui->cbProducts->addItem(q.value(1).toString(), q.value(0).toInt());
    }
}

// Формирование временного списка (корзины) поставки
void SuppliesDialog::on_btnAddItem_clicked() {
    if (ui->cbProducts->currentIndex() == -1) return;

    int productId = ui->cbProducts->currentData().toInt();
    QString name = ui->cbProducts->currentText();
    int qty = ui->sbQty->value();

    for (int i = 0; i < m_supplyModel->rowCount(); ++i) {
        if (m_supplyModel->item(i, 0)->text() == QString::number(productId)) {
            QMessageBox::warning(this, "Внимание", "Этот товар уже в списке!");
            return;
        }
    }

    QList<QStandardItem*> row;
    row << new QStandardItem(QString::number(productId))
        << new QStandardItem(name)
        << new QStandardItem(QString::number(qty));
    m_supplyModel->appendRow(row);
}

void SuppliesDialog::on_btnRemoveItem_clicked() {
    int row = ui->tvSupply->currentIndex().row();
    if (row >= 0) {
        m_supplyModel->removeRow(row);
    } else {
        QMessageBox::warning(this, "Внимание", "Выберите товар для удаления из списка!");
    }
}

// Проведение поставки в БД через транзакцию
void SuppliesDialog::on_btnProcess_clicked() {
    if (m_supplyModel->rowCount() == 0 || ui->cbSuppliers->currentIndex() == -1) {
        QMessageBox::warning(this, "Ошибка", "Проверьте выбор поставщика и список товаров!");
        return;
    }

    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    db.transaction();

    QSqlQuery q;

    // 1. Создание записи о поставке (получаем ID через RETURNING)
    q.prepare("INSERT INTO supplies (supplier_id) VALUES (:sid) RETURNING id");
    q.bindValue(":sid", ui->cbSuppliers->currentData().toInt());

    if (!q.exec() || !q.next()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка", Utils::formatSqlError(q.lastError()));
        return;
    }

    int supplyId = q.value(0).toInt();

    // 2. Добавление позиций поставки
    q.prepare("INSERT INTO supply_items (supply_id, product_id, quantity) VALUES (:sid, :pid, :qty)");
    for (int i = 0; i < m_supplyModel->rowCount(); ++i) {
        q.bindValue(":sid", supplyId);
        q.bindValue(":pid", m_supplyModel->item(i, 0)->text().toInt());
        q.bindValue(":qty", m_supplyModel->item(i, 2)->text().toInt());

        if (!q.exec()) {
            db.rollback();
            QMessageBox::critical(this, "Ошибка", Utils::formatSqlError(q.lastError()));
            return;
        }
    }

    if (db.commit()) {
        QMessageBox::information(this, "Успех", "Поставка проведена. Склад обновлен.");
        accept();
    } else {
        db.rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось зафиксировать транзакцию.");
    }
}
