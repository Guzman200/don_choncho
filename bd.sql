CREATE DATABASE don_concho;

--Tabla clientes
CREATE TABLE clientes(

    id_cliente serial PRIMARY KEY,
    nombres varchar NOT NULL,
    aPaterno varchar NOT NULL,
    aMaterno varchar NOT NULL,
    telefono varchar NOT NULL,
    fecha_registro date,
    limite_credito float NOT NULL

);

--Tabla ventas 
CREATE TABLE ventas(

    id_venta serial PRIMARY KEY,
    id_cliente integer NOT NULL,
    total_venta float NOT NULL,
    credito boolean,
    FOREIGN KEY (id_cliente) REFERENCES clientes(id_cliente)

);

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

--Tabla Tabulador
CREATE TABLE tabulador(

    id_tabulador serial PRIMARY KEY,
    id_cat integer NOT NULL,
    porcentaje integer NOT NULL,
    unidades integer NOT NULL,
    FOREIGN KEY (id_cat) REFERENCES categorias(id_cat)

);

--Tabla detalle_venta
CREATE TABLE detalle_venta(

    id_detalle serial PRIMARY KEY,
    id_mat integer NOT NULL,
    id_venta integer NOT NULL,
    porcentaje integer,
    precio integer NOT NULL,
    unidades integer NOT NULL,
    FOREIGN KEY (id_mat) REFERENCES materiales(id_mat),
    FOREIGN KEY (id_venta) REFERENCES ventas(id_venta)

);