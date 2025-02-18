CREATE OR REPLACE TRIGGER trigger1
BEFORE INSERT ON Temporada
FOR EACH ROW
	WHEN (NEW.inicio_temp+1!=NEW.fin_temp)
BEGIN
	raise_application_error(-20000,'el valor de inicio de temporada debe ser 1 a√o menor al valor de fin');
END;
/
