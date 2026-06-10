#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "catalogdialog.h"
#include "chartviewdialog.h"
#include "clientsdialog.h"
#include "dictionarydialog.h"
#include "orderprocessingdialog.h"
#include "ordersdialog.h"
#include "productsdialog.h"
#include "reportsdialog.h"
#include "suppliersdialog.h"
#include "suppliesdialog.h"
#include "usersdialog.h"

MainWindow::MainWindow(const QString &userName, const QString &userRole,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_userName(userName),
      m_userRole(userRole) {
  ui->setupUi(this);
  setWindowTitle(QString("АИС Магазин - %1 (%2)").arg(m_userName, m_userRole));
  setupRoleBasedUI();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupRoleBasedUI() {
  ui->gbAdmin->setVisible(false);
  ui->gbManager->setVisible(false);

  if (m_userRole == "Администратор") {
    ui->gbAdmin->setVisible(true);
    ui->gbManager->setVisible(true); // Админ имеет доступ к функциям менеджера
  } else if (m_userRole == "Менеджер") {
    ui->gbManager->setVisible(true);
  }
}

// --- Панель администратора ---

void MainWindow::on_btnCategories_clicked() {
  DictionaryDialog dlg("categories", "Справочник категорий", this);
  dlg.exec();
}

void MainWindow::on_btnProducts_clicked() {
  ProductsDialog dlg(this);
  dlg.exec();
}

void MainWindow::on_btnEmployees_clicked() {
  DictionaryDialog dlg("employees", "Справочник сотрудников", this);
  dlg.exec();
}

void MainWindow::on_btnSuppliers_clicked() {
  SuppliersDialog dlg(this);
  dlg.exec();
}

void MainWindow::on_btnSupplies_clicked() {
  SuppliesDialog dlg(this);
  dlg.exec();
}

void MainWindow::on_btnStockReport_clicked() {
  ReportsDialog dlg(ReportsDialog::StockBalances, this);
  dlg.exec();
}

void MainWindow::on_btnKpi_clicked() {
  ReportsDialog dlg(ReportsDialog::EmployeeKPI, this);
  dlg.exec();
}

void MainWindow::on_btnSalesMatrix_clicked() {
  ReportsDialog dlg(ReportsDialog::SalesMatrix, this);
  dlg.exec();
}

void MainWindow::on_btnOrdersChart_clicked() {
  ChartViewDialog dlg(this);
  dlg.exec();
}

void MainWindow::on_btnUsers_clicked() {
  UsersDialog dlg(this);
  dlg.exec();
}

// --- Панель менеджера ---

void MainWindow::on_btnClients_clicked() {
  ClientsDialog dialog(this);
  dialog.exec();
}

void MainWindow::on_btnCatalog_clicked() {
  CatalogDialog dialog(this);
  dialog.exec();
}

void MainWindow::on_btnOrders_clicked() {
  OrdersDialog dialog(m_userName, this);
  dialog.exec();
}

void MainWindow::on_btnOrderHistory_clicked() {
  OrderProcessingDialog dialog(this);
  dialog.exec();
}
