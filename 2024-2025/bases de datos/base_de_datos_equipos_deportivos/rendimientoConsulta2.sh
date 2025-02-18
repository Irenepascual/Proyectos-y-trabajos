CREATE VIEW enSegunda (nom_equipo,temporada) AS
	SELECT DISTINCT equipo_loc,inicio_temp_p
	FROM Partidos
	WHERE denom_ofc_p='2';

CREATE VIEW enPrimera (nom_equipo,temporada) AS
	SELECT DISTINCT equipo_loc,inicio_temp_p
	FROM Partidos
	WHERE denom_ofc_p='1';

EXPLAIN PLAN FOR SELECT T1.nom_equipo
FROM enSegunda T1, enPrimera T2, enSegunda T3
WHERE (T1.nom_equipo=T2.nom_equipo AND T2.nom_equipo=T3.nom_equipo) AND ((T1.temporada=2005 AND T2.temporada=2006 AND T3.temporada=2007)
OR (T1.temporada=2006 AND T2.temporada=2007 AND T3.temporada=2008)
OR (T1.temporada=2007 AND T2.temporada=2008 AND T3.temporada=2009)
OR (T1.temporada=2008 AND T2.temporada=2009 AND T3.temporada=2010)
OR (T1.temporada=2009 AND T2.temporada=2010 AND T3.temporada=2011)
OR (T1.temporada=2010 AND T2.temporada=2011 AND T3.temporada=2012)
OR (T1.temporada=2011 AND T2.temporada=2012 AND T3.temporada=2013)
OR (T1.temporada=2012 AND T2.temporada=2013 AND T3.temporada=2014)
OR (T1.temporada=2013 AND T2.temporada=2014 AND T3.temporada=2015));

DROP VIEW enSegunda;
DROP VIEW enPrimera;
