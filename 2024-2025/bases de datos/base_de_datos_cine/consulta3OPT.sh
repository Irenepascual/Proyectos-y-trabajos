CREATE VIEW solo_peliculas (nombre, titulo, agno) AS
    SELECT DISTINCT T.nom_persona_a, T.titulo_obra_a, T.agno_produc_a
    FROM trabaja_actor T, Pelicula P
    WHERE T.titulo_obra_a = P.titulo_obra_p AND T.agno_produc_a = P.agno_produc_p AND T.agno_produc_a BETWEEN 1980 AND 2010;

CREATE VIEW listado_peliculas_actor (nombre, genero, nacionalidad, titulo, agno) AS
    SELECT S.nombre, P.genero_pers, P.nacionalidad, S.titulo, S.agno
    FROM solo_peliculas S, Personal P
    WHERE S.nombre = P.nom_persona AND P.genero_pers <> 'NULL' AND P.nacionalidad <> 'NULL';

SELECT DISTINCT L1.nombre, L1.nacionalidad, L2.nombre, L2.nacionalidad, COUNT(L1.nombre) numVeces
FROM listado_peliculas_actor L1
JOIN listado_peliculas_actor L2 ON L1.titulo = L2.titulo
WHERE L1.genero = 'm' AND L2.genero = 'f' AND L1.nacionalidad <> L2.nacionalidad
AND NOT EXISTS (
    SELECT 1
    FROM listado_peliculas_actor L3
    WHERE L3.nombre = L1.nombre AND L3.genero = 'm'
    AND NOT EXISTS (
        SELECT 1
        FROM listado_peliculas_actor L4
        WHERE L4.nombre = L2.nombre AND L4.genero = 'f' AND L3.titulo = L4.titulo
    )
)
AND NOT EXISTS (
    SELECT 1
    FROM listado_peliculas_actor L5
    WHERE L5.nombre = L2.nombre AND L5.genero = 'f'
    AND NOT EXISTS (
        SELECT 1
        FROM listado_peliculas_actor L6
        WHERE L6.nombre = L1.nombre AND L6.genero = 'm' AND L5.titulo = L6.titulo
    )
)
GROUP BY (L1.nombre, L1.nacionalidad, L2.nombre, L2.nacionalidad)
ORDER BY numVeces DESC;

DROP VIEW solo_peliculas;
DROP VIEW listado_peliculas_actor;

