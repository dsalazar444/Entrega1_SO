#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <cstdio>
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

    void agregarTodosDesdeTxt(const string& filepath) {
        ifstream archivo(filepath);
        if (!archivo.is_open()) {
            cerr << "No se pudo abrir el archivo: " << filepath << endl;
            return;
        }
    
        string linea;
        while (getline(archivo, linea)) {
            Proceso nuevo = parseLineaTxt(linea);
            procesos.push(nuevo);
            cout << "Se cargó el proceso PID " << nuevo.pid << endl;
        }
    }

    void agregarDesdeTxt(const string& filepath) {
    Proceso nuevo(filepath);
    procesos.push(nuevo);
    nuevo.mostrar();
    }

    void recorrerProcesos(queue<Proceso> procesos) {
        //cout << "Tipo " << typeid(procesos.front()).name() << endl;

    }

    void roundRobin(){
        int n = this->procesos.size();
        while(n > 0 ){
            this->procesos.push(this->procesos.front()); //Añadimos primer elemento a final de la cola
            Proceso actual = this->procesos.front(); //Obtengo el primer elemento
            printf("Cargando estado de Proceso %d: PC= %d, AX= %d, BX= %d, CX= %d\n", actual.pid, actual.pc, actual.ax, actual.bx, actual.cx);
            //OJO: El número de nuestro proceso será el que se le asigno en el JSON
            funcionTiempo();
            printf("Guardando estado de Proceso %d: PC= %d, AX= %d, BX= %d, CX= %d\n", actual.pid, actual.pc, actual.ax, actual.bx, actual.cx);
            //ESE PRINT puede ir en Finstruccion (donde se actualizan los datos
            //del proceso)
            this->procesos.pop(); //Eliminamos el primer elemento, avanzando en la queue
            n--; //Para que solo lo recorra un ciclo (porque no eliminaremos elementos)
        }
        

    }

    void funcionTiempo(){
        // TO-DO: Implementar lógica de tiempo-> counter que llama tarea durante
        // n segundos. OJO, Ftiempo -> Finstruccion -> debe actualizar los ficheros
    }

};

