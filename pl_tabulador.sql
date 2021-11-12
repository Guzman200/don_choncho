
DROP FUNCTION insertar_categoria(nom varchar, por integer, uni integer);
CREATE OR REPLACE FUNCTION insertar_categoria(nom varchar, por integer, uni integer) RETURNS varchar AS $$
DECLARE

obtener_id integer;
BEGIN

    insert into categorias (nombre)values(nom);

    obtener_id:= (select max(id_cat) from categorias);

    insert into tabulador (porcentaje,unidades,id_cat)values(por,uni,obtener_id);
    
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
$$ LANGUAGE plpgsql;
