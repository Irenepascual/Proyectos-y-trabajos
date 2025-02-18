CREATE VIEW Directores AS
   SELECT DISTINCT nom_persona_o AS nombre_persona
   FROM trabaja_otros
   WHERE papel_obra = 'director';

CREATE VIEW UltimaPeliDirector AS
   SELECT po.titulo_obra_p AS titulo_peli, po.agno_produc_p AS agno_peli, pers.nom_persona AS nombre_persona
   FROM otrosYactores pers
   JOIN nombres_directores d ON d.nombre_persona = pers.nom_persona
   JOIN pelis_ordenadas po ON pers.titulo_obra = po.titulo_obra_p
   WHERE po.agno_produc_p = (
      SELECT MAX(po2.agno_produc_p)
      FROM pelis_ordenadas po2
      JOIN otrosYactores pers2 ON pers2.titulo_obra = po2.titulo_obra_p
      WHERE pers2.nom_persona = pers.nom_persona
   );

SELECT DISTINCT upd.nombre_persona AS nombre_persona, upd.titulo_peli AS titulo_peli
FROM UltimaPeliDirector upd
JOIN trabaja_actor ac ON upd.nombre_persona = ac.nom_persona_a AND upd.titulo_peli = ac.titulo_obra_a;
