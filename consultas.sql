-- OBTENER VENTAS A CREDITO
select *
from ventas v
inner join clientes c on c.id_cliente = v.id_cliente
where v.credito = true;

-- OBTENER VENTAS DE CONTADO
select v.id_venta, v.total_venta, CONCAT(c.nombres, ' ',c.aPaterno, ' ', c.aMaterno) as cliente,
    CASE
        WHEN v.credito THEN 'SI'
        ELSE 'NO'
    END 
    AS credito
from ventas v
inner join clientes c on c.id_cliente = v.id_cliente
where v.credito = false;

-- OBTENER VENTAS TODAS
select v.id_venta, v.total_venta, CONCAT(c.nombres, ' ',c.aPaterno, ' ', c.aMaterno) as cliente,
    CASE
        WHEN v.credito THEN 'SI'
        ELSE 'NO'
    END 
    AS credito
from ventas v
inner join clientes c on c.id_cliente = v.id_cliente;
