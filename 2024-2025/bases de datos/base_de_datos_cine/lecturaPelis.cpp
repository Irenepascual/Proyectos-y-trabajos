#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <set>

using namespace std;

struct Cinema{
    //TABLA CAPITULO
    string num_cap_c;
    string temp_cap_c;
    string titulo_cap_c; //FK
    string titulo_obra_cap_c; //FK
    string agno_produc_obra_cap_c;

    //TABLA SERIE
    string ini_emision_serie_c;
    string fin_emision_serie_c;
    string titulo_obra_serie_c; //FK

    //TABLA PELI
    string titulo_obra_pel_c; //FK
    string tipo_pel_c;

    //TABLA RELA_PELI
    string titulo_obra_pr_c;
    string agno_pr_c;
    string titulo_obra_rela_pr_c;
    string agno_rela_pr_c;
    string tipo_pr_c;

    //TABLA OBRA
    string titulo_obra_c;
    string agno_obra_c;

    //TABLA GÉNERO
    string genero_gen1_gen2_c;

    //TABLA PERTENECE_AL_GÉNERO
    string titulo_obra_gen1_c;
    string genero_gen1_c; //FK

    //TABLA PERSONAL
    string nombre_pers_c;
    string sexo_pers_c;
    string nacionalidad_pers_c;
    string lugarNac_pers_c;
    string fechaNac_per_c;
    string lugarDEP_per_c;
    string fechaDEP_per_c;
    string otros_pers_c;

    //TABLA TRABAJA_OTROS
    string nombre_otros_c; //FK
    string titulo_obra_otros_c; //FK
    string papel_otros_c;
    string agno_produc_t;

    //TABLA TRABAJA_ACTOR
    string nombre_act_c; //FK
    string titulo_obra_act_c; //FK
    string agno_produc_ta;

    //TABLA PERSONAJE
    string nombre_act_personaje_c; //FK
    string titulo_obra_personaje_c; //FK
    string nom_personaje_c;
    string orden_personaje_c;
    string agno_produc_pj;
};

// Función para dividir una cadena en campos utilizando un delimitador
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, '=')) {
        tokens.push_back(token);
    }
    return tokens;
}


void insertIntoObra(const vector<Cinema>& cine){
    ofstream sqlFile("insert_Obra.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }
    
    set<string> insertedLines;
    for (const auto& cine : cine) {
        if (cine.titulo_obra_c!= "NULL" && cine.titulo_obra_c != "") {
            
            string line = "INSERT INTO Obra (titulo_obra, agno_produc) VALUES ("
                    + cine.titulo_obra_c + ", " + cine.agno_obra_c + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}    

void insertIntoPelicula(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_Pelicula.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedPelis;
    for (const auto& cine : cine) {
        if (cine.titulo_obra_pel_c!= "NULL" && cine.titulo_obra_pel_c != "") {
            string line = "INSERT INTO Pelicula (titulo_obra_p, agno_produc_p) VALUES (" 
                    + cine.titulo_obra_pel_c + ", " + cine.agno_obra_c + ");";

            if (insertedPelis.find(line) == insertedPelis.end()) {
                sqlFile << line << endl;
                insertedPelis.insert(line);
            }
        }
    }

    sqlFile.close();
}

void insertIntoPeliculaRelac(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_pelicula_relac.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedPelisRelac;
    for (const auto& cine : cine) {
        if (cine.tipo_pr_c != "NULL" && cine.tipo_pr_c != "" && cine.titulo_obra_pr_c!= "NULL" && cine.titulo_obra_pr_c != "") {

            string line = "INSERT INTO pelicula_relac (titulo_obra_pr, agno_produc_pr, titulo_obra_rela_pr, agno_produc_rela_pr, tipo_pr) VALUES (" 
                    + cine.titulo_obra_pr_c + ", " + cine.agno_pr_c + ", " + cine.titulo_obra_rela_pr_c
                    + "," + cine.agno_rela_pr_c + ", " + cine.tipo_pr_c + ");";
            string clave = cine.titulo_obra_pr_c + ", " + cine.agno_pr_c + ", " + cine.titulo_obra_rela_pr_c + "," + cine.agno_rela_pr_c;
            
            if (insertedPelisRelac.find(clave) == insertedPelisRelac.end()) {
                sqlFile << line << endl;
                insertedPelisRelac.insert(clave);
            }
        }
    }

    sqlFile.close();
}

void insertIntoSerie(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_Serie.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    // OPCION 1: con 4 "Amor a primera vista"
    // set<string> insertedLines;
    // for (const auto& cine : cine) {
    //     if (cine.titulo_obra_serie_c != "NULL" && cine.titulo_obra_serie_c != "") {
    //         //OJOOOOOO FALTA AÑADIR EL TIPO DE PELICULA
    //         string line = "INSERT INTO Serie (titulo_obra_s, ini_emision, fin_emision) VALUES (" 
    //                 + cine.titulo_obra_serie_c + ", " + cine.ini_emision_serie_c + ", " + cine.fin_emision_serie_c + ");";
    //         if (insertedLines.find(line) == insertedLines.end()) {
    //             sqlFile << line << endl;
    //             insertedLines.insert(line);
    //         }
    //     }
    // }

    // OPCION 2: con 1 "Amor a primera vista"
    set<string> insertedSeries;
    for (const auto& cine : cine) {
        if (cine.titulo_obra_serie_c != "NULL" && cine.titulo_obra_serie_c != "") {

            string line = "INSERT INTO Serie (titulo_obra_s, agno_produc_s, ini_emision, fin_emision) VALUES (" 
                    + cine.titulo_obra_serie_c + ", " + cine.agno_obra_c + ", " + cine.ini_emision_serie_c + ", " + cine.fin_emision_serie_c + ");";
            
            string clave = cine.titulo_obra_serie_c + ", " + cine.agno_obra_c;
            if (insertedSeries.find(clave) == insertedSeries.end()) {
                sqlFile << line << endl;
                insertedSeries.insert(clave);
            }
        }
    }

    sqlFile.close();
}

void insertIntoCapitulo(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_Capitulo.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedCap;
    for (const auto& cine : cine) {
        if (cine.titulo_obra_cap_c!= "NULL" && cine.titulo_obra_cap_c != "") {
            
            string line = "INSERT INTO Capitulo (titulo_obra_c, agno_produc_c, titulo_cap, num_cap, num_temp) VALUES ("
                    + cine.titulo_obra_cap_c + ", " + cine.agno_produc_obra_cap_c + ", "
                    + cine.titulo_cap_c + ", " + cine.num_cap_c + ", " + cine.temp_cap_c +  ");";
            string clave = cine.titulo_obra_cap_c + ", " + cine.agno_produc_obra_cap_c + ", " + cine.titulo_cap_c;
            
            if (insertedCap.find(clave) == insertedCap.end()) {
                sqlFile << line << endl;
                insertedCap.insert(clave);
            }
        }
    }

    sqlFile.close();
}

void insertIntoGenero(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_Genero.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& cine : cine) {
        if (cine.genero_gen1_gen2_c!= "NULL") {
            
            string line = "INSERT INTO Genero (genero_obra) VALUES ("
                    + cine.genero_gen1_gen2_c +  ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

void insertIntoPerteneceGenero(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_pertenece_al_genero.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& cine : cine) {
        if (cine.genero_gen1_c != "NULL" && cine.genero_gen1_c != "" && cine.titulo_obra_gen1_c != "" && cine.titulo_obra_gen1_c != "NULL") {
            
            string line = "INSERT INTO pertenece_al_genero (titulo_obra_g, agno_produc_g, genero_obra_g) VALUES ("
                    + cine.titulo_obra_gen1_c + ", " + cine.agno_obra_c + ", " + cine.genero_gen1_c +  ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

void insertIntoPersonal(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_Personal.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    // OPCION 1 : con todos los atributos pero MUY MAL
    // set<string> insertedLines;
    // for (const auto& cine : cine) {
    //     if (cine.nombre_pers_c != "NULL" && cine.nombre_pers_c != "") {
            
    //         string line = "INSERT INTO Personal (nom_persona, genero_pers, nacionalidad, lugar_nacim, fecha_nacim, lugar_muerte, fecha_muerte, otras_notas) VALUES ("
    //                 + cine.nombre_pers_c + ", " + cine.sexo_pers_c + ", " + cine.nacionalidad_pers_c + ", "
    //                 + cine.lugarNac_pers_c + ", " + cine.fechaNac_per_c + ", " + cine.lugarDEP_per_c + ", "
    //                 + cine.fechaDEP_per_c + ", " + cine.otros_pers_c + ");";

    //         if (insertedLines.find(cine.nombre_pers_c) == insertedLines.end()) {
    //             sqlFile << line << endl;
    //             insertedLines.insert(cine.nombre_pers_c);
    //         }
    //     }
    // }

    // OPCION 2 : sólo con los necesarios
    set<string> insertedNames;
    string nacionalidad;
    string lugar_nacimiento;
    for (int i=0; i<cine.size(); i++) {
        if (cine[i].nombre_pers_c != "NULL" && cine[i].nombre_pers_c != "") {
            nacionalidad = "NULL";
            lugar_nacimiento = "NULL";
            while (cine[i].nombre_pers_c == cine[i+1].nombre_pers_c) {
                if (cine[i].nacionalidad_pers_c != "NULL" && cine[i].nacionalidad_pers_c != "") {
                    nacionalidad = cine[i].nacionalidad_pers_c;
                    lugar_nacimiento = cine[i].lugarNac_pers_c;
                }
                i++;
            }
            if (cine[i].nacionalidad_pers_c != "NULL" && cine[i].nacionalidad_pers_c != "") {
                nacionalidad = cine[i].nacionalidad_pers_c;
                lugar_nacimiento = cine[i].lugarNac_pers_c;
            }
            string line = "INSERT INTO Personal (nom_persona, genero_pers, lugar_nac, nacionalidad) VALUES ("
                        + cine[i].nombre_pers_c + ", " + cine[i].sexo_pers_c + ", " + lugar_nacimiento + ", " + nacionalidad + ");";
            
            if (insertedNames.find(cine[i].nombre_pers_c) == insertedNames.end()) {
                sqlFile << line << endl;
                insertedNames.insert(cine[i].nombre_pers_c);
            }
        }
    }

    sqlFile.close();
}

void insertIntoTrabajaOtros(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_trabaja_otros.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& cine : cine) {
        if (cine.nombre_otros_c != "NULL" && cine.nombre_otros_c != "" && cine.papel_otros_c != "actress" && cine.papel_otros_c != "actor" && cine.papel_otros_c != "NULL" && cine.papel_otros_c != "") {
            
            string line = "INSERT INTO trabaja_otros (nom_persona_o, titulo_obra_o, agno_produc_o, papel_obra) VALUES ("
                    + cine.nombre_otros_c + ", " + cine.titulo_obra_otros_c + ", " + cine.agno_produc_t + ", " + cine.papel_otros_c + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

void insertIntoTrabajaActor(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_trabaja_actor.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& cine : cine) {
        if (cine.nombre_act_c != "NULL" && cine.nombre_act_c != "" && cine.titulo_obra_act_c != "NULL" && cine.titulo_obra_act_c != "") {
            
            string line = "INSERT INTO trabaja_actor (nom_persona_a, titulo_obra_a, agno_produc_a) VALUES ("
                    + cine.nombre_act_c + ", " + cine.titulo_obra_act_c + ", " + cine.agno_produc_ta + ");";

            if (insertedLines.find(line) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(line);
            }
        }
    }

    sqlFile.close();
}

void insertIntoPersonaje(const vector<Cinema>& cine) {
    ofstream sqlFile("insert_Personaje.sql");
    if (!sqlFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir SQL." << endl;
        return;
    }

    set<string> insertedLines;
    for (const auto& cine : cine) {
        if (cine.nombre_act_personaje_c != "NULL" && cine.nombre_act_personaje_c != ""
            && cine.titulo_obra_personaje_c != "NULL" && cine.titulo_obra_personaje_c != ""
            && cine.nom_personaje_c != "NULL" && cine.nom_personaje_c != "") {
            
            string line = "INSERT INTO Personaje (nom_persona_j, titulo_obra_j, agno_produc_j, nom_personaje, orden_import) VALUES ("
                    + cine.nombre_act_personaje_c + ", " + cine.titulo_obra_personaje_c + ", " + cine.agno_produc_pj + ", "
                    + cine.nom_personaje_c + ", " + cine.orden_personaje_c + ");";
            string clave = cine.nombre_act_personaje_c + ", " + cine.titulo_obra_personaje_c + ", " + cine.agno_produc_pj + ", " + cine.nom_personaje_c;
            
            if (insertedLines.find(clave) == insertedLines.end()) {
                sqlFile << line << endl;
                insertedLines.insert(clave);
            }
        }
    }
    sqlFile.close();
}



int main() {
    ifstream file("DatosPeliculas.csv");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo CSV." << endl;
        return 1;
    }

    string line;
    vector<Cinema> cine;
    
    // Ignora la primera línea (encabezado)
    getline(file, line);
    int i = 1;
    // Lee cada línea del archivo CSV
    while (getline(file, line)) {
        i++;
        vector<string> tokens = split(line, ';');
        if (tokens.size() != 20) {
            cerr << "Error: " << i << "Formato de línea inválido. Línea: " << line << endl;
            cerr << "SIZE: " << tokens.size() << endl;
            cout << tokens[0] << endl;
            continue;
        }

        Cinema cines;

        if(tokens[7]=="movie"){
            //TITULO OBRA Y PELICULA SI ES PELICULA
            if (!tokens[0].empty()) {
                if (tokens[0][0] == '"') {
                    tokens[0].erase(tokens[0].begin());
                    tokens[0].erase(tokens[0].end()-1);
                }
                if(tokens[0].find('&') != -1) {
                    string p1 = tokens[0];
                    string p2 = tokens[0];
                    p1.erase(p1.begin()+p1.find('&'),p1.end());
                    p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                    string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                    cines.titulo_obra_c = res;
                }else {
                    cines.titulo_obra_c = "'" + (tokens[0]) + "'";
                }
            }else{
                cines.titulo_obra_c="NULL";
            }
            cines.titulo_obra_pel_c = cines.titulo_obra_c;

            //falta hacer el tipo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if (!tokens[10].empty()) {
                string s1 = cines.titulo_obra_pel_c;
                string s2;
                if (tokens[8][0] == '"') {
                    tokens[8].erase(tokens[8].begin());
                    tokens[8].erase(tokens[8].end()-1);
                }
                if(tokens[8].find('&') != -1) {
                    string p1 = tokens[8];
                    string p2 = tokens[8];
                    p1.erase(p1.begin()+p1.find('&'),p1.end());
                    p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                    string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                    s2 = res;
                }else {
                    s2 = "'" + (tokens[8]) + "'";
                }
                
                if (tokens[10] == "version of" || tokens[10] == "remake of" || tokens[10] == "remade as"
                    || tokens[10] == "spoofs" || tokens[10] == "spoofed in") {
                    cines.tipo_pr_c = "'remake'";
                    if (tokens[1] > tokens[9]) {
                        cines.agno_pr_c = tokens[1];
                        cines.agno_rela_pr_c = tokens[9];
                        cines.titulo_obra_pr_c = s1;
                        cines.titulo_obra_rela_pr_c = s2;
                    }else {
                        cines.agno_pr_c = tokens[9];
                        cines.agno_rela_pr_c = tokens[1];
                        cines.titulo_obra_pr_c = s2;
                        cines.titulo_obra_rela_pr_c = s1;
                    }
                }
                else if (tokens[10] == "follows") {
                    if (tokens[1] > tokens[9]) {
                        cines.tipo_pr_c = "'secuela'";
                        cines.agno_pr_c = tokens[1];
                        cines.agno_rela_pr_c = tokens[9];
                        cines.titulo_obra_pr_c = s1;
                        cines.titulo_obra_rela_pr_c = s2;
                    }else if (tokens[1] < tokens[9]) {
                        // AQUÍ EN NUESTROS DATOS NO HAY NINGUNA PRECUELA, LO PODRÍAMOS OMITIR
                        cines.tipo_pr_c = "'precuela'";
                        cines.agno_pr_c = tokens[9];
                        cines.agno_rela_pr_c = tokens[1];
                        cines.titulo_obra_pr_c = s2;
                        cines.titulo_obra_rela_pr_c = s1;
                    }
                }
                else if (tokens[10] == "followed by") {
                    if (tokens[9] > tokens[1]) {
                        cines.tipo_pr_c = "'secuela'";
                        cines.agno_pr_c = tokens[9];
                        cines.agno_rela_pr_c = tokens[1];
                        cines.titulo_obra_pr_c = s2;
                        cines.titulo_obra_rela_pr_c = s1;
                    }else if (tokens[9] < tokens[1]){
                        // AQUÍ EN NUESTROS DATOS NO HAY NINGUNA PRECUELA, LO PODRÍAMOS OMITIR
                        cines.tipo_pr_c = "'precuela'";
                        cines.agno_pr_c = tokens[1];
                        cines.agno_rela_pr_c = tokens[9];
                        cines.titulo_obra_pr_c = s1;
                        cines.titulo_obra_rela_pr_c = s2;
                    }
                }
                else {
                    cines.tipo_pr_c = "NULL";
                }
            }
        }
        else if(tokens[7]=="tv series"){
            //TITULO OBRA Y SERIE SI ES SERIE
            if (!tokens[0].empty()) {
                if (tokens[0][0] == '"') {
                    tokens[0].erase(tokens[0].begin());
                    tokens[0].erase(tokens[0].end()-1);
                }
                if(tokens[0].find('&') != -1) {
                    string p1 = tokens[0];
                    string p2 = tokens[0];
                    p1.erase(p1.begin()+p1.find('&'),p1.end());
                    p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                    string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                    cines.titulo_obra_c = res;
                }else {
                    cines.titulo_obra_c = "'" + (tokens[0]) + "'";
                }
            }else{
                cines.titulo_obra_c="NULL";
            }
            cines.titulo_obra_serie_c = cines.titulo_obra_c;

            //INICIO Y COMIENZO DE LA SERIE
            if(!tokens[6].empty()){
                // string agno_ini;
                // string agno_fin;
                // bool cambio=false;
                // bool inicio=true;
                // bool final=true;

                // for (int i=0; i<tokens[6].size() ; i++){
                //     char c=tokens[6][i];
                //     if(c == '-'){
                //         cambio=true;
                //     }
                //     else if(!cambio){
                //         if(c=='?'){
                //             inicio=false;
                //             cines.ini_emision_serie_c="NULL";
                //         }
                //         else if(inicio){
                //             agno_ini += c;
                //         }
                //     }
                //     else{
                //         if(c=='?'){
                //             final=false;
                //             cines.fin_emision_serie_c="NULL";
                //         }
                //         else if(final){
                //             agno_fin += c;
                //         }
                //     }
                // }
                // if(inicio){
                //     cines.ini_emision_serie_c=agno_ini;
                // }
                // if(final){
                //     cines.fin_emision_serie_c=agno_fin;
                // }

                string s = tokens[6];
                string ini = s;
                ini.erase(ini.begin()+ini.find('-'),ini.end());
                string fin = s;
                fin.erase(fin.begin(),fin.begin()+fin.find('-')+1);
                if (ini[0] == '?') {
                    cines.ini_emision_serie_c="NULL";
                }else {
                    cines.ini_emision_serie_c=ini;
                }
                if (fin[0] == '?') {
                    cines.fin_emision_serie_c="NULL";
                }else {
                    cines.fin_emision_serie_c=fin;
                }
            }else {
                cines.ini_emision_serie_c="NULL";
                cines.fin_emision_serie_c="NULL";
            }
        }
        else if (tokens[7]=="episode"){
            //TITULO OBRA Y EPISODIO SI ES EPISODIO
            if (!tokens[0].empty()) {
                if (tokens[0][0] == '"') {
                    tokens[0].erase(tokens[0].begin());
                    tokens[0].erase(tokens[0].end()-1);
                }
                if(tokens[0].find('&') != -1) {
                    string p1 = tokens[0];
                    string p2 = tokens[0];
                    p1.erase(p1.begin()+p1.find('&'),p1.end());
                    p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                    string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                    cines.titulo_cap_c = res;
                }else {
                    cines.titulo_cap_c = "'" + (tokens[0]) + "'";
                }
            }else{
                cines.titulo_cap_c="NULL";
            }

            // TITULO OBRA
            if(!tokens[2].empty()){
                cines.titulo_obra_cap_c= "'" + tokens[2] + "'";
            }else{
                cines.titulo_obra_cap_c="NULL";
            }

            // AÑO OBRA
            cines.agno_produc_obra_cap_c = tokens[3];

            //NUM TEMPORADA DEL CAP
            if(!tokens[4].empty()){
                cines.temp_cap_c=tokens[4];
            }
            else{
                cines.temp_cap_c="NULL";
            }

            //NUM DEL CAP
            if(!tokens[5].empty()){
                cines.num_cap_c=tokens[5];
            }
            else{
                cines.num_cap_c="NULL";
            } 
        }

        //AÑO DE PRODUCCIÓN OBRA
        if (!tokens[1].empty()) {
            cines.agno_obra_c = (tokens[1]);
        }else{
            cines.agno_obra_c ="NULL";
        }


        //GÉNERO
        if(!tokens[11].empty()){
            cines.genero_gen1_c= "'" + tokens[11] + "'";
            cines.genero_gen1_gen2_c=cines.genero_gen1_c;
            cines.titulo_obra_gen1_c=cines.titulo_obra_c;
        }
        else{
            cines.genero_gen1_gen2_c="NULL";
            cines.genero_gen1_c="NULL";
            cines.titulo_obra_gen1_c="NULL";
        }

        //NOMBRE DE PERSONAL
        if(!tokens[12].empty()){
            if (tokens[12][0] == '"') {
                tokens[12].erase(tokens[12].begin());
                tokens[12].erase(tokens[12].end()-1);
            }
            if ((int)tokens[12][0] >= 97) {
                tokens[12][0] = tokens[12][0] - 32;
            }

            if(tokens[12].find('&') != -1) {
                string s1 = tokens[12];
                string s2 = tokens[12];
                s1.erase(s1.begin()+s1.find('&'),s1.end());
                s2.erase(s2.begin(),s2.begin()+s2.find('&')+1);
                string res = "'" + s1 + "' || '&' || '" + s2 + "'";
                cines.nombre_pers_c= res;
            }else {
                cines.nombre_pers_c= "'" + tokens[12] + "'";
            }
                    


            
        }
        else{
            cines.nombre_pers_c = "NULL";
        }

        //SEXO DE PERSONAL
        if(!tokens[13].empty()){
            cines.sexo_pers_c = "'" + tokens[13] + "'";
        }
        else{
            cines.sexo_pers_c="NULL";
        }

        //NACIONALIDAD, LUGAR Y FECHA NAC/DEP DE PERSONAL
        if(!tokens[14].empty()){
            if (tokens[15]=="birth notes"){
                if(tokens[14][0] == '"') {
                    tokens[14].erase(tokens[14].begin());
                    tokens[14].erase(tokens[14].end()-1);
                }

                if (tokens[14].find('&') != -1) {
                    string s1 = tokens[14];
                    string s2 = tokens[14];
                    s1.erase(s1.begin()+s1.find('&'),s1.end());
                    s2.erase(s2.begin(),s2.begin()+s2.find('&')+1);
                    if(s1.find('\'') != -1) {
                        s1.erase(s1.begin()+s1.find('\''));
                    }
                    string res = "'" + s1 + "' || '&' || '" + s2 + "'";
                    cines.lugarNac_pers_c= res;
                }else {
                    if(tokens[14].find('\'') != -1) {
                        tokens[14].erase(tokens[14].begin()+tokens[14].find('\''));
                    }
                    cines.lugarNac_pers_c= "'" + tokens[14] + "'";
                }

                string s = tokens[14];
                if (s.find('[') == -1 && s.find('(') == -1) {
                    s.erase(s.begin(),s.begin()+s.find_last_of(' ')+1);
                }
                else if (s.find('(') != -1) {
                    s.erase(s.begin(),s.begin()+s.find_first_of('(')+1);
                    s.erase(s.end()-1);
                    s.erase(s.begin(),s.begin()+s.find_first_of(' ')+1);
                }
                else {	//s.find('[') != -1
                    if (s.find_last_of(']') == s.size()-1) {
                        s.erase(s.begin(),s.begin()+s.find_first_of('[')+1);
                                s.erase(s.end()-1);
                        if (s.find(',') == -1) {
                            s.erase(s.begin(),s.begin()+s.find_first_of(' ')+1);
                        }
                        else {
                            s.erase(s.begin(),s.begin()+s.find_last_of(',')+2);
                        }
                    }
                    else {
                        s.erase(s.begin(),s.begin()+s.find_last_of(',')+2);
                    }
                }
                if (s[s.size()-1] == '"') {
                    s.erase(s.end()-1);
                }

                if (s.find('&') != -1) {
                    string s1 = s;
                    string s2 = s;
                    s1.erase(s1.begin()+s1.find('&'),s1.end());
                    s2.erase(s2.begin(),s2.begin()+s2.find('&')+1);
                    string res = "'" + s1 + "' || '&' || '" + s2 + "'";
                    cines.nacionalidad_pers_c = res;
                }else {
                    cines.nacionalidad_pers_c = "'" + s + "'";
                }
            }else {
                cines.nacionalidad_pers_c = "NULL";
            }
            // else if(tokens[15]=="birth date"){
            //     cines.fechaNac_per_c= "'" + tokens[14] + "'";
            // }
            // else if(tokens[15]=="death notes"){
            //     cines.lugarDEP_per_c= "'" + tokens[14] + "'";
            // }
            // else if(tokens[15]=="death date"){
            //     cines.fechaDEP_per_c= "'" + tokens[14] + "'";
            // }
        }    

        //OTRAS NOTAS DE PERSONAL
        if(!tokens[18].empty()){
            cines.otros_pers_c= "'" + tokens[18] + "'";
        }
        else{
            cines.otros_pers_c="NULL";
        }

        //TRABAJA COMO OTROS
        if (!tokens[12].empty() && !tokens[16].empty() && tokens[16] != "actor" && tokens[16] != "actress"){
            if (tokens[7] == "episode") {
                if (tokens[2][0] == '"') {
                    tokens[2].erase(tokens[2].begin());
                    tokens[2].erase(tokens[2].end()-1);
                }
                cines.titulo_obra_otros_c="'" + tokens[2] + "'";
                cines.agno_produc_t = tokens[3];
            }else {
                if (tokens[0][0] == '"') {
                    tokens[0].erase(tokens[0].begin());
                    tokens[0].erase(tokens[0].end()-1);
                }
                if(tokens[0].find('&') != -1) {
                    string p1 = tokens[0];
                    string p2 = tokens[0];
                    p1.erase(p1.begin()+p1.find('&'),p1.end());
                    p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                    string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                    cines.titulo_obra_otros_c = res;
                }else {
                    cines.titulo_obra_otros_c = "'" + (tokens[0]) + "'";
                }
                cines.agno_produc_t = tokens[1];
            }
            cines.nombre_otros_c=cines.nombre_pers_c;
            cines.papel_otros_c= "'" + tokens[16] + "'";
        }else{
            cines.papel_otros_c="NULL";
            cines.titulo_obra_otros_c="NULL";
            cines.nombre_otros_c="NULL";
        }

        //TRABAJA COMO ACTOR
        if(!tokens[12].empty() && !tokens[16].empty() && (tokens[16]=="actor" || tokens[16]=="actress")){
            cines.nombre_act_c=cines.nombre_pers_c;
            if (tokens[7] == "episode") {
                if (tokens[2][0] == '"') {
                    tokens[2].erase(tokens[2].begin());
                    tokens[2].erase(tokens[2].end()-1);
                }
                cines.titulo_obra_act_c="'" + tokens[2] + "'";
                cines.agno_produc_ta = tokens[3];
            }else {
                if (tokens[0][0] == '"') {
                    tokens[0].erase(tokens[0].begin());
                    tokens[0].erase(tokens[0].end()-1);
                }
                if(tokens[0].find('&') != -1) {
                    string p1 = tokens[0];
                    string p2 = tokens[0];
                    p1.erase(p1.begin()+p1.find('&'),p1.end());
                    p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                    string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                    cines.titulo_obra_act_c = res;
                }else {
                    cines.titulo_obra_act_c = "'" + (tokens[0]) + "'";
                }
                cines.agno_produc_ta = tokens[1];
            }

            //PERSONAJE
            if(!tokens[17].empty()){
                if (tokens[17].find('&') != -1) {
                    string s1 = tokens[17];
                    string s2 = tokens[17];
                    s1.erase(s1.begin()+s1.find('&'),s1.end());
                    s2.erase(s2.begin(),s2.begin()+s2.find('&')+1);
                    string res = "'" + s1 + "' || '&' || '" + s2 + "'";
                    cines.nom_personaje_c = res;
                }else {
                    cines.nom_personaje_c = "'" + tokens[17] + "'";
                }
                
                cines.nombre_act_personaje_c=cines.nombre_pers_c;
                if (tokens[7] == "episode") {
                    if (tokens[2][0] == '"') {
                        tokens[2].erase(tokens[2].begin());
                        tokens[2].erase(tokens[2].end()-1);
                    }
                    cines.titulo_obra_personaje_c="'" + tokens[2] + "'";
                    cines.agno_produc_pj = tokens[3];
                }else {
                    if (tokens[0][0] == '"') {
                        tokens[0].erase(tokens[0].begin());
                        tokens[0].erase(tokens[0].end()-1);
                    }
                    if(tokens[0].find('&') != -1) {
                        string p1 = tokens[0];
                        string p2 = tokens[0];
                        p1.erase(p1.begin()+p1.find('&'),p1.end());
                        p2.erase(p2.begin(),p2.begin()+p2.find('&')+1);
                        string res = "'" + p1 + "' || '&' || '" + p2 + "'";
                        cines.titulo_obra_personaje_c = res;
                    }else {
                        cines.titulo_obra_personaje_c = "'" + (tokens[0]) + "'";
                    }
                    cines.agno_produc_pj = tokens[1];
                }
                tokens[19].erase(tokens[19].end()-1);
                if(!tokens[19].empty()) {
                    cines.orden_personaje_c=tokens[19];
                }else {
                    cines.orden_personaje_c="NULL";
                }
            }else{
                cines.nom_personaje_c= "NULL";
                cines.nombre_act_personaje_c="NULL";
                cines.titulo_obra_personaje_c="NULL";
                cines.orden_personaje_c="NULL";
            }
        }

        cine.push_back(cines);
    }

    file.close();

    // Inserta los datos en las tablas
    insertIntoObra(cine);
    insertIntoPelicula(cine);
    insertIntoPeliculaRelac(cine);
    insertIntoSerie(cine);
    insertIntoCapitulo(cine);
    insertIntoGenero(cine);
    insertIntoPerteneceGenero(cine);
    insertIntoPersonal(cine);
    insertIntoTrabajaOtros(cine);
    insertIntoTrabajaActor(cine);
    insertIntoPersonaje(cine);
    cout << "Datos insertados correctamente en los archivos SQL." << endl;

    return 0;
    
}
