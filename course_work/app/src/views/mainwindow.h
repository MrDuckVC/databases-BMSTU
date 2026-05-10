#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& userName, const QString& userRole, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Блок администратора
    void on_btnCategories_clicked();
    void on_btnProducts_clicked();
    void on_btnEmployees_clicked();
    void on_btnSuppliers_clicked();
    void on_btnSupplies_clicked();
    void on_btnStockReport_clicked();
    void on_btnKpi_clicked();
    void on_btnSalesMatrix_clicked();
    void on_btnOrdersChart_clicked();
    void on_btnUsers_clicked();

    // Блок менеджера
    void on_btnClients_clicked();
    void on_btnCatalog_clicked();
    void on_btnOrders_clicked();
    void on_btnOrderHistory_clicked();

private:
    Ui::MainWindow *ui;
    QString m_userName;
    QString m_userRole;

    void setupRoleBasedUI();
};
#endif // MAINWINDOW_H
