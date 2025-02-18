CREATE OR REPLACE TRIGGER trigger3
BEFORE INSERT OR UPDATE ON pelicula_relac
FOR EACH ROW
DECLARE
	contador INTEGER;
BEGIN
	SELECT COUNT(*) INTO contador FROM Pelicula WHERE (:NEW.titulo_obra_pr = titulo_obra_p AND :NEW.agno_produc_pr = agno_produc_p);
	IF contador = 0 THEN
		raise_application_error(-20000,'La película que quieres insertar no está en el sistema');
	END IF;
	SELECT COUNT(*) INTO contador FROM Pelicula WHERE (:NEW.titulo_obra_rela_pr = titulo_obra_p AND :NEW.agno_produc_rela_pr = agno_produc_p);
	IF contador = 0 THEN
		raise_application_error(-20000,'La película que quieres insertar no está en el sistema');
	END IF;
END;
/
