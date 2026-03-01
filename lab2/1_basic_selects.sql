/* Создать к базе данных SELECT-запросы следующих видов: */

/*запрос, выбирающий все данные из таблицы; */
SELECT * FROM products;

/*запрос, выбирающий данные из некоторых столбцов таблицы; */
SELECT p.product_name, p.price FROM products p;

/*запрос с использованием сортировки данных; */
SELECT * FROM products p
ORDER BY p.price DESC;

/*запрос с использованием ограничения на число строк; */
SELECT p.product_name, p.price FROM products p
LIMIT 5;

/*запрос с использованием операторов сравнения; */
SELECT * FROM products p
WHERE p.price >= 10000.00;

/*запрос с использованием оператора BETWEEN; */
SELECT * FROM products p
WHERE p.price BETWEEN 10000.00 AND 100000.00;

/*запрос с использованием оператора IN, содержащий подзапрос; */
SELECT * FROM customers c
WHERE c.id IN (
	SELECT o.customer_id FROM orders o
	WHERE o.status IN ('Adopted', 'Cancelled')
);

/*запрос с использованием оператора LIKE и строковых функций; */
SELECT * FROM customers c
WHERE c.address LIKE 'ул. П%';

/*запрос с использованием предиката IS NULL; */
SELECT * FROM orders o
WHERE o.ship_date IS NULL;

/*запрос с использованием агрегатных функций; */
SELECT o.status, COUNT(*) FROM orders o
GROUP BY o.status;

/* запрос с использованием агрегатных функций и предложения HAVING; */
SELECT o.status, COUNT(*) FROM orders o
GROUP BY o.status
HAVING COUNT(*) < 5;

/*запрос, выбирающий данные из нескольких таблиц с использованием соединения по предикату; */
SELECT p.product_name, i.total, o.status FROM items i
INNER JOIN orders o ON o.id = i.order_id
INNER JOIN products p ON p.id = i.product_id
WHERE o.id = 2;

/*запрос с использованием ключевого слова DISTINCT; */
SELECT DISTINCT c.city FROM customers c
ORDER BY c.city ASC;

/*запрос с использованием оператора EXISTS; */
SELECT * FROM products p
WHERE EXISTS (
	SELECT 1 FROM items i
	WHERE i.product_id = p.id
	GROUP BY i.product_id
	HAVING COUNT(i.product_id) = 2
);

/*запрос с использованием функции CASE; */
SELECT
	o.id, o.customer_id, o.order_date,
	CASE
		WHEN o.status = 'Cancelled' THEN 'Отменен'
		WHEN o.status = 'Processed' THEN 'В обработке'
		WHEN o.status = 'Adopted' THEN 'Принят'
		ELSE 'Неизвестно'
	END AS "Статус заказа"
FROM orders o;
