#ifndef ORDERSDIALOG_H
#define ORDERSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui { class OrdersDialog; }

class OrdersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrdersDialog(const QString& employeeName, QWidget *parent = nullptr);
    ~OrdersDialog();

private slots:
    void on_btnAddItem_clicked();
    void on_btnCreateOrder_clicked();
    void on_btnRemoveItem_clicked();

private:
    Ui::OrdersDialog *ui;
    QStandardItemModel *m_cartModel;
    QString m_employeeName;

    void loadClients();
    void loadProducts();
    void updateCartTotal();
};

#endif // ORDERSDIALOG_H
