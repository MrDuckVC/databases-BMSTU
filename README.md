# Базы данных ИУ5 2 курс 2026

> Репозиторий для выполнения лабораторных работ и курсовой работы в рамках курса "Базы данных" кафедры ИУ5.

---

## 🛠 Технологический стек

![C++17](https://img.shields.io/badge/C++17-00599C?style=flat&logo=cplusplus&logoColor=white)
![Qt 5.12+](https://img.shields.io/badge/Qt_5.12+-41CD52?style=flat&logo=qt&logoColor=white)
![CMake 3.14+](https://img.shields.io/badge/CMake_3.14+-064F8C?style=flat&logo=cmake&logoColor=white)
![PostgreSQL 14+](https://img.shields.io/badge/PostgreSQL_14+-336791?style=flat&logo=postgresql&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=flat&logo=docker&logoColor=white)
![Docker Compose](https://img.shields.io/badge/Docker_Compose-2496ED?style=flat&logo=docker&logoColor=white)

---

## 📁 Структура репозитория

| Директория / Файл | Описание |
| --- | --- |
| **`course_work/`** | Итоговая курсовая работа: **АИС «Магазин бытовых товаров»**. Включает серверные SQL-скрипты (DDL, DML, триггеры, представления) и полноценное клиентское desktop-приложение на C++/Qt. [Подробнее в README курсовой](./course_work/README.md). |
| **`lab1/`** | Лабораторная работа 1: Создание базы данных в СУБД PostgreSQL. Основы программирования на языке SQL. |
| **`lab2/`** | Лабораторная работа 2: Основы программирования на языке SQL. SELECT для СУБД PostgreSQL. |
| **`lab3/`** | Лабораторная работа 3: Использование представлений в СУБД PostgreSQL. |
| **`lab5/`** | Лабораторная работа 5: Использование пользовательских процедур, функций и триггеров в PostgreSQL. |
| **`lab6/`** | Лабораторная работа 6: Экспорт и импорт данных, оконные функции. |
| **`assets/`** | Каталог с графическими материалами для оформления всех `README.md`. |
| **`docker/`** | Локальные тома (volumes) для сохранения данных СУБД и сессий pgAdmin. |
| **`docker-compose.yml`** | Конфигурационный файл для быстрого развертывания инфраструктуры в контейнерах. |

---

## 🐳 Локальное развертывание инфраструктуры

Для выполнения лабораторных работ и запуска серверной части курсового проекта используется Docker. В файле `docker-compose.yml` настроены два сервиса:

* **PostgreSQL 17** (порт 5432) — ядро базы данных.
* **pgAdmin 4** (порт 8081) — веб-интерфейс для администрирования.

### Шаги для запуска:

1. Создайте файл `.env` в корне репозитория (рядом с `docker-compose.yml`) и задайте базовые переменные окружения:

    ```env
    COMPOSE_PROJECT_NAME=databases_bmstu

    POSTGRES_USER=postgres
    POSTGRES_PASSWORD=your_secure_password
    POSTGRES_DB=store_db
    TZ=Europe/Moscow

    PGADMIN_DEFAULT_EMAIL=admin@test.com
    PGADMIN_DEFAULT_PASSWORD=admin_password
    ```

    Пример `.env` файла находится в файле `.env.dev`.

2. Запустите контейнеры в фоновом режиме:

    ```bash
    docker-compose up -d
    ```


3. Для управления базами данных откройте браузер по адресу `http://localhost:8081` и авторизуйтесь в pgAdmin, используя email и пароль из файла `.env`. Данные сохраняются локально в директории `docker/`.

---

## 🛡 Лицензия

Программный код и SQL-скрипты в данном репозитории распространяются под лицензией **GNU General Public License v3.0 (GPLv3)**. Подробности смотрите в файле `LICENSE`.

Разработано в образовательных целях.

**МГТУ им. Н.Э. Баумана, Кафедра ИУ5, 2026 г.**
