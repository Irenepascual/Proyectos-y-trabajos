SELECT '2015' AS inicio_temporada_p, num_jornada_p AS jornada, SUM(gol_loc + gol_visit) AS max_goles
FROM Partidos
WHERE inicio_temp_p = 2015
GROUP BY num_jornada_p
HAVING SUM(gol_loc + gol_visit) = (
    SELECT MAX(total_goles) AS max_goles
    FROM (
        SELECT SUM(gol_loc + gol_visit) AS total_goles
        FROM Partidos
        WHERE inicio_temp_p = 2015
        GROUP BY num_jornada_p
    )
)
UNION ALL
SELECT '2014' AS inicio_temporada_p, num_jornada_p AS jornada, SUM(gol_loc + gol_visit) AS max_goles
FROM Partidos
WHERE inicio_temp_p = 2014
GROUP BY num_jornada_p
HAVING SUM(gol_loc + gol_visit) = (
    SELECT MAX(total_goles) AS max_goles
    FROM (
        SELECT SUM(gol_loc + gol_visit) AS total_goles
        FROM Partidos
        WHERE inicio_temp_p = 2014
        GROUP BY num_jornada_p
    )
)
UNION ALL
SELECT '2013' AS inicio_temporada_p, num_jornada_p AS jornada, SUM(gol_loc + gol_visit) AS max_goles
FROM Partidos
WHERE inicio_temp_p = 2013
GROUP BY num_jornada_p
HAVING SUM(gol_loc + gol_visit) = (
    SELECT MAX(total_goles) AS max_goles
    FROM (
        SELECT SUM(gol_loc + gol_visit) AS total_goles
        FROM Partidos
        WHERE inicio_temp_p = 2013
        GROUP BY num_jornada_p
    )
)
UNION ALL
SELECT '2012' AS inicio_temporada_p, num_jornada_p AS jornada, SUM(gol_loc + gol_visit) AS max_goles
FROM Partidos
WHERE inicio_temp_p = 2012
GROUP BY num_jornada_p
HAVING SUM(gol_loc + gol_visit) = (
    SELECT MAX(total_goles) AS max_goles
    FROM (
        SELECT SUM(gol_loc + gol_visit) AS total_goles
        FROM Partidos
        WHERE inicio_temp_p = 2012
        GROUP BY num_jornada_p
    )
)
UNION ALL
SELECT '2011' AS inicio_temporada_p, num_jornada_p AS jornada, SUM(gol_loc + gol_visit) AS max_goles
FROM Partidos
WHERE inicio_temp_p = 2011
GROUP BY num_jornada_p
HAVING SUM(gol_loc + gol_visit) = (
    SELECT MAX(total_goles) AS max_goles
    FROM (
        SELECT SUM(gol_loc + gol_visit) AS total_goles
        FROM Partidos
        WHERE inicio_temp_p = 2011
        GROUP BY num_jornada_p
    )
);
