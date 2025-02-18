#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <set>

using namespace std;

struct Partido {
    string inicio_temp_l;
    string fin_temp_l;
    string denom_ofc_l;
    string num_jornada_l;
    string equipo_loc_l;
    string nom_corto_l;
    string equipo_visit_l;
    string gol_loc_l;
    string gol_visit_l;
    string nom_ofc_l;
    string ciudad_l;
    string fecha_fund_l;
    string fecha_fund_leg_l;
    string nom_hist_l;
    string nom_estad_l;
    string fecha_inaug_l;
    string capacidad_l;
    string nom_otro_l;
};

// Función para dividir una cadena en campos utilizando un delimitador
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ';')) {
        tokens.push_back(token);
    }
    // Si hay un campo adicional al final, leerlo con '\n' como delimitador
    // if (getline(ss, token/*, '\n'*/)) {
    //     tokens.push_back(token);
    // }

    return tokens;
}

// Función para insertar datos en la tabla de Partidos en SQL*Plus
void insertIntoPartidos(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_partidos.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    int puntosLoc, puntosVisit;
    set<string> insertedLines;
    for (const auto& partido : partidos) {
        if (partido.equipo_loc_l != "NULL") {
            if (partido.gol_loc_l < partido.gol_visit_l) {
                puntosLoc = 0;
                puntosVisit = 3;
            }
            else if (partido.gol_loc_l > partido.gol_visit_l) {
                puntosLoc = 3;
                puntosVisit = 0;
            }
            else if (partido.gol_loc_l == partido.gol_visit_l) {
                puntosLoc = 1;
                puntosVisit = 1;
            }
            string line = "INSERT INTO Partidos (equipo_loc, equipo_visit, num_jornada_p, inicio_temp_p, denom_ofc_p, gol_loc, gol_visit, puntos_loc, puntos_visit) VALUES ("
                    + partido.equipo_loc_l + ", " + partido.equipo_visit_l + ", "
                    + partido.num_jornada_l + ", " + partido.inicio_temp_l + ", " + partido.denom_ofc_l + ", "
                    + partido.gol_loc_l + ", " + partido.gol_visit_l + ", " + to_string(puntosLoc) + ", " + to_string(puntosVisit) + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

// Función para insertar datos en la tabla de Temporada en SQL*Plus
void insertIntoTemporada(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_temporada.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& partido : partidos) {
        if (partido.inicio_temp_l != "NULL") {
            string line = "INSERT INTO Temporada (inicio_temp, fin_temp) VALUES ("
                    + partido.inicio_temp_l + ", " + partido.fin_temp_l + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            } 
        }
    }

    sqlFile.close();
}

// Función para insertar datos en la tabla de Division en SQL*Plus
void insertIntoDivision(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_division.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& partido : partidos) {
        if (partido.denom_ofc_l != "NULL") {
            string line = "INSERT INTO Division (denom_ofc) VALUES (" + partido.denom_ofc_l + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

// Función para insertar datos en la tabla de Estadio en SQL*Plus
void insertIntoEstadio(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_estadio.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& partido : partidos) {
        if (partido.nom_estad_l != "NULL") {
            string line = "INSERT INTO Estadio (nom_estad, fecha_inaug, capacidad) VALUES (" + partido.nom_estad_l + ", "
                + partido.fecha_inaug_l + ", " + partido.capacidad_l + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
        
    }

    sqlFile.close();
}

// Función para insertar datos en la tabla de Equipo en SQL*Plus
void insertIntoEquipo(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_equipo.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;

    for (const auto& partido : partidos) {
        string line = "INSERT INTO Equipo (nom_corto, nom_ofc, nom_hist, ciudad, fecha_fund, fecha_fund_leg, nom_estad_e) VALUES ("
                    + partido.nom_corto_l + ", " + partido.nom_ofc_l + ", " + partido.nom_hist_l + ", "
                    + partido.ciudad_l + ", " + partido.fecha_fund_l + ", " + partido.fecha_fund_leg_l + ", "
                    + partido.nom_estad_l + ");";

        if (insertedLines.find(line) == insertedLines.end()) {
            sqlFile << line << endl;
            insertedLines.insert(line);
        }
    }

    sqlFile.close();
}

// Función para insertar datos en la tabla de Jornada en SQL*Plus
void insertIntoJornada(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_jornada.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& partido : partidos) {
        if (partido.num_jornada_l != "NULL") {
            string line = "INSERT INTO Jornada (num_jornada, inicio_temp_j, denom_ofc_j) VALUES ("
                    + partido.num_jornada_l + ", " + partido.inicio_temp_l + ", " + partido.denom_ofc_l + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

// Función para insertar datos en la tabla de Otro_nombre en SQL*Plus
void insertIntoOtroNombre(const vector<Partido>& partidos) {
    ofstream sqlFile("insert_otro_nombre.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;

    for (const auto& partido : partidos) {
        if (partido.nom_otro_l != "NULL") {
            string line = "INSERT INTO Otro_nombre (nom_otro, nom_corto_o) VALUES (" + partido.nom_otro_l + ", " + partido.nom_corto_l + ");";

            if (insertedLines.find(partido.nom_corto_l) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(partido.nom_corto_l);
            }
        }
    }

    sqlFile.close();
}

int main() {
    ifstream file("Liga.csv");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo CSV." << endl;
        return 1;
    }

    string line;
    vector<Partido> partidos;

    // Ignora la primera línea (encabezado)
    getline(file, line);


    // Lee cada línea del archivo CSV
    while (getline(file, line)) {

        vector<string> tokens = split(line, ';');
        if (tokens.size() != 17) {
            cerr << "Error: Formato de línea inválido. Línea: " << line << endl;
            continue;
        }

        Partido partido;

        if (!tokens[0].empty()) {
            partido.inicio_temp_l = (tokens[0]);
        }else{
            partido.inicio_temp_l = "NULL";
        }

        if (!tokens[1].empty()) {
            partido.fin_temp_l = (tokens[1]);
        }else{
            partido.fin_temp_l ="NULL";
        }
        
        if (!tokens[2].empty()){
            partido.denom_ofc_l = "'" + tokens[2] + "'";
        } else {
            partido.denom_ofc_l = "NULL";
        }

        if (!tokens[3].empty()) {
            partido.num_jornada_l = (tokens[3]);
        }else{
            partido.num_jornada_l ="NULL";
        }

        if (!tokens[4].empty()) {
            partido.equipo_loc_l = "'" + (tokens[4]) + "'";
        }else{
            partido.equipo_loc_l ="NULL";
        }

        if (!tokens[4].empty()){
            partido.nom_corto_l = "'" + tokens[4] + "'";
        } else {
            partido.nom_corto_l = "'" + tokens[8] + "'";
        }

        if (!tokens[5].empty()){
            partido.equipo_visit_l = "'" + tokens[5] + "'";
        } else {
            partido.equipo_visit_l = "NULL";
        }

        if (!tokens[6].empty()) {
            partido.gol_loc_l = (tokens[6]);
        }else{
            partido.gol_loc_l = "NULL";
        }

        if (!tokens[7].empty()) {
            partido.gol_visit_l = (tokens[7]);
        }else{
            partido.gol_visit_l = "NULL";
        }

        if (!tokens[8].empty()) {
            partido.nom_ofc_l = "'" + tokens[8] + "'";
        }else{
            partido.nom_ofc_l = "NULL";
        }

        if (!tokens[9].empty()) {
            partido.ciudad_l = "'" + tokens[9] + "'";
        }else{
            partido.ciudad_l = "NULL";
        }

        if (!tokens[10].empty()) {
            partido.fecha_fund_l = (tokens[10]);
        }else{
            partido.fecha_fund_l = "NULL";
        }

        if (!tokens[11].empty()) {
            partido.fecha_fund_leg_l = (tokens[11]);
        }else{
            partido.fecha_fund_leg_l = "NULL";
        }

        if (!tokens[12].empty()) {
            partido.nom_hist_l = "'" + tokens[12] + "'";
        }else{
            partido.nom_hist_l = "NULL";
        }

        if (!tokens[13].empty()) {
            partido.nom_estad_l = "'" + tokens[13] + "'";
        }else{
            partido.nom_estad_l = "NULL";
        }

        if (!tokens[14].empty()) {
            partido.fecha_inaug_l = (tokens[14]);
        }else{
            partido.fecha_inaug_l = "NULL";
        }

        if (!tokens[15].empty()) {
            partido.capacidad_l = (tokens[15]);
        }else{
            partido.capacidad_l = "NULL";
        }

        if (isalpha(tokens[16][0])) {
            partido.nom_otro_l = "'";
            for (int i=0; i<tokens[16].size()-1; i++) {
                partido.nom_otro_l += tokens[16][i];
            }
            partido.nom_otro_l += "'";
        } else {
           partido.nom_otro_l = "NULL";
        }

        partidos.push_back(partido);
    }

    file.close();

    // Inserta los datos en las tablas
    insertIntoPartidos(partidos);
    insertIntoTemporada(partidos);
    insertIntoDivision(partidos);
    insertIntoEstadio(partidos);
    insertIntoEquipo(partidos);
    insertIntoJornada(partidos);
    insertIntoOtroNombre(partidos);

    cout << "Datos insertados correctamente en los archivos SQL." << endl;

    return 0;
}