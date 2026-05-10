#ifndef CLIENTSDIALOG_H
#define CLIENTSDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui { class ClientsDialog; }

class ClientsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientsDialog(QWidget *parent = nullptr);
    ~ClientsDialog();

private slots:
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnSave_clicked();

private:
    Ui::ClientsDialog *ui;
    QSqlTableModel *m_model;
};

#endif // CLIENTSDIALOG_H
