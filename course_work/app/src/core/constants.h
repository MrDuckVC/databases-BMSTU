#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace AppConst {

    // === РОЛИ ПОЛЬЗОВАТЕЛЕЙ ===
    namespace Role {
        inline constexpr const char* Admin = "Администратор";
        inline constexpr const char* Manager = "Менеджер";
    }

    // === БАЗА ДАННЫХ И ОКРУЖЕНИЕ ===
    namespace DB {
        inline constexpr const char* Driver = "QPSQL";
        inline constexpr const char* TblCategories = "categories";
        inline constexpr const char* TblEmployees = "employees";
        inline constexpr const char* TblProducts = "products";
        inline constexpr const char* TblClients = "clients";
        inline constexpr const char* TblSuppliers = "suppliers";
        inline constexpr const char* TblUsers = "app_users";
    }

    namespace Env {
        inline constexpr const char* FileName = ".env";
        inline constexpr const char* DbHost = "DB_HOST";
        inline constexpr const char* DbPort = "DB_PORT";
        inline constexpr const char* DbName = "DB_NAME";
        inline constexpr const char* DbUser = "DB_USER";
        inline constexpr const char* DbPass = "DB_PASS";

        inline constexpr const char* DefaultHost = "localhost";
        inline constexpr const char* DefaultPort = "5432";
        inline constexpr const char* DefaultName = "course_work";
        inline constexpr const char* DefaultUser = "shop_app_user";
    }

    // === ЗАГОЛОВКИ ОКОН ===
    namespace Titles {
        inline constexpr const char* MainWindow = "АИС Магазин - %1 (%2)";
        inline constexpr const char* Categories = "Справочник категорий";
        inline constexpr const char* Employees = "Справочник сотрудников";
        inline constexpr const char* SaveReport = "Сохранить отчет";
    }

    // === НАСТРОЙКИ ОТЧЕТОВ ===
    namespace Report {
        inline constexpr const char* Prefix = "Отчет_";
        inline constexpr const char* PdfExt = ".pdf";
        inline constexpr const char* PdfFilter = "PDF Files (*.pdf)";
        inline constexpr const char* DateFormatFile = "yyyyMMdd_HHmmss";
        inline constexpr const char* DateFormatDisplay = "dd.MM.yyyy HH:mm";
        inline constexpr const char* MatrixCornerLabel = "Товар / Клиент";
        inline constexpr const char* DateLabel = "Дата: ";
    }

    // === СООБЩЕНИЯ ОБ ОШИБКАХ БД (ДЛЯ UTILS) ===
    namespace Errors {
        inline constexpr const char* EmptyRow = "Нет данных для сохранения. Вы добавили пустую строку, но ничего в неё не ввели.";
        inline constexpr const char* PhoneRequired = "Необходимо указать номер телефона.";
        inline constexpr const char* NameRequired = "ФИО/Наименование не может быть пустым.";
        inline constexpr const char* SupplierNameRequired = "Укажите название поставщика.";
        inline constexpr const char* AddressRequired = "Адрес доставки обязателен для заполнения.";
        inline constexpr const char* AllFieldsRequired = "Заполните все обязательные поля.";

        inline constexpr const char* DupCategory = "Категория с таким названием уже существует.";
        inline constexpr const char* DupSupplier = "Поставщик с таким названием уже существует.";
        inline constexpr const char* DupPhoneSupp = "Этот телефон уже закреплен за другим контрагентом.";
        inline constexpr const char* DupPhoneClient = "Клиент с таким номером телефона уже есть в базе.";
        inline constexpr const char* DupEmail = "Этот e-mail уже используется.";
        inline constexpr const char* DupLogin = "Такой логин уже занят другим пользователем.";
        inline constexpr const char* DupGeneric = "Такая запись уже существует в системе.";

        inline constexpr const char* FmtPhone = "Неверный формат телефона. Используйте + и цифры (напр. +79991234567).";
        inline constexpr const char* FmtEmail = "Введен некорректный формат e-mail.";
        inline constexpr const char* StockLow = "Недостаточно товара на складе для совершения операции.";
        inline constexpr const char* FieldEmpty = "Поле не может быть пустым или содержать только пробелы.";
        inline constexpr const char* InvalidData = "Данные не соответствуют правилам валидации.";

        inline constexpr const char* FkCategory = "Нельзя удалить категорию: в ней еще числятся товары.";
        inline constexpr const char* FkEmployee = "Нельзя удалить сотрудника: у него есть учетная запись в системе.";
        inline constexpr const char* FkClient = "Нельзя удалить клиента: на него уже оформлены заказы.";
        inline constexpr const char* FkProduct = "Нельзя удалить товар: он фигурирует в истории заказов или поставок.";
        inline constexpr const char* FkSupplier = "Нельзя удалить поставщика: в базе зафиксированы поставки от него.";
        inline constexpr const char* FkGeneric = "Нельзя удалить запись: она используется в других разделах программы.";
    }

} // namespace AppConst

#endif // CONSTANTS_H
