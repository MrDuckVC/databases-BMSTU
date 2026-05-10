#include "chartviewdialog.h"
#include "ui_chartviewdialog.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>

ChartViewDialog::ChartViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartViewDialog)
{
    ui->setupUi(this);
    setWindowTitle("Аналитика: Диаграмма выручки");
    resize(800, 500);

    setupChart();
}

ChartViewDialog::~ChartViewDialog() {
    delete ui;
}

void ChartViewDialog::setupChart() {
    QBarSet *set = new QBarSet("Выручка (руб)");
    QStringList categories;
    double maxRevenue = 0;

    // Извлекаем Топ-10 сотрудников по выручке
    QSqlQuery q("SELECT employee_name, total_revenue FROM v_employee_kpi LIMIT 10");
    while(q.next()) {
        QString fullName = q.value(0).toString();
        // Используем только фамилию для читаемости подписей на оси X
        QString lastName = fullName.split(" ").first();
        double revenue = q.value(1).toDouble();

        *set << revenue;
        categories << lastName;

        if (revenue > maxRevenue) maxRevenue = revenue;
    }

    if (categories.isEmpty()) {
        QMessageBox::information(this, "Аналитика", "Нет данных о продажах для построения диаграммы.");
        return;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Топ сотрудников по сумме оформленных заказов");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Настройка осей
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxRevenue * 1.1); // Запас 10% сверху для красоты
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(chartView);
}

void ChartViewDialog::on_btnPrint_clicked() {
    if (ui->chartLayout->count() == 0) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить график", "Диаграмма_выручки.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter(&printer);
    QWidget *chartWidget = ui->chartLayout->itemAt(0)->widget();

    // Расчет коэффициента масштабирования для вписывания в страницу
    double xscale = (double)printer.width() / chartWidget->width();
    double yscale = (double)printer.height() / chartWidget->height();
    double scale = std::min(xscale, yscale);

    painter.scale(scale, scale);
    chartWidget->render(&painter);
    painter.end();

    QMessageBox::information(this, "Экспорт", "График успешно сохранен в PDF.");
}
