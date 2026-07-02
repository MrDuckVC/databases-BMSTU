-- Скрипт создания структуры БД АИС «Магазин бытовых товаров»

SET TIME ZONE 'Europe/Moscow';

-- Пользовательские типы

CREATE TYPE user_role AS ENUM (
    'Администратор',
    'Менеджер'
);

CREATE TYPE order_status_item AS ENUM (
    'Принят',
    'Оформляется',
    'Оплачен',
    'Доставлен',
    'Отменен'
);

-- Таблицы-справочники

CREATE TABLE categories (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    category_name VARCHAR(50) NOT NULL,

    CONSTRAINT uq_categories_name UNIQUE (category_name),
    CONSTRAINT chk_categories_name_not_empty CHECK (trim(category_name) <> '')
);

CREATE TABLE employees (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    position_name VARCHAR(50) NOT NULL,

    CONSTRAINT chk_employees_name_not_empty CHECK (trim(full_name) <> '')
);

CREATE TABLE app_users (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    login VARCHAR(50) NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    role user_role NOT NULL,
    employee_id INT NOT NULL,

    CONSTRAINT uq_app_users_login UNIQUE (login),
    CONSTRAINT uq_app_users_employee UNIQUE (employee_id),
    CONSTRAINT fk_app_users_employee FOREIGN KEY (employee_id)
        REFERENCES employees(id) ON DELETE CASCADE,
    CONSTRAINT chk_app_users_login_not_empty CHECK (trim(login) <> '')
);

CREATE TABLE clients (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    email VARCHAR(50),

    CONSTRAINT uq_clients_phone UNIQUE (phone),
    CONSTRAINT chk_clients_phone_format CHECK (phone ~ '^\+[1-9]\d{6,14}$'),
    CONSTRAINT chk_clients_email_format CHECK (email IS NULL OR email ~* '^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$'),
    CONSTRAINT chk_clients_name_not_empty CHECK (trim(full_name) <> '')
);

-- Индекс для проверки уникальности email без учета регистра
CREATE UNIQUE INDEX idx_clients_email_lower ON clients (LOWER(email));

CREATE TABLE suppliers (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    supplier_name VARCHAR(100) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    email VARCHAR(50),

    CONSTRAINT uq_suppliers_name UNIQUE (supplier_name),
    CONSTRAINT uq_suppliers_phone UNIQUE (phone),
    CONSTRAINT chk_suppliers_phone_format CHECK (phone ~ '^\+[1-9]\d{6,14}$'),
    CONSTRAINT chk_suppliers_email_format CHECK (email IS NULL OR email ~* '^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$'),
    CONSTRAINT chk_suppliers_name_not_empty CHECK (trim(supplier_name) <> '')
);

CREATE UNIQUE INDEX idx_suppliers_email_lower ON suppliers (LOWER(email));

CREATE TABLE products (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    product_name VARCHAR(100) NOT NULL,
    description TEXT,
    retail_price NUMERIC(12,2) NOT NULL,
    stock INT NOT NULL DEFAULT 0,
    category_id INT NOT NULL,

    CONSTRAINT fk_products_category FOREIGN KEY (category_id)
        REFERENCES categories(id) ON DELETE RESTRICT,
    CONSTRAINT chk_products_price CHECK (retail_price > 0.00),
    CONSTRAINT chk_products_stock CHECK (stock >= 0),
    CONSTRAINT chk_products_name_not_empty CHECK (trim(product_name) <> '')
);

CREATE INDEX idx_products_retail_price ON prod-ucts(retail_price);

-- =============================================================================
-- ОПЕРАЦИОННЫЕ ТАБЛИЦЫ (ДОКУМЕНТЫ)
-- =============================================================================

CREATE TABLE orders (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    created_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
    status order_status_item NOT NULL DEFAULT 'Принят',
    address TEXT NOT NULL,
    client_id INT NOT NULL,
    employee_id INT NOT NULL,

    CONSTRAINT fk_orders_client FOREIGN KEY (client_id)
        REFERENCES clients(id) ON DELETE CASCADE,
    CONSTRAINT fk_orders_employee FOREIGN KEY (employee_id)
        REFERENCES employees(id) ON DELETE RESTRICT,
    CONSTRAINT chk_orders_address_not_empty CHECK (trim(address) <> '')
);

CREATE TABLE order_items (
    order_id INT NOT NULL,
    product_id INT NOT NULL,
    quantity INT NOT NULL DEFAULT 1,
    price_at_sale NUMERIC(12,2) NOT NULL,

    PRIMARY KEY (order_id, product_id),
    CONSTRAINT fk_order_items_order FOREIGN KEY (order_id)
        REFERENCES orders(id) ON DELETE CASCADE,
    CONSTRAINT fk_order_items_product FOREIGN KEY (product_id)
        REFERENCES products(id) ON DELETE RESTRICT,
    CONSTRAINT chk_order_items_qty CHECK (quantity > 0),
    CONSTRAINT chk_order_items_price CHECK (price_at_sale > 0.00)
);

CREATE TABLE supplies (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    supplied_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
    supplier_id INT NOT NULL,

    CONSTRAINT fk_supplies_supplier FOREIGN KEY (supplier_id)
        REFERENCES suppliers(id) ON DELETE CASCADE,
    CONSTRAINT chk_supplies_date CHECK (supplied_at <= NOW())
);

CREATE TABLE supply_items (
    supply_id INT NOT NULL,
    product_id INT NOT NULL,
    quantity INT NOT NULL DEFAULT 1,

    PRIMARY KEY (supply_id, product_id),
    CONSTRAINT fk_supply_items_supply FOREIGN KEY (supply_id)
        REFERENCES supplies(id) ON DELETE CASCADE,
    CONSTRAINT fk_supply_items_product FOREIGN KEY (product_id)
        REFERENCES products(id) ON DELETE RESTRICT,
    CONSTRAINT chk_supply_items_qty CHECK (quantity > 0)
);
