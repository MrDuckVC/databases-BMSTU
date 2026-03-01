/* Получить список заказов от компании «НАЗВАНИЕ КОМПАНИИ». Список отсортировать по дате заказа. */

SELECT o.id AS "Order ID", c.company_name AS "Comp name", o.order_date AS "Order date", o.status AS "Status" FROM orders o
INNER JOIN customers c ON c.id = o.customer_id
WHERE c.company_name = 'Zarya LLC'
ORDER BY o.order_date DESC;

/* Вывести десять самых заказываемых продуктов. (учитывать общее количество) */

SELECT p.id AS "Product ID", p.product_name AS "Product name", SUM(i.quantity) AS "Order amount" FROM products p
LEFT JOIN items i ON i.product_id = p.id
GROUP BY p.id, p.product_name
ORDER BY SUM(i.quantity) DESC
LIMIT 10;
