CREATE OR REPLACE TRIGGER crearNacionalidad
BEFORE INSERT OR UPDATE ON Personal
FOR EACH ROW
BEGIN
  --Verificar si el lugar de nacimiento no es NULL
  IF :NEW.lugar_nac IS NOT NULL THEN
    --Si el lugar de nacimiento no contiene comas,
    --asignar la cadena completa como la nacionalidad
      IF INSTR(:NEW.lugar_nac, ',') = 0 THEN
        :NEW.nacionalidad := :NEW.lugar_nac;
      ELSE
        --Si el lugar de nacimiento contiene comas,
        --extraer la nacionalidad después de la última coma (con el -1)
        --y empezamos a guardar desde una posición más (+1) para quitar
        --el espacio en blanco
        :NEW.nacionalidad := SUBSTR(:NEW.lugar_nac, INSTR(:NEW.lugar_nac, ',', -1) + 1);
      END IF;
    END IF;
  IF INSTR(:NEW.nacionalidad, '(') > 0 THEN
    :NEW.nacionalidad := SUBSTR(:NEW.nacionalidad, 1, INSTR(:NEW.nacionalidad, '(') -2);
  END IF;
END;
/
