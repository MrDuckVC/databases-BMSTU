#ifndef REPORTSDIALOG_H
#define REPORTSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QStandardItemModel>

namespace Ui { class ReportsDialog; }

class ReportsDialog : public QDialog
{
    Q_OBJECT

public:
    enum ReportType {
        StockBalances,
        EmployeeKPI,
        OrderStatuses,
        SalesMatrix
    };

    explicit ReportsDialog(ReportType type, QWidget *parent = nullptr);
    ~ReportsDialog();

private slots:
    void on_btnPrint_clicked();

private:
    Ui::ReportsDialog *ui;

    ReportType m_type;
    QSqlQueryModel *m_queryModel;
    QStandardItemModel *m_matrixModel;

    void loadReport(ReportType type);
    void buildSalesMatrix();
    void calculateTotals(ReportType type);
};

#endif // REPORTSDIALOG_H
