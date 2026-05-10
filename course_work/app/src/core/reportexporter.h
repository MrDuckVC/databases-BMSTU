#ifndef REPORTEXPORTER_H
#define REPORTEXPORTER_H

#include <QAbstractItemModel>
#include <QString>

class ReportExporter {
public:
    // Экспорт любой табличной модели в PDF файл
    static bool exportToPdf(const QString &title, const QString &summary,
                            QAbstractItemModel *model, bool isMatrix = false);
};

#endif // REPORTEXPORTER_H
