--------------------------------------------------------------------------

--------------------------CONSULTA 1--------------------------------------
-- utilizado tambien en consulta3
CREATE INDEX ind_pelis_agno ON Pelicula(agno_produc_p);

-- utilizado tambien en consulta3
CREATE MATERIALIZED VIEW pelis_ordenadas REFRESH ON COMMIT AS
SELECT titulo_obra_p, agno_produc_p
FROM Pelicula
ORDER BY agno_produc_p ASC;

-- utilizado tambien en consulta3
CREATE INDEX ind_pelis_ordenadas_titulo ON pelis_ordenadas(titulo_obra_p);

CREATE INDEX ind_trabaja_otros_persona_obra_1 ON trabaja_otros(nom_persona_o, titulo_obra_o);

CREATE MATERIALIZED VIEW nombres_directores REFRESH ON COMMIT AS
SELECT nom_persona_o AS nombre_persona
FROM trabaja_otros
WHERE papel_obra = 'director';

CREATE UNIQUE INDEX ind_nom_persona ON Personal(nom_persona);

CREATE INDEX ind_trabaja_otros_persona_obra_2 ON trabaja_otros(papel_obra);
CREATE INDEX ind_trabaja_actor_persona_obra ON trabaja_actor(nom_persona_a, titulo_obra_a);

--DROP INDEX ind_pelis_agno;
--DROP MATERIALIZED VIEW pelis_ordenadas;
--DROP INDEX ind_pelis_ordenadas_titulo;
--DROP INDEX ind_trabaja_otros_persona_obra_1;
--DROP MATERIALIZED VIEW nombres_directores;
--DROP INDEX ind_nom_persona;
--DROP INDEX ind_trabaja_otros_persona_obra_2;
--DROP INDEX ind_trabaja_actor_persona_obra;

--------------------------------------------------------------------------

--------------------------CONSULTA 2--------------------------------------
CREATE INDEX ind_pelicula_relac ON pelicula_relac(titulo_obra_rela_pr, agno_produc_rela_pr);

--DROP INDEX ind_pelicula_relac;

--------------------------------------------------------------------------

--------------------------CONSULTA 3--------------------------------------
-- utilizamos los 2 primeros indices y la primera vista materializada de la consulta 1