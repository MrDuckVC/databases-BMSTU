#ifndef UTILS_H
#define UTILS_H

#include <QCryptographicHash>
#include <QDebug>
#include <QSqlError>
#include <QString>

class Utils {
public:
  static QString formatSqlError(const QSqlError &error) {
    QString text = error.text();

    // Ошибки QSqlTableModel (пустая строка без введенных данных)
    if (text.contains("No Fields to update", Qt::CaseInsensitive)) {
      return "Нет данных для сохранения. Вы добавили пустую строку, но ничего "
             "в неё не ввели.";
    }

    // Нарушения NOT NULL
    if (text.contains("not-null constraint", Qt::CaseInsensitive) ||
        text.contains("null value in column", Qt::CaseInsensitive)) {
      if (text.contains("phone"))
        return "Необходимо указать номер телефона.";
      if (text.contains("full_name"))
        return "ФИО/Наименование не может быть пустым.";
      if (text.contains("supplier_name"))
        return "Укажите название поставщика.";
      if (text.contains("address"))
        return "Адрес доставки обязателен для заполнения.";
      return "Заполните все обязательные поля.";
    }

    // Нарушения UNIQUE (дублирование данных)
    if (text.contains("unique constraint", Qt::CaseInsensitive)) {
      if (text.contains("uq_categories_name"))
        return "Категория с таким названием уже существует.";
      if (text.contains("uq_suppliers_name"))
        return "Поставщик с таким названием уже существует.";
      if (text.contains("uq_suppliers_phone"))
        return "Этот телефон уже закреплен за другим контрагентом.";
      if (text.contains("uq_clients_phone"))
        return "Клиент с таким номером телефона уже есть в базе.";
      if (text.contains("idx_clients_email_lower") ||
          text.contains("idx_suppliers_email_lower"))
        return "Этот e-mail уже используется.";
      if (text.contains("uq_app_users_login"))
        return "Такой логин уже занят другим пользователем.";
      return "Такая запись уже существует в системе.";
    }

    // Нарушения CHECK (неверный формат или бизнес-правила)
    if (text.contains("check constraint", Qt::CaseInsensitive)) {
      if (text.contains("_phone_format"))
        return "Неверный формат телефона. Используйте + и цифры (напр. "
               "+79991234567).";
      if (text.contains("_email_format"))
        return "Введен некорректный формат e-mail.";
      if (text.contains("chk_products_stock"))
        return "Недостаточно товара на складе для совершения операции.";
      if (text.contains("_not_empty"))
        return "Поле не может быть пустым или содержать только пробелы.";
      return "Данные не соответствуют правилам валидации.";
    }

    // Нарушения FOREIGN KEY (целостность связанных данных)
    if (text.contains("foreign key constraint", Qt::CaseInsensitive)) {
      if (text.contains("fk_products_category"))
        return "Нельзя удалить категорию: в ней еще числятся товары.";
      if (text.contains("fk_app_users_employee"))
        return "Нельзя удалить сотрудника: у него есть учетная запись в "
               "системе.";
      if (text.contains("fk_orders_client"))
        return "Нельзя удалить клиента: на него уже оформлены заказы.";
      if (text.contains("fk_order_items_product") ||
          text.contains("fk_supply_items_product"))
        return "Нельзя удалить товар: он фигурирует в истории заказов или "
               "поставок.";
      if (text.contains("fk_supplies_supplier"))
        return "Нельзя удалить поставщика: в базе зафиксированы поставки от "
               "него.";
      return "Нельзя удалить запись: она используется в других разделах "
             "программы.";
    }

    qCritical() << "Unhandled SQL error: " << text;

    // Извлечение чистого текста из пользовательских исключений БД (RAISE
    // EXCEPTION из триггеров)
    int start = text.indexOf("ERROR: ");
    if (start != -1) {
      start += 7;
      int end = text.indexOf("\nCONTEXT:", start);
      if (end == -1)
        end = text.indexOf("(P", start);
      if (end != -1)
        return text.mid(start, end - start).trimmed();
      return text.mid(start).trimmed();
    }

    // Очистка неизвестных ошибок от драйверного префикса QPSQL
    return text.split("QPSQL:").last().trimmed();
  }

  static QString hashPassword(const QString &password) {
    QByteArray hash =
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
  }
};

#endif // UTILS_H
