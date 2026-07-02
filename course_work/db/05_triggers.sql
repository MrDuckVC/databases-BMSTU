-- Создание Триггеров (Бизнес-логика)
SET TIME ZONE 'Europe/Moscow';

-- Автоматическая фиксация текущей цены товара при оформлении заказа (защита от махинаций на клиенте)
CREATE OR REPLACE FUNCTION fn_set_order_item_price()
RETURNS TRIGGER AS $$
BEGIN
    SELECT retail_price INTO NEW.price_at_sale
    FROM products
    WHERE id = NEW.product_id;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_set_order_item_price
BEFORE INSERT ON order_items
FOR EACH ROW EXECUTE FUNCTION fn_set_order_item_price();

-- Пересчет остатков на складе при изменении состава заказа
CREATE OR REPLACE FUNCTION fn_update_stock_on_order()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        UPDATE products SET stock = stock - NEW.quantity WHERE id = NEW.product_id;
        RETURN NEW;
    ELSIF TG_OP = 'DELETE' THEN
        UPDATE products SET stock = stock + OLD.quantity WHERE id = OLD.product_id;
        RETURN OLD;
    ELSIF TG_OP = 'UPDATE' THEN
        UPDATE products SET stock = stock + OLD.quantity - NEW.quantity WHERE id = NEW.product_id;
        RETURN NEW;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_update_stock_on_order
AFTER INSERT OR UPDATE OR DELETE ON order_items
FOR EACH ROW EXECUTE FUNCTION fn_update_stock_on_order();

-- Пересчет остатков на складе при поступлении товаров от поставщика
CREATE OR REPLACE FUNCTION fn_update_stock_on_supply()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        UPDATE products SET stock = stock + NEW.quantity WHERE id = NEW.product_id;
        RETURN NEW;
    ELSIF TG_OP = 'DELETE' THEN
        UPDATE products SET stock = stock - OLD.quantity WHERE id = OLD.product_id;
        RETURN OLD;
    ELSIF TG_OP = 'UPDATE' THEN
        UPDATE products SET stock = stock - OLD.quantity + NEW.quantity WHERE id = NEW.product_id;
        RETURN NEW;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_update_stock_on_supply
AFTER INSERT OR UPDATE OR DELETE ON supply_items
FOR EACH ROW EXECUTE FUNCTION fn_update_stock_on_supply();

-- Защита исторических данных (финансовых документов) от удаления
CREATE OR REPLACE FUNCTION fn_protect_history()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_TABLE_NAME = 'orders' THEN
        IF OLD.status IN ('Оплачен', 'Доставлен') THEN
            RAISE EXCEPTION 'Нарушение бизнес-логики: Запрещено удалять оплаченные или доставленные заказы!';
        END IF;
    ELSIF TG_TABLE_NAME = 'supplies' THEN
        RAISE EXCEPTION 'Нарушение бизнес-логики: Запрещено удалять проведенные поставки. Используйте корректирующие проводки.';
    END IF;

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_protect_orders_history
BEFORE DELETE ON orders
FOR EACH ROW EXECUTE FUNCTION fn_protect_history();

CREATE TRIGGER trg_protect_supplies_history
BEFORE DELETE ON supplies
FOR EACH ROW EXECUTE FUNCTION fn_protect_history();
