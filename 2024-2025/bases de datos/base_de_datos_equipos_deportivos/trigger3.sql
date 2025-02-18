CREATE OR REPLACE TRIGGER trigger3
BEFORE INSERT OR UPDATE ON Partidos
FOR EACH ROW
BEGIN
  IF(:NEW.gol_loc<0 OR :NEW.gol_visit<0) THEN
    raise_application_error(-20000,'Los valores deben ser positivos');
  END IF;
  IF :NEW.gol_loc > :NEW.gol_visit THEN
    :NEW.puntos_loc:=3;
    :NEW.puntos_visit:=0;
  ELSIF :NEW.gol_loc < :NEW.gol_visit THEN
    :NEW.puntos_loc:=0;
    :NEW.puntos_visit:=3;
  ELSE
    :NEW.puntos_loc:=1;
    :NEW.puntos_visit:=1;
  END IF;
END;
/
