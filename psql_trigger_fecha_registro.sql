--TRIGGER: ACTUALIZA CON LA FECHA ACTUAL CUANDO SE REALIZA UN INSERT
CREATE OR REPLACE FUNCTION insertar_fecha() RETURNS TRIGGER AS $$

BEGIN

UPDATE clientes SET fecha_registro = current_date WHERE id_cliente = new.id_cliente;

RETURN NEW;
END
$$
LANGUAGE plpgsql;

CREATE TRIGGER TR_fecha AFTER INSERT ON clientes
FOR EACH ROW
EXECUTE PROCEDURE insertar_fecha();