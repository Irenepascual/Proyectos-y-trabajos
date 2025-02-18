CREATE VIEW listado_peliculas_actor (nombre, genero, nacionalidad, titulo, agno) AS
    SELECT P.nom_persona, P.genero_pers, P.nacionalidad, T.titulo_obra_a, T.agno_produc_a
    FROM Personal P
    JOIN trabaja_actor T ON P.nom_persona = T.nom_persona_a
    JOIN Pelicula Pl ON T.titulo_obra_a = Pl.titulo_obra_p AND T.agno_produc_a = Pl.agno_produc_p
    WHERE T.agno_produc_a BETWEEN 1980 AND 2010 AND P.genero_pers <> 'NULL' AND P.nacionalidad <> 'NULL';

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

DROP VIEW listado_peliculas_actor;