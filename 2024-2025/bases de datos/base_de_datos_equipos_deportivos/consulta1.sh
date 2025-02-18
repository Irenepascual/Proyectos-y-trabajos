--Obtenemos los puntos de cada equipo local en cada temporada en primera division
CREATE VIEW PuntosLocal AS
        SELECT equipo_loc,inicio_temp_p,SUM(puntos_loc) AS puntos_loc
        FROM Partidos
	WHERE denom_ofc_p='1'
        GROUP BY equipo_loc,inicio_temp_p;

--Obtenemos los puntos de cada equipo visitante en cada temporada en primera division
CREATE VIEW PuntosVisitante AS
	SELECT equipo_visit,inicio_temp_p,SUM(puntos_visit) AS puntos_visit
	FROM Partidos
	WHERE denom_ofc_p='1'
	GROUP BY equipo_visit,inicio_temp_p;

--Obtenemos los puntos totales de cada equipo en cada temporada en primera division
CREATE VIEW clasGeneral (nom_equipo,temporada, puntos_total) AS
	SELECT P1.equipo_loc,P1.inicio_temp_p,SUM(P1.puntos_loc)+SUM(P2.puntos_visit)
	FROM PuntosLocal P1, PuntosVisitante P2
	WHERE P1.inicio_temp_p = P2.inicio_temp_p AND P1.equipo_loc = P2.equipo_visit
	GROUP BY P1.inicio_temp_p,P1.equipo_loc;

--Obtenemos el equipo campeon de cada temporada en primera division
CREATE VIEW clasMax (nom_equipo,temporada,puntos_max) AS
	SELECT C1.nom_equipo,C1.temporada,C1.puntos_total
	FROM clasGeneral C1
	WHERE (C1.temporada,C1.puntos_total) IN (
    		SELECT temporada, MAX(puntos_total)
    		FROM clasGeneral
    		GROUP BY temporada
	);

--Obtenemos el numero de veces que cada equipo ha ganado la liga de primera division
CREATE VIEW clasVictorias (nom_equipo,num_vict) AS
	SELECT nom_equipo,COUNT(nom_equipo)
	FROM clasMax
	GROUP BY nom_equipo;

--Obtenemos el equipo que mmas ligas ha ganado de primera division
SELECT nom_equipo AS equipo,MAX(num_vict) AS num_Victorias
FROM clasVictorias
GROUP BY nom_equipo
HAVING MAX(num_vict) = (
	SELECT MAX(num_vict)
	FROM clasVictorias
);

DROP VIEW PuntosLocal;
DROP VIEW PuntosVisitante;
DROP VIEW clasGeneral;
DROP VIEW clasMax;
DROP VIEW clasVictorias;
