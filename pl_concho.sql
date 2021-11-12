CREATE OR REPLACE FUNCTION VAL_STOCK (material_ID INTEGER, cantidad INTEGER) RETURNS INTEGER AS $$
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
$$ LANGUAGE plpgsql;




CREATE OR REPLACE FUNCTION Insert_DetalleVenta (material_ID INTEGER, credito BOOLEAN, unidadesMat INTEGER) RETURNS INTEGER AS $$
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
$$ LANGUAGE plpgsql;






CREATE OR REPLACE FUNCTION Insert_DetalleVenta (material_ID INTEGER, credito BOOLEAN, unidadesMat INTEGER) RETURNS INTEGER AS $$
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
$$ LANGUAGE plpgsql;