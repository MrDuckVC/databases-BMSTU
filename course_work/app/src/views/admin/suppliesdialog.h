#ifndef SUPPLIESDIALOG_H
#define SUPPLIESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui { class SuppliesDialog; }

class SuppliesDialog : public QDialog {
    Q_OBJECT
public:
    explicit SuppliesDialog(QWidget *parent = nullptr);
    ~SuppliesDialog();

private slots:
    void on_btnAddItem_clicked();
    void on_btnRemoveItem_clicked();
    void on_btnProcess_clicked();

private:
    Ui::SuppliesDialog *ui;
    QStandardItemModel *m_supplyModel;

    void loadSuppliers();
    void loadProducts();
};

#endif // SUPPLIESDIALOG_H
