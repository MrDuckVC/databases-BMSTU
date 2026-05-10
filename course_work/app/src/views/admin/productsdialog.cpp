#include "productsdialog.h"
#include "databasemanager.h"
#include "ui_productsdialog.h"
#include "utils.h"
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QSpinBox>
#include <QSqlError>
#include <QSqlRelation>
#include <QStyledItemDelegate>

// Делегаты для ограничения ввода: только положительные числа
class PositiveDoubleDelegate : public QStyledItemDelegate {
public:
  PositiveDoubleDelegate(QObject *parent = nullptr)
      : QStyledItemDelegate(parent) {}
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                        const QModelIndex &) const override {
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(0.01);
    editor->setMaximum(9999999.99);
    editor->setDecimals(2);
    return editor;
  }
};

class PositiveIntDelegate : public QStyledItemDelegate {
public:
  PositiveIntDelegate(QObject *parent = nullptr)
      : QStyledItemDelegate(parent) {}
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                        const QModelIndex &) const override {
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(999999);
    return editor;
  }
};

ProductsDialog::ProductsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ProductsDialog) {
  ui->setupUi(this);
  setWindowTitle("Управление каталогом товаров");

  setupModel();

  ui->tableView->setModel(m_model);

  // Установка делегатов для выбора категории (Combo) и числовых полей
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
  ui->tableView->setItemDelegateForColumn(3, new PositiveDoubleDelegate(this));
  ui->tableView->setItemDelegateForColumn(4, new PositiveIntDelegate(this));

  ui->tableView->hideColumn(0);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

ProductsDialog::~ProductsDialog() { delete ui; }

void ProductsDialog::setupModel() {
  m_model = new QSqlRelationalTableModel(
      this, DatabaseManager::instance().getDatabase());
  m_model->setTable("products");
  m_model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

  // Связь: в 5-м столбце (category_id) показываем имя из таблицы categories
  m_model->setRelation(5, QSqlRelation("categories", "id", "category_name"));

  m_model->setHeaderData(1, Qt::Horizontal, "Наименование");
  m_model->setHeaderData(2, Qt::Horizontal, "Описание");
  m_model->setHeaderData(3, Qt::Horizontal, "Цена (руб.)");
  m_model->setHeaderData(4, Qt::Horizontal, "Остаток");
  m_model->setHeaderData(5, Qt::Horizontal, "Категория");

  m_model->select();
}

void ProductsDialog::on_btnAdd_clicked() {
  m_model->insertRow(m_model->rowCount());
}

void ProductsDialog::on_btnDelete_clicked() {
  int row = ui->tableView->currentIndex().row();
  if (row >= 0) {
    m_model->removeRow(row);
  } else {
    QMessageBox::warning(this, "Внимание", "Выберите товар для удаления.");
  }
}

void ProductsDialog::on_btnSave_clicked() {
  if (!m_model->submitAll()) {
    QMessageBox::critical(this, "Ошибка сохранения",
                          Utils::formatSqlError(m_model->lastError()));
    m_model->revertAll();
  } else {
    QMessageBox::information(this, "Успех", "Каталог товаров обновлен.");
  }
}

void ProductsDialog::on_leSearch_textChanged(const QString &text) {
  if (text.isEmpty()) {
    m_model->setFilter("");
  } else {
    // ILIKE обеспечивает регистронезависимый поиск в PostgreSQL
    m_model->setFilter(QString("product_name ILIKE '%%1%'").arg(text));
  }
}
