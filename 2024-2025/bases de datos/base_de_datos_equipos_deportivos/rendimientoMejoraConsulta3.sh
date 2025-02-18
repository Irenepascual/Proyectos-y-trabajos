CREATE VIEW numGoles(jornada,temporada,goles) AS
	SELECT num_jornada_p,inicio_temp_p,SUM(gol_loc+gol_visit)
	FROM Partidos
	WHERE inicio_temp_p BETWEEN 2011 AND 2015
	GROUP BY num_jornada_p,inicio_temp_p;

EXPLAIN PLAN FOR SELECT N1.jornada,N1.temporada,N1.goles
FROM numGoles N1
WHERE (N1.temporada,N1.goles) IN (
	SELECT temporada,MAX(goles)
	FROM numGoles
	GROUP BY temporada
)
ORDER BY N1.temporada;

DROP VIEW numGoles;
