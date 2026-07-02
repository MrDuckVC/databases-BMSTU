#include "reportexporter.h"
#include <QDateTime>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>

bool ReportExporter::exportToPdf(const QString &title, const QString &summary,
                                 QAbstractItemModel *model, bool isMatrix) {
  if (!model)
    return false;

  QString defaultFileName =
      "Отчет_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") +
      ".pdf";
  QString fileName = QFileDialog::getSaveFileName(
      nullptr, "Сохранить отчет", defaultFileName, "PDF Files (*.pdf)");
  if (fileName.isEmpty())
    return false;

  QPrinter printer(QPrinter::HighResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(fileName);
  printer.setPageOrientation(QPageLayout::Landscape);

  // Формирование шапки документа
  QString html = "<h1 align='center'>" + title + "</h1>";
  html += "<p align='right'>Дата: " +
          QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm") + "</p>";
  html += "<table border='1' cellspacing='0' cellpadding='5' "
          "width='100%'><thead><tr>";

  // Угловая ячейка для матрицы (перекрестного запроса)
  if (isMatrix) {
    html += "<th bgcolor='#e0e0e0'>Товар / Клиент</th>";
  }

  // Заголовки столбцов
  for (int c = 0; c < model->columnCount(); ++c) {
    html += "<th bgcolor='#e0e0e0'>" +
            model->headerData(c, Qt::Horizontal).toString() + "</th>";
  }
  html += "</tr></thead><tbody>";

  // Заполнение данных таблицы
  for (int r = 0; r < model->rowCount(); ++r) {
    html += "<tr>";

    if (isMatrix) {
      html += "<td bgcolor='#f9f9f9'><b>" +
              model->headerData(r, Qt::Vertical).toString() + "</b></td>";
    }

    for (int c = 0; c < model->columnCount(); ++c) {
      html += "<td align='center'>" +
              model->data(model->index(r, c)).toString() + "</td>";
    }

    html += "</tr>";
  }
  html += "</tbody></table>";

  // Блок итогов / дополнительной информации под таблицей
  if (!summary.isEmpty()) {
    html += "<h3 align='right'>" + summary + "</h3>";
  }

  // Рендер HTML в PDF
  QTextDocument document;
  document.setHtml(html);
  document.print(&printer);

  return true;
}
