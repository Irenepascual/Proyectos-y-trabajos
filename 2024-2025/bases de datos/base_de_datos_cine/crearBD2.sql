CREATE TABLE Obra (
	titulo_obra		VARCHAR(165),
	agno_produc		NUMBER(4),
	PRIMARY KEY (titulo_obra, agno_produc)
);

CREATE TABLE Pelicula (
	titulo_obra_p		VARCHAR(165),
	agno_produc_p		NUMBER(4),
	FOREIGN KEY (titulo_obra_p, agno_produc_p) REFERENCES Obra(titulo_obra, agno_produc),
	PRIMARY KEY (titulo_obra_p, agno_produc_p)
);

CREATE TABLE pelicula_relac (
	titulo_obra_pr        	VARCHAR(165),
	agno_produc_pr		  	NUMBER(4),
	titulo_obra_rela_pr   	VARCHAR(165),
	agno_produc_rela_pr		NUMBER(4),
	tipo_pr 			  	VARCHAR(15) NOT NULL CHECK (tipo_pr IN ('secuela', 'precuela', 'remake')),
	FOREIGN KEY (titulo_obra_pr, agno_produc_pr) REFERENCES Pelicula(titulo_obra_p, agno_produc_p),
	FOREIGN KEY (titulo_obra_rela_pr, agno_produc_rela_pr) REFERENCES Pelicula(titulo_obra_p, agno_produc_p),
	PRIMARY KEY (titulo_obra_pr, agno_produc_pr, titulo_obra_rela_pr, agno_produc_rela_pr)
);

CREATE TABLE Serie (
	titulo_obra_s 		VARCHAR(165),
	agno_produc_s		NUMBER(4),
	ini_emision		NUMBER(4),
	fin_emision		NUMBER(4),
	FOREIGN KEY (titulo_obra_s, agno_produc_s) REFERENCES Obra(titulo_obra, agno_produc),
	PRIMARY KEY (titulo_obra_s, agno_produc_s)
);

CREATE TABLE Capitulo (
	titulo_obra_c 		VARCHAR(165),
	agno_produc_c		NUMBER(4),
	titulo_cap		VARCHAR(75),
	num_cap			NUMBER(2),
	num_temp		NUMBER(2),
	FOREIGN KEY (titulo_obra_c, agno_produc_c) REFERENCES Serie(titulo_obra_s, agno_produc_s),
	PRIMARY KEY (titulo_obra_c, agno_produc_c, titulo_cap)
);

CREATE TABLE Genero (
	genero_obra		VARCHAR(20) PRIMARY KEY
);

CREATE TABLE pertenece_al_genero (
	titulo_obra_g		VARCHAR(165),
	agno_produc_g		NUMBER(4),
	genero_obra_g		VARCHAR(20),
	FOREIGN KEY (titulo_obra_g, agno_produc_g) REFERENCES Obra(titulo_obra, agno_produc),
	FOREIGN KEY (genero_obra_g) REFERENCES Genero(genero_obra),
	PRIMARY KEY (titulo_obra_g, agno_produc_g, genero_obra_g)
);

CREATE TABLE Personal (
	nom_persona		VARCHAR(50),
	genero_pers		VARCHAR(1) CHECK (genero_pers IN ('m', 'f', 'NULL')),
	lugar_nac		VARCHAR(110),
	nacionalidad		VARCHAR(50),
	PRIMARY KEY (nom_persona)
);

CREATE TABLE trabaja_otros (
	nom_persona_o		VARCHAR(50),
	lugar_nac_o			VARCHAR(110),
	titulo_obra_o		VARCHAR(165),
	agno_produc_o		NUMBER(4),
	papel_obra		VARCHAR(20),
	FOREIGN KEY (nom_persona_o) REFERENCES Personal(nom_persona),
	FOREIGN KEY (titulo_obra_o, agno_produc_o) REFERENCES Obra(titulo_obra, agno_produc),
	PRIMARY KEY (nom_persona_o, titulo_obra_o, agno_produc_o, papel_obra)
);

CREATE TABLE trabaja_actor (
	nom_persona_a		VARCHAR(50),
	titulo_obra_a		VARCHAR(165),
	agno_produc_a 		NUMBER(4),
	FOREIGN KEY (nom_persona_a) REFERENCES Personal(nom_persona),
	FOREIGN KEY (titulo_obra_a, agno_produc_a) REFERENCES Obra(titulo_obra, agno_produc),
	PRIMARY KEY (nom_persona_a, titulo_obra_a, agno_produc_a)
);

CREATE TABLE Personaje (
	nom_persona_j		VARCHAR(50),
	titulo_obra_j		VARCHAR(165),
	agno_produc_j		NUMBER(4),
	nom_personaje		VARCHAR(80),
	orden_import		NUMBER(4),
	FOREIGN KEY (nom_persona_j, titulo_obra_j, agno_produc_j) REFERENCES trabaja_actor(nom_persona_a, titulo_obra_a, agno_produc_a),
	PRIMARY KEY (nom_persona_j, titulo_obra_j, agno_produc_j, nom_personaje)
);


