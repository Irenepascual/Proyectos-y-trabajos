CREATE OR REPLACE TRIGGER trigger2
AFTER DELETE ON Estadio
FOR EACH ROW
BEGIN
	UPDATE Equipo
	SET Equipo.nom_estad_e=NULL
	WHERE Equipo.nom_estad_e=:OLD.nom_estad;
END;
/
