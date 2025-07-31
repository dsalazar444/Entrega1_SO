#include <string>
#include <vector>
#include "imports/json.hpp"

using namespace std;
using json = nlohmann::json;

struct Proceso {
    int pid;
    int pc;
    int ax, bx, cx;
    int quantum;
    string estado = "Listo"; //Estado para proceso recien creado será Listo.
    vector<string> instrucciones; //Almacenará instrucciones del Proceso

    //FUNCIONES
    Proceso(const json& prc){
        //Usamos la libreria json para manejar el archivo .JSON en el que
        //definiremos todos los procesos
        pid=prc.at("pid");
        pc=prc.at("pc");
        ax=prc.at("ax");
        bx=prc.at("bx");
        cx=prc.at("cx");
        quantum=prc.at("quantum");
        instrucciones=prc.at("instrucciones").get<vector<string>>(); //Con get<> lo que hacemos es obtener y convertir el dato obtenido en el JSON a un vector de C++

    }

    





};