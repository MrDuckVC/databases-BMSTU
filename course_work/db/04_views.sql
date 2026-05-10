-- Создание представлений (Views) для аналитики и интерфейса приложения
SET TIME ZONE 'Europe/Moscow';

-- Каталог товаров и текущие остатки на складе
CREATE OR REPLACE VIEW v_stock_balances AS
SELECT
    c.category_name,
    p.id AS product_id,
    p.product_name,
    p.retail_price,
    p.stock
FROM products p
JOIN categories c ON p.category_id = c.id
ORDER BY c.category_name, p.product_name;

-- Перекрестная матрица продаж (учитываются только успешные заказы: Оплачен/Доставлен)
CREATE OR REPLACE VIEW v_sales_matrix AS
SELECT
    cl.full_name AS client_name,
    p.product_name,
    SUM(oi.quantity) AS total_quantity,
    SUM(oi.quantity * oi.price_at_sale) AS total_spent
FROM order_items oi
JOIN orders o ON oi.order_id = o.id
JOIN products p ON oi.product_id = p.id
JOIN clients cl ON o.client_id = cl.id
WHERE o.status IN ('Оплачен', 'Доставлен')
GROUP BY cl.full_name, p.product_name
ORDER BY cl.full_name, p.product_name;

-- КПЭ сотрудников (Аналитика по количеству заказов и принесенной выручке)
CREATE OR REPLACE VIEW v_employee_kpi AS
SELECT
    e.id AS employee_id,
    e.full_name AS employee_name,
    COUNT(DISTINCT o.id) AS orders_count,
    SUM(oi.quantity * oi.price_at_sale) AS total_revenue
FROM employees e
JOIN orders o ON e.id = o.employee_id
JOIN order_items oi ON o.id = oi.order_id
WHERE o.status IN ('Оплачен', 'Доставлен')
GROUP BY e.id, e.full_name
ORDER BY total_revenue DESC;

-- История и статусы заказов
CREATE OR REPLACE VIEW v_order_statuses AS
SELECT
    o.id AS order_id,
    o.created_at,
    cl.full_name AS client_name,
    o.status,
    COALESCE(SUM(oi.quantity * oi.price_at_sale), 0.00) AS order_total
FROM orders o
JOIN clients cl ON o.client_id = cl.id
LEFT JOIN order_items oi ON o.id = oi.order_id
GROUP BY o.id, o.created_at, cl.full_name, o.status
ORDER BY o.created_at DESC;

-- Полная детализация заказа (используется для генерации товарного чека/накладной)
CREATE OR REPLACE VIEW v_order_details AS
SELECT
    o.id AS order_id,
    o.created_at,
    o.status,
    cl.full_name AS client_name,
    e.full_name AS employee_name,
    p.id AS product_id,
    p.product_name,
    oi.quantity,
    oi.price_at_sale,
    (oi.quantity * oi.price_at_sale) AS row_total
FROM order_items oi
JOIN orders o ON oi.order_id = o.id
JOIN products p ON oi.product_id = p.id
JOIN clients cl ON o.client_id = cl.id
JOIN employees e ON o.employee_id = e.id;

-- Выдача прав на чтение представлений пользователю приложения
GRANT SELECT ON v_stock_balances, v_sales_matrix, v_employee_kpi, v_order_statuses, v_order_details TO shop_app_user;
