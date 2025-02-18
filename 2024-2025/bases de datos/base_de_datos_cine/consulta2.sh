-- SAGA DE PELICULAS MAS LARGA (EN NUMERO DE PELICULAS), LISTANDO LOS TITULOS DE 
-- LAS PELICULAS QUE LA COMPONEN (INCLUYENDO SECUELAS Y PRECUELAS)

-- 1. Obtenemos la pelicula que mas secuelas y precuelas tiene
CREATE VIEW max (nombre, agno) AS
    SELECT titulo_obra_rela_pr, agno_produc_rela_pr
    FROM pelicula_relac
    GROUP BY titulo_obra_rela_pr, agno_produc_rela_pr
    HAVING COUNT(*) = (
        SELECT MAX(num_veces)
        FROM (
            SELECT titulo_obra_rela_pr, agno_produc_rela_pr, COUNT(*) AS num_veces
            FROM pelicula_relac
            WHERE tipo_pr='secuela' OR tipo_pr='precuela'
            GROUP BY titulo_obra_rela_pr, agno_produc_rela_pr
        )
    );

-- 2. Obtenemos la lista de todas las películas que forman parte de la saga
SELECT M.nombre, M.agno
FROM max M
UNION
SELECT P.titulo_obra_pr, P.agno_produc_pr
FROM max M, pelicula_relac P
WHERE M.nombre = P.titulo_obra_rela_pr AND M.agno = P.agno_produc_rela_pr;

DROP VIEW max;