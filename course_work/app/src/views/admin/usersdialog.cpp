#include "usersdialog.h"
#include "ui_usersdialog.h"
#include "databasemanager.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlRecord>

UsersDialog::UsersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersDialog)
{
    ui->setupUi(this);
    setWindowTitle("Настройка прав доступа");
    resize(700, 450);

    m_usersModel = new QSqlQueryModel(this);
    ui->tvUsers->setModel(m_usersModel);
    ui->tvUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvUsers->horizontalHeader()->setStretchLastSection(true);

    ui->cbRole->addItems({"Менеджер", "Администратор"});

    loadEmployees();
    updateUsersTable();
}

UsersDialog::~UsersDialog() {
    delete ui;
}

void UsersDialog::loadEmployees() {
    ui->cbEmployees->clear();
    QSqlQuery q("SELECT id, full_name FROM employees ORDER BY full_name");
    while(q.next()) {
        ui->cbEmployees->addItem(q.value(1).toString(), q.value(0).toInt());
    }
}

void UsersDialog::updateUsersTable() {
    m_usersModel->setQuery("SELECT u.id, e.full_name AS \"Сотрудник\", u.login AS \"Логин\", u.role AS \"Роль\" "
                           "FROM app_users u JOIN employees e ON u.employee_id = e.id "
                           "ORDER BY e.full_name");
    ui->tvUsers->hideColumn(0);
}

void UsersDialog::on_btnSave_clicked() {
    if (ui->cbEmployees->currentIndex() == -1) return;

    int empId = ui->cbEmployees->currentData().toInt();
    QString login = ui->leLogin->text().trimmed();
    QString pass = ui->lePassword->text();
    QString role = ui->cbRole->currentText();

    if (login.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Логин не может быть пустым!");
        return;
    }

    // Проверка наличия существующей записи
    QSqlQuery checkQ;
    checkQ.prepare("SELECT id FROM app_users WHERE employee_id = :eid");
    checkQ.bindValue(":eid", empId);
    checkQ.exec();
    bool userExists = checkQ.next();

    if (!userExists && pass.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Для новой учетной записи необходимо задать пароль!");
        return;
    }

    QSqlQuery q;
    if (userExists) {
        if (pass.isEmpty()) {
            q.prepare("UPDATE app_users SET login = :login, role = CAST(:role AS user_role) WHERE employee_id = :eid");
        } else {
            q.prepare("UPDATE app_users SET login = :login, password_hash = :hash, role = CAST(:role AS user_role) WHERE employee_id = :eid");
            q.bindValue(":hash", Utils::hashPassword(pass));
        }
    } else {
        q.prepare("INSERT INTO app_users (login, password_hash, role, employee_id) "
                  "VALUES (:login, :hash, CAST(:role AS user_role), :eid)");
        q.bindValue(":hash", Utils::hashPassword(pass));
    }

    q.bindValue(":login", login);
    q.bindValue(":role", role);
    q.bindValue(":eid", empId);

    if (!q.exec()) {
        QMessageBox::critical(this, "Ошибка БД", Utils::formatSqlError(q.lastError()));
    } else {
        QMessageBox::information(this, "Успех", "Права доступа успешно сохранены!");
        ui->lePassword->clear();
        updateUsersTable();
    }
}

void UsersDialog::on_btnDelete_clicked() {
    int row = ui->tvUsers->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите пользователя для удаления доступа!");
        return;
    }

    int userId = m_usersModel->record(row).value("id").toInt();
    QString empName = m_usersModel->record(row).value("Сотрудник").toString();

    auto res = QMessageBox::question(this, "Подтверждение",
                                     QString("Удалить доступ для сотрудника:\n%1?").arg(empName));

    if (res == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM app_users WHERE id = :id");
        q.bindValue(":id", userId);
        if (!q.exec()) {
            QMessageBox::critical(this, "Ошибка", Utils::formatSqlError(q.lastError()));
        } else {
            updateUsersTable();
        }
    }
}

void UsersDialog::on_tvUsers_clicked(const QModelIndex &index) {
    int row = index.row();
    ui->cbEmployees->setCurrentText(m_usersModel->record(row).value("Сотрудник").toString());
    ui->leLogin->setText(m_usersModel->record(row).value("Логин").toString());
    ui->cbRole->setCurrentText(m_usersModel->record(row).value("Роль").toString());
    ui->lePassword->clear();
}
