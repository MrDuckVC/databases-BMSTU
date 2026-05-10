-- Настройка DCL: создание пользователя и выдача прав
-- Выполнять от имени суперпользователя в контексте БД store_db

CREATE USER shop_app_user WITH PASSWORD 'secure_shop_pass_2026';

GRANT CONNECT ON DATABASE store_db TO shop_app_user;
GRANT USAGE ON SCHEMA public TO shop_app_user;

-- Права на существующие объекты
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO shop_app_user;

-- Права на последовательности (необходимо для работы IDENTITY при генерации ID во время INSERT)
GRANT USAGE, SELECT, UPDATE ON ALL SEQUENCES IN SCHEMA public TO shop_app_user;

-- Автоматическая выдача прав на таблицы и последовательности, которые будут созданы в будущем
ALTER DEFAULT PRIVILEGES IN SCHEMA public
    GRANT SELECT, INSERT, UPDATE, DELETE ON TABLES TO shop_app_user;

ALTER DEFAULT PRIVILEGES IN SCHEMA public
    GRANT USAGE, SELECT, UPDATE ON SEQUENCES TO shop_app_user;
