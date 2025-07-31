#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include "imports/json.hpp"
#include "Proceso.cpp"

using namespace std;
using json = nlohmann::json;

struct Planificador {
    queue<Proceso> procesos; //Contendrá elementos tipo Proceso

    Planificador(const string& filename) {
        ifstream archivo(filename); //Creamos objeto ifstream para abrir, cerrar y leer archivos. Abre automaticamente el archivo (json) al crear el objeto.
        json prcs; //Creamos un JSON
        archivo >> prcs; //Archivo lo guardamos en JSON de C++ para que este lo use.

        for (const auto& prc : prcs ){ //Recorremos la lista de diccionarios, almacenando cada item en prc
            procesos.push(Proceso(prc)); //Añadimos lo que "retorna" el constructor Proceso, a la queue
        }
    
    }

    void recorrerProcesos(queue<Proceso> procesos) {
        cout << "Tipo " << typeid(procesos.front()).name() << endl;
    }

};

