#ifndef CHARTVIEWDIALOG_H
#define CHARTVIEWDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

namespace Ui { class ChartViewDialog; }

class ChartViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartViewDialog(QWidget *parent = nullptr);
    ~ChartViewDialog();

private slots:
    void on_btnPrint_clicked();

private:
    Ui::ChartViewDialog *ui;
    void setupChart();
};

#endif // CHARTVIEWDIALOG_H
