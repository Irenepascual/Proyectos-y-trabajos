CREATE OR REPLACE TRIGGER trigger2
BEFORE INSERT OR UPDATE ON Serie
FOR EACH ROW
BEGIN
	IF :NEW.fin_emision <> 'NULL' THEN
		IF :NEW.ini_emision > :NEW.fin_emision THEN
        		raise_application_error(-20000,'El año ini_emision debe ser menor o igual que fin_emision');
    		END IF;
	END IF;
END;
/
