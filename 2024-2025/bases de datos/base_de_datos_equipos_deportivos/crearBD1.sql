CREATE TABLE Temporada (
        inicio_temp     NUMBER(4) PRIMARY KEY,
        fin_temp        NUMBER(4)
);

CREATE TABLE Division (
        denom_ofc       VARCHAR(7) PRIMARY KEY
);

CREATE TABLE Jornada (
        num_jornada     NUMBER(2),
		inicio_temp_j	NUMBER(4),
		denom_ofc_j		VARCHAR(7),
		FOREIGN KEY (inicio_temp_j) REFERENCES Temporada(inicio_temp),
		FOREIGN KEY (denom_ofc_j) REFERENCES Division(denom_ofc),
		PRIMARY KEY (num_jornada,inicio_temp_j,denom_ofc_j)
);

CREATE TABLE Estadio (
        nom_estad       VARCHAR(50) PRIMARY KEY,
        fecha_inaug     NUMBER(4),
        capacidad       NUMBER(6)
);

CREATE TABLE Equipo (
        nom_corto       VARCHAR(70) PRIMARY KEY,
        nom_ofc         VARCHAR(70),
        nom_hist        VARCHAR(70),
        ciudad          VARCHAR(40),
        fecha_fund      NUMBER(4),
        fecha_fund_leg  NUMBER(4),
        nom_estad_e		VARCHAR(50),
	FOREIGN KEY (nom_estad_e) REFERENCES Estadio(nom_estad)
);

CREATE TABLE Otro_nombre (
		nom_otro		VARCHAR(70) PRIMARY KEY,
		nom_corto_o		VARCHAR(70),
		FOREIGN KEY (nom_corto_o) REFERENCES Equipo(nom_corto)
);

CREATE TABLE Partidos (
		equipo_loc		VARCHAR(70),
		equipo_visit	VARCHAR(70),
		num_jornada_p	NUMBER(2),
		inicio_temp_p	NUMBER(4),
		denom_ofc_p		VARCHAR(7),
		gol_loc			NUMBER(2) NOT NULL,
		gol_visit		NUMBER(2) NOT NULL,
		puntos_loc		NUMBER(1),
		puntos_visit	NUMBER(1),
		FOREIGN KEY (equipo_loc) REFERENCES Equipo(nom_corto),
		FOREIGN KEY (equipo_visit) REFERENCES Equipo(nom_corto),
		FOREIGN KEY (num_jornada_p,inicio_temp_p,denom_ofc_p) REFERENCES Jornada(num_jornada,inicio_temp_j,denom_ofc_j),
		PRIMARY KEY (equipo_loc,equipo_visit,num_jornada_p,inicio_temp_p,denom_ofc_p)
);

