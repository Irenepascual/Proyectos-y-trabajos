CREATE VIEW Directores AS
   SELECT DISTINCT nom_persona_o AS nombre_persona
   FROM trabaja_otros
   WHERE papel_obra = 'director';

CREATE VIEW TodasPelisDirector AS
   SELECT p.titulo_obra_p AS titulo_peli, p.agno_produc_p AS agno_peli, pers.nom_persona AS nombre_persona
   FROM (
      SELECT nom_persona_o AS nom_persona, titulo_obra_o AS titulo_obra
      FROM trabaja_otros
      UNION
      SELECT nom_persona_a AS nom_persona, titulo_obra_a AS titulo_obra
      FROM trabaja_actor
   ) pers
   JOIN Directores d ON d.nombre_persona = pers.nom_persona
   JOIN Pelicula p ON pers.titulo_obra = p.titulo_obra_p;

CREATE VIEW UltimaPeliDirector AS
   SELECT tpd.nombre_persona AS nombre_persona, tpd.titulo_peli AS titulo_peli
   FROM TodasPelisDirector tpd
   JOIN (
      SELECT MAX(agno_peli) AS max_agno_peli, nombre_persona
      FROM TodasPelisDirector
      GROUP BY nombre_persona
   ) max_por_persona ON tpd.nombre_persona = max_por_persona.nombre_persona AND tpd.agno_peli = max_por_persona.max_agno_peli;

SELECT DISTINCT upd.nombre_persona AS nombre_persona, upd.titulo_peli AS titulo_peli
FROM UltimaPeliDirector upd
JOIN trabaja_actor ac ON upd.nombre_persona = ac.nom_persona_a AND upd.titulo_peli = ac.titulo_obra_a;


