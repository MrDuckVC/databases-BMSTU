#ifndef ORDERPROCESSINGDIALOG_H
#define ORDERPROCESSINGDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui { class OrderProcessingDialog; }

class OrderProcessingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderProcessingDialog(QWidget *parent = nullptr);
    ~OrderProcessingDialog();

private slots:
    void on_btnUpdateStatus_clicked();
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::OrderProcessingDialog *ui;
    QSqlQueryModel *m_model;
    int m_selectedOrderId = -1;

    void loadOrders();
};

#endif // ORDERPROCESSINGDIALOG_H
