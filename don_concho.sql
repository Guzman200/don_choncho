--
-- PostgreSQL database dump
--

-- Dumped from database version 12.8 (Ubuntu 12.8-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.8 (Ubuntu 12.8-0ubuntu0.20.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: insert_detalleventa(integer, boolean, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.insert_detalleventa(material_id integer, credito boolean, unidadesmat integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE

    venta_ID INTEGER := (SELECT MAX(id_venta) FROM ventas);
    porcentaje_max INTEGER := ( SELECT porcentaje_max FROM tabulador 
                            INNER JOIN categorias ON tabulador.id_cat=categorias.id_cat
                            INNER JOIN materiales ON categorias.id_cat=materiales.id_cat
                            WHERE materiales.id_mat = material_ID);
    tabulador INTEGER := (SELECT unidades FROM tabulador 
                            INNER JOIN categorias ON tabulador.id_cat=categorias.id_cat
                            INNER JOIN materiales ON categorias.id_cat=materiales.id_cat
                            WHERE materiales.id_mat = material_ID );
    contTabulador INTEGER:= 0;
    precioMat INTEGER := (SELECT precio FROM materiales WHERE id_mat=material_ID);
    cliente_ID INTEGER := (SELECT id_cliente FROM ventas WHERE id_venta=venta_ID);
    porcentajeMat INTEGER := 0;
    precioUno FLOAT := 0.00;
    precioDos FLOAT := 0.00;
    precioTemp FLOAT := 0.00;
    precioFinal FLOAT := 0.00;
    

BEGIN

    IF (credito=true) THEN

        INSERT INTO detalle_venta (id_mat, id_venta, porcentaje,precio,unidades) VALUES (material_ID,venta_ID,0,precioMat,unidadesMat);
        UPDATE materiales SET stock=stock-(unidadesMat) WHERE id_mat = (material_ID);
        precioFinal = precioMat*unidadesMat;
        UPDATE ventas SET total_venta=total_venta+(precioFinal) WHERE id_venta = (venta_ID);

    ELSE
    
        PERFORM id_cliente
        FROM clientes
        WHERE NOW()::DATE >= fecha_registro +( interval '1 year')
        AND id_cliente=cliente_ID;
            
        IF FOUND THEN
        
        contTabulador = contTabulador+tabulador;
            WHILE (unidadesMat>=contTabulador)
            LOOP
                IF (porcentaje_max>porcentajeMat) THEN

                    contTabulador = contTabulador+tabulador;
                    porcentajeMat = porcentajeMat+1;
                
                ELSE

                    EXIT;

                END IF;
            END LOOP;

            precioUno = porcentajeMat*precioMat;
            precioDos = precioUno/100;
            precioTemp = precioMat-precioDos;
        
        ELSE
            precioTemp = precioMat;

        END IF;

        INSERT INTO detalle_venta (id_mat, id_venta, porcentaje,precio,unidades) VALUES (material_ID,venta_ID,porcentajeMat,precioTemp,unidadesMat);
        UPDATE materiales SET stock=stock-(unidadesMat) WHERE id_mat = (material_ID);
        precioFinal = precioTemp*unidadesMat;
        UPDATE ventas SET total_venta=total_venta+(precioFinal) WHERE id_venta = (venta_ID);
            
    END IF;
return 0;
END
$$;


ALTER FUNCTION public.insert_detalleventa(material_id integer, credito boolean, unidadesmat integer) OWNER TO postgres;

--
-- Name: insertar_categoria(character varying, integer, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.insertar_categoria(nom character varying, por integer, uni integer) RETURNS character varying
    LANGUAGE plpgsql
    AS $$
DECLARE

obtener_id integer;
BEGIN

    insert into categorias (nombre)values(nom);

    obtener_id:= (select max(id_cat) from categorias);

    insert into tabulador (porcentaje_max,unidades,id_cat)values(por,uni,obtener_id);
    
    RETURN '     CATEGORIA CREADO';
EXCEPTION
WHEN SQLSTATE '23503' THEN
        RETURN 'ERROR, ALGO NO COINCIDE';
    WHEN SQLSTATE '42830' THEN
        RETURN 'ERROR, ALGO NO COINCIDE';
    ROLLBACK;
    COMMIT;


    RETURN 0;
END
$$;


ALTER FUNCTION public.insertar_categoria(nom character varying, por integer, uni integer) OWNER TO postgres;

--
-- Name: val_stock(integer, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.val_stock(material_id integer, cantidad integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE

    stock_actual INTEGER := (SELECT stock FROM materiales WHERE id_mat=material_ID);
    stock_min INTEGER := (SELECT stock_min FROM materiales WHERE id_mat=material_ID);
    restante INTEGER;

BEGIN

    IF (stock_actual>=cantidad) THEN

        restante = stock_actual-cantidad;

        IF (restante>=stock_min) THEN
            return 200; --0 indica que si hay stock suficiente
        ELSE
            return 201; --1 indica que hay stock, pero esta por agotarse
        END IF;
    ELSE
        return 404; --2 indica que no hay stock suficiente
    END IF;
END
$$;


ALTER FUNCTION public.val_stock(material_id integer, cantidad integer) OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: categorias; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.categorias (
    id_cat integer NOT NULL,
    nombre character varying NOT NULL
);


ALTER TABLE public.categorias OWNER TO postgres;

--
-- Name: categorias_id_cat_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.categorias_id_cat_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.categorias_id_cat_seq OWNER TO postgres;

--
-- Name: categorias_id_cat_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.categorias_id_cat_seq OWNED BY public.categorias.id_cat;


--
-- Name: clientes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.clientes (
    id_cliente integer NOT NULL,
    nombres character varying NOT NULL,
    apaterno character varying NOT NULL,
    amaterno character varying NOT NULL,
    telefono character varying NOT NULL,
    fecha_registro date DEFAULT now(),
    limite_credito double precision NOT NULL
);


ALTER TABLE public.clientes OWNER TO postgres;

--
-- Name: clientes_id_cliente_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.clientes_id_cliente_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.clientes_id_cliente_seq OWNER TO postgres;

--
-- Name: clientes_id_cliente_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.clientes_id_cliente_seq OWNED BY public.clientes.id_cliente;


--
-- Name: detalle_venta; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.detalle_venta (
    id_detalle integer NOT NULL,
    id_mat integer NOT NULL,
    id_venta integer NOT NULL,
    porcentaje integer,
    precio double precision NOT NULL,
    unidades integer NOT NULL
);


ALTER TABLE public.detalle_venta OWNER TO postgres;

--
-- Name: detalle_venta_id_detalle_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.detalle_venta_id_detalle_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.detalle_venta_id_detalle_seq OWNER TO postgres;

--
-- Name: detalle_venta_id_detalle_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.detalle_venta_id_detalle_seq OWNED BY public.detalle_venta.id_detalle;


--
-- Name: materiales; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.materiales (
    id_mat integer NOT NULL,
    id_cat integer NOT NULL,
    nombre character varying NOT NULL,
    marca character varying NOT NULL,
    precio double precision NOT NULL,
    stock integer NOT NULL,
    stock_min integer NOT NULL
);


ALTER TABLE public.materiales OWNER TO postgres;

--
-- Name: materiales_id_mat_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.materiales_id_mat_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.materiales_id_mat_seq OWNER TO postgres;

--
-- Name: materiales_id_mat_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.materiales_id_mat_seq OWNED BY public.materiales.id_mat;


--
-- Name: tabulador; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.tabulador (
    id_tabulador integer NOT NULL,
    id_cat integer NOT NULL,
    porcentaje_max integer NOT NULL,
    unidades integer NOT NULL
);


ALTER TABLE public.tabulador OWNER TO postgres;

--
-- Name: tabulador_id_tabulador_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.tabulador_id_tabulador_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tabulador_id_tabulador_seq OWNER TO postgres;

--
-- Name: tabulador_id_tabulador_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.tabulador_id_tabulador_seq OWNED BY public.tabulador.id_tabulador;


--
-- Name: ventas; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.ventas (
    id_venta integer NOT NULL,
    id_cliente integer NOT NULL,
    total_venta double precision,
    credito boolean,
    fecha_registro date DEFAULT now()
);


ALTER TABLE public.ventas OWNER TO postgres;

--
-- Name: ventas_id_venta_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.ventas_id_venta_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.ventas_id_venta_seq OWNER TO postgres;

--
-- Name: ventas_id_venta_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.ventas_id_venta_seq OWNED BY public.ventas.id_venta;


--
-- Name: categorias id_cat; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categorias ALTER COLUMN id_cat SET DEFAULT nextval('public.categorias_id_cat_seq'::regclass);


--
-- Name: clientes id_cliente; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.clientes ALTER COLUMN id_cliente SET DEFAULT nextval('public.clientes_id_cliente_seq'::regclass);


--
-- Name: detalle_venta id_detalle; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.detalle_venta ALTER COLUMN id_detalle SET DEFAULT nextval('public.detalle_venta_id_detalle_seq'::regclass);


--
-- Name: materiales id_mat; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.materiales ALTER COLUMN id_mat SET DEFAULT nextval('public.materiales_id_mat_seq'::regclass);


--
-- Name: tabulador id_tabulador; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tabulador ALTER COLUMN id_tabulador SET DEFAULT nextval('public.tabulador_id_tabulador_seq'::regclass);


--
-- Name: ventas id_venta; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ventas ALTER COLUMN id_venta SET DEFAULT nextval('public.ventas_id_venta_seq'::regclass);


--
-- Data for Name: categorias; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.categorias (id_cat, nombre) FROM stdin;
1	Apagadores
2	Contactos
3	Clavijas
\.


--
-- Data for Name: clientes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.clientes (id_cliente, nombres, apaterno, amaterno, telefono, fecha_registro, limite_credito) FROM stdin;
1	Amado	Rizo	Gordillo	9621234567	2019-01-01	50000
\.


--
-- Data for Name: detalle_venta; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.detalle_venta (id_detalle, id_mat, id_venta, porcentaje, precio, unidades) FROM stdin;
1	1	3	0	20	100
2	1	4	0	20	100
3	1	5	2	19.6	100
4	2	6	0	60	100
5	3	7	0	25	100
6	2	7	0	60	100
7	3	7	0	25	100
8	1	8	0	20	100
9	1	9	2	19.6	100
\.


--
-- Data for Name: materiales; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.materiales (id_mat, id_cat, nombre, marca, precio, stock, stock_min) FROM stdin;
2	2	Contacto doble	IUSA	60	999800	8
3	3	Clavija de lujo	IUSA	25	999800	6
1	1	Apagador sencillo	IUSA	20	999500	6
\.


--
-- Data for Name: tabulador; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.tabulador (id_tabulador, id_cat, porcentaje_max, unidades) FROM stdin;
1	1	10	50
2	2	15	35
3	3	20	70
\.


--
-- Data for Name: ventas; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.ventas (id_venta, id_cliente, total_venta, credito, fecha_registro) FROM stdin;
1	1	0	t	2021-11-11
2	1	0	f	2021-11-11
3	1	2000	t	2021-11-11
4	1	2000	f	2021-11-11
5	1	1960.0000000000002	f	2021-11-11
6	1	6000	f	2021-11-11
7	1	11000	t	2021-11-11
8	1	2000	f	2021-11-11
9	1	1960.0000000000002	f	2021-11-11
\.


--
-- Name: categorias_id_cat_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.categorias_id_cat_seq', 5, true);


--
-- Name: clientes_id_cliente_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.clientes_id_cliente_seq', 2, true);


--
-- Name: detalle_venta_id_detalle_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.detalle_venta_id_detalle_seq', 9, true);


--
-- Name: materiales_id_mat_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.materiales_id_mat_seq', 4, true);


--
-- Name: tabulador_id_tabulador_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.tabulador_id_tabulador_seq', 4, true);


--
-- Name: ventas_id_venta_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.ventas_id_venta_seq', 9, true);


--
-- Name: categorias categorias_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categorias
    ADD CONSTRAINT categorias_pkey PRIMARY KEY (id_cat);


--
-- Name: clientes clientes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.clientes
    ADD CONSTRAINT clientes_pkey PRIMARY KEY (id_cliente);


--
-- Name: detalle_venta detalle_venta_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.detalle_venta
    ADD CONSTRAINT detalle_venta_pkey PRIMARY KEY (id_detalle);


--
-- Name: materiales materiales_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.materiales
    ADD CONSTRAINT materiales_pkey PRIMARY KEY (id_mat);


--
-- Name: tabulador tabulador_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tabulador
    ADD CONSTRAINT tabulador_pkey PRIMARY KEY (id_tabulador);


--
-- Name: ventas ventas_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ventas
    ADD CONSTRAINT ventas_pkey PRIMARY KEY (id_venta);


--
-- Name: detalle_venta detalle_venta_id_mat_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.detalle_venta
    ADD CONSTRAINT detalle_venta_id_mat_fkey FOREIGN KEY (id_mat) REFERENCES public.materiales(id_mat) ON DELETE CASCADE;


--
-- Name: detalle_venta detalle_venta_id_venta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.detalle_venta
    ADD CONSTRAINT detalle_venta_id_venta_fkey FOREIGN KEY (id_venta) REFERENCES public.ventas(id_venta) ON DELETE CASCADE;


--
-- Name: materiales materiales_id_cat_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.materiales
    ADD CONSTRAINT materiales_id_cat_fkey FOREIGN KEY (id_cat) REFERENCES public.categorias(id_cat) ON DELETE CASCADE;


--
-- Name: tabulador tabulador_id_cat_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tabulador
    ADD CONSTRAINT tabulador_id_cat_fkey FOREIGN KEY (id_cat) REFERENCES public.categorias(id_cat) ON DELETE CASCADE;


--
-- Name: ventas ventas_id_cliente_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ventas
    ADD CONSTRAINT ventas_id_cliente_fkey FOREIGN KEY (id_cliente) REFERENCES public.clientes(id_cliente) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

