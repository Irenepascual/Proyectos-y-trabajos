EXPLAIN PLAN FOR SELECT DISTINCT T1.equipo_loc
FROM Partidos T1, Partidos T2, Partidos T3
WHERE (T1.equipo_loc=T2.equipo_loc AND T2.equipo_loc=T3.equipo_loc)
AND T1.denom_ofc_p='2' AND T2.denom_ofc_p='1' AND T3.denom_ofc_p='2'
AND ((T1.inicio_temp_p=2005 AND T2.inicio_temp_p=2006 AND T3.inicio_temp_p=2007)
OR (T1.inicio_temp_p=2006 AND T2.inicio_temp_p=2007 AND T3.inicio_temp_p=2008)
OR (T1.inicio_temp_p=2007 AND T2.inicio_temp_p=2008 AND T3.inicio_temp_p=2009)
OR (T1.inicio_temp_p=2008 AND T2.inicio_temp_p=2009 AND T3.inicio_temp_p=2010)
OR (T1.inicio_temp_p=2009 AND T2.inicio_temp_p=2010 AND T3.inicio_temp_p=2011)
OR (T1.inicio_temp_p=2010 AND T2.inicio_temp_p=2011 AND T3.inicio_temp_p=2012)
OR (T1.inicio_temp_p=2011 AND T2.inicio_temp_p=2012 AND T3.inicio_temp_p=2013)
OR (T1.inicio_temp_p=2012 AND T2.inicio_temp_p=2013 AND T3.inicio_temp_p=2014)
OR (T1.inicio_temp_p=2013 AND T2.inicio_temp_p=2014 AND T3.inicio_temp_p=2015));
