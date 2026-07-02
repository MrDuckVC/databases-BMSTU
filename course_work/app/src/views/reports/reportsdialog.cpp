#include "reportsdialog.h"
#include "reportexporter.h"
#include "ui_reportsdialog.h"
#include "utils.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

ReportsDialog::ReportsDialog(ReportType type, QWidget *parent)
    : QDialog(parent), ui(new Ui::ReportsDialog), m_type(type),
      m_queryModel(new QSqlQueryModel(this)),
      m_matrixModel(new QStandardItemModel(this)) {
  ui->setupUi(this);
  resize(900, 500);

  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);

  loadReport(type);
}

ReportsDialog::~ReportsDialog() { delete ui; }

void ReportsDialog::loadReport(ReportType type) {
  QString queryStr;

  if (type == SalesMatrix) {
    setWindowTitle("Матрица продаж (Топ клиентов)");
    buildSalesMatrix();
    ui->lblSummary->hide();
    return;
  }

  ui->tableView->setModel(m_queryModel);

  switch (type) {
  case StockBalances:
    setWindowTitle("Каталог и остатки на складе");
    queryStr =
        "SELECT category_name AS \"Категория\", product_id AS \"ID Товара\", "
        "product_name AS \"Наименование\", retail_price AS \"Цена\", stock AS "
        "\"Остаток\" "
        "FROM v_stock_balances";
    break;

  case EmployeeKPI:
    setWindowTitle("КПЭ Сотрудников (Аналитика)");
    queryStr = "SELECT employee_name AS \"ФИО Сотрудника\", orders_count AS "
               "\"Кол-во заказов\", "
               "total_revenue AS \"Принесенная выручка (руб)\" "
               "FROM v_employee_kpi";
    break;

  case OrderStatuses:
    setWindowTitle("История и статусы заказов");
    queryStr =
        "SELECT order_id AS \"№ Заказа\", created_at AS \"Дата оформления\", "
        "client_name AS \"Клиент\", status AS \"Статус\", order_total AS "
        "\"Сумма (руб)\" "
        "FROM v_order_statuses";
    break;
  default:
    break;
  }

  m_queryModel->setQuery(queryStr);

  if (m_queryModel->lastError().isValid()) {
    QMessageBox::critical(this, "Ошибка построения отчета",
                          Utils::formatSqlError(m_queryModel->lastError()));
  } else {
    calculateTotals(type);
  }
}

void ReportsDialog::calculateTotals(ReportType type) {
  ui->lblSummary->clear();
  ui->lblSummary->hide();

  if (type == EmployeeKPI) {
    int totalOrders = 0;
    double totalRevenue = 0.0;

    for (int row = 0; row < m_queryModel->rowCount(); ++row) {
      totalOrders += m_queryModel->data(m_queryModel->index(row, 1)).toInt();
      totalRevenue +=
          m_queryModel->data(m_queryModel->index(row, 2)).toDouble();
    }

    ui->lblSummary->setText(
        QString("ИТОГО: Заказов — %1 шт. | Общая выручка — %2 руб.")
            .arg(totalOrders)
            .arg(totalRevenue, 0, 'f', 2));
    ui->lblSummary->show();
  } else if (type == StockBalances) {
    int totalStock = 0;
    double totalValue = 0.0;

    for (int row = 0; row < m_queryModel->rowCount(); ++row) {
      double price = m_queryModel->data(m_queryModel->index(row, 3)).toDouble();
      int qty = m_queryModel->data(m_queryModel->index(row, 4)).toInt();
      totalStock += qty;
      totalValue += (price * qty);
    }

    ui->lblSummary->setText(
        QString("ИТОГО: Товарных единиц — %1 шт. | Стоимость склада — %2 руб.")
            .arg(totalStock)
            .arg(totalValue, 0, 'f', 2));
    ui->lblSummary->show();
  }
}

// Логика построения Pivot Table (Сводной таблицы) вручную
void ReportsDialog::buildSalesMatrix() {
  ui->tableView->setModel(m_matrixModel);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

  // 1. Получаем Топ-7 клиентов по обороту (Столбцы)
  QSqlQuery qClients(
      "SELECT client_name, SUM(total_spent) as spent "
      "FROM v_sales_matrix GROUP BY client_name ORDER BY spent DESC LIMIT 7");
  QStringList topClients;
  while (qClients.next()) {
    QString fullName = qClients.value(0).toString();
    QStringList parts = fullName.split(" ");
    QString shortName =
        parts[0] + (parts.size() > 1 ? " " + parts[1].left(1) + "." : "");
    topClients << shortName;
  }

  if (topClients.isEmpty())
    return;

  // 2. Получаем список товаров (Строки)
  QSqlQuery qProducts(
      "SELECT DISTINCT product_name FROM v_sales_matrix ORDER BY product_name");
  QStringList products;
  while (qProducts.next()) {
    products << qProducts.value(0).toString();
  }

  m_matrixModel->setRowCount(products.size());
  m_matrixModel->setColumnCount(topClients.size());
  m_matrixModel->setHorizontalHeaderLabels(topClients);
  m_matrixModel->setVerticalHeaderLabels(products);

  // Инициализация сетки прочерками
  for (int r = 0; r < products.size(); ++r) {
    for (int c = 0; c < topClients.size(); ++c) {
      QStandardItem *item = new QStandardItem("-");
      item->setTextAlignment(Qt::AlignCenter);
      m_matrixModel->setItem(r, c, item);
    }
  }

  // 3. Расстановка данных о проданном количестве
  QSqlQuery qData(
      "SELECT client_name, product_name, total_quantity FROM v_sales_matrix");
  while (qData.next()) {
    QString fullName = qData.value(0).toString();
    QStringList parts = fullName.split(" ");
    QString shortName =
        parts[0] + (parts.size() > 1 ? " " + parts[1].left(1) + "." : "");

    QString pName = qData.value(1).toString();
    int qty = qData.value(2).toInt();

    int col = topClients.indexOf(shortName);
    int row = products.indexOf(pName);

    if (col != -1 && row != -1) {
      QStandardItem *item = new QStandardItem(QString::number(qty));
      item->setTextAlignment(Qt::AlignCenter);
      item->setBackground(
          QBrush(QColor(230, 255, 230))); // Подсветка ячейки с продажей
      m_matrixModel->setItem(row, col, item);
    }
  }
}

void ReportsDialog::on_btnPrint_clicked() {
  ReportExporter::exportToPdf(windowTitle(), ui->lblSummary->text(),
                              ui->tableView->model(), (m_type == SalesMatrix));
}
