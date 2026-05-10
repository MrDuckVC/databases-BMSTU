#include "catalogdialog.h"
#include "ui_catalogdialog.h"
#include "utils.h"
#include <QMessageBox>
#include <QSqlError>

CatalogDialog::CatalogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatalogDialog),
    m_model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowTitle("Каталог товаров (Просмотр)");
    resize(800, 500);

    ui->tableView->setModel(m_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    loadData();
}

CatalogDialog::~CatalogDialog() {
    delete ui;
}

void CatalogDialog::loadData(const QString &filter) {
    // Используем VIEW v_stock_balances для получения актуальных остатков и категорий
    QString queryStr = "SELECT category_name AS \"Категория\", product_id AS \"ID Товара\", "
                       "product_name AS \"Наименование\", retail_price AS \"Цена (руб)\", stock AS \"Остаток\" "
                       "FROM v_stock_balances";

    if (!filter.isEmpty()) {
        // Регистронезависимый поиск PostgreSQL по названию или категории
        queryStr += QString(" WHERE product_name ILIKE '%%1%' OR category_name ILIKE '%%1%'").arg(filter);
    }

    queryStr += " ORDER BY category_name, product_name";

    m_model->setQuery(queryStr);

    if (m_model->lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка БД", Utils::formatSqlError(m_model->lastError()));
    } else {
        ui->tableView->hideColumn(1); // Скрываем технический ID
    }
}

void CatalogDialog::on_leSearch_textChanged(const QString &text) {
    loadData(text.trimmed());
}
