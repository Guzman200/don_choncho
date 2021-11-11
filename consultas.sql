-- OBTENER VENTAS A CREDITO
select *
from ventas v
inner join clientes c on c.id_cliente = v.id_cliente
where v.credito = true;

-- OBTENER VENTAS DE CONTADO
select *
from ventas v
inner join clientes c on c.id_cliente = v.id_cliente
where v.credito = false;

-- OBTENER VENTAS TODAS
select *
from ventas v
inner join clientes c on c.id_cliente = v.id_cliente;

-- OBTENER PRODUCTO MAS VENDIDO
select m.id_mat,  m.nombre, sum(unidades) as cantidad
from detalle_venta dv
inner join materiales m on m.id_mat = dv.id_mat
group by m.id_mat
order by cantidad desc limit 1;

-- OBTENER PRODUCTO MENOS VENDIDO
select m.id_mat,  m.nombre, sum(unidades) as cantidad
from detalle_venta dv
inner join materiales m on m.id_mat = dv.id_mat
group by m.id_mat
order by cantidad limit 1;

-- OBTENER PRODUCTOS CON DESCUENTO
select m.id_mat, m.nombre as material, c.id_cat, c.nombre as categoria, t.porcentaje_max, t.unidades 
from materiales m
inner join categorias c on c.id_cat = m.id_cat 
inner join tabulador t  on t.id_cat = c.id_cat;

-- VER TODOS LOS PRODUCTOS
select m.id_mat, m.nombre as material, m.marca, m.precio, m.stock, m.stock_min ,c.id_cat, c.nombre as categoria, t.porcentaje_max, t.unidades 
from materiales m
inner join categorias c on c.id_cat = m.id_cat 
inner join tabulador t  on t.id_cat = c.id_cat;