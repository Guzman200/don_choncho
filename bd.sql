CREATE DATABASE don_concho;

--Tabla clientes
CREATE TABLE clientes(

    id_cliente serial PRIMARY KEY,
    nombres varchar NOT NULL,
    aPaterno varchar NOT NULL,
    aMaterno varchar NOT NULL,
    telefono varchar NOT NULL,
    fecha_registro date DEFAULT NOW(),
    limite_credito float NOT NULL

);

INSERT INTO clientes (nombres,aPaterno,aMaterno,telefono,limite_credito) 
VALUES ('Amado','Rizo','Gordillo',9621234567,50000),('Adal','Moreno','Cardenas',962111111,80000);

--Tabla ventas
CREATE TABLE ventas(

    id_venta serial PRIMARY KEY,
    id_cliente integer NOT NULL,
    total_venta float, --Cambio
    credito boolean,
    fecha_registro date  DEFAULT NOW(),
    FOREIGN KEY (id_cliente) REFERENCES clientes(id_cliente)

);

INSERT INTO ventas (id_cliente,total_venta,credito) 
VALUES (1,0,true),(1,0,false);

--Tabla categorias
CREATE TABLE categorias(

    id_cat serial PRIMARY KEY,
    nombre varchar NOT NULL

);

INSERT INTO categorias (nombre) 
VALUES ('Apagadores'),('Contactos'),('Clavijas');


--Tabla materiales
CREATE TABLE materiales(

    id_mat serial PRIMARY KEY,
    id_cat integer NOT NULL,
    nombre varchar NOT NULL,
    marca varchar NOT NULL,
    precio float NOT NULL,
    stock integer NOT NULL,
    stock_min integer NOT NULL,
    FOREIGN KEY (id_cat) REFERENCES categorias(id_cat)

);

INSERT INTO materiales (id_cat,nombre,marca,precio,stock,stock_min) 
VALUES (1,'Apagador sencillo','IUSA',20,50,6),(2,'Contacto doble','IUSA',60,50,8),(3,'Clavija de lujo','IUSA',25,50,6);

--Tabla Tabulador
CREATE TABLE tabulador(

    id_tabulador serial PRIMARY KEY,
    id_cat integer NOT NULL,
    porcentaje_max integer NOT NULL,
    unidades integer NOT NULL,
    FOREIGN KEY (id_cat) REFERENCES categorias(id_cat) ON DELETE CASCADE

);

INSERT INTO tabulador (id_cat,porcentaje_max,unidades) 
VALUES (1,10,50),(2,15,35),(3,20,70);

--Tabla detalle_venta
CREATE TABLE detalle_venta(

    id_detalle serial PRIMARY KEY,
    id_mat integer NOT NULL,
    id_venta integer NOT NULL,
    porcentaje integer,
    precio float NOT NULL,
    unidades integer NOT NULL,
    FOREIGN KEY (id_mat) REFERENCES materiales(id_mat),
    FOREIGN KEY (id_venta) REFERENCES ventas(id_venta)

);