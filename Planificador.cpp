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

    void ejecutarInstruccion(Proceso& p, const string& instruccion) {
        string inst = instruccion;
        // Limpiar espacios iniciales/finales
        inst.erase(0, inst.find_first_not_of(" \t"));
        inst.erase(inst.find_last_not_of(" \t") + 1);

        if (inst == "NOP") return;

        size_t espacio = inst.find(' ');
        string op = inst.substr(0, espacio);
        string args = inst.substr(espacio + 1);

        // Quitar espacios y separar por coma si hay
        size_t coma = args.find(',');
        string arg1 = args.substr(0, coma);
        string arg2 = (coma != string::npos) ? args.substr(coma + 1) : "";

        // Limpiar espacios
        auto trim = [](string& s) {
            s.erase(0, s.find_first_not_of(" \t"));
            s.erase(s.find_last_not_of(" \t") + 1);
        };

        trim(arg1); trim(arg2);

        int* reg1 = nullptr;
        int val2 = 0;

        // Obtener puntero al registro destino
        if (arg1 == "AX") reg1 = &p.ax;
        else if (arg1 == "BX") reg1 = &p.bx;
        else if (arg1 == "CX") reg1 = &p.cx;

        // Si es una operación unaria (como INC)
        if (op == "INC") {
            if (reg1) (*reg1)++;
            return;
        }

        // Si es binaria (ADD, SUB)
        if (!arg2.empty()) {
            if (arg2 == "AX") val2 = p.ax;
            else if (arg2 == "BX") val2 = p.bx;
            else if (arg2 == "CX") val2 = p.cx;
            else val2 = stoi(arg2);  // valor inmediato

            if (reg1) {
                if (op == "ADD") *reg1 += val2;
                else if (op == "SUB") *reg1 -= val2;
            }
        }
    }


    void roundRobin() {
        while (!procesos.empty()) {
            Proceso actual = procesos.front();
            procesos.pop();

            cout << "\n[Cambio de contexto]" << endl;
            cout << "Cargando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            int ciclos = 0;
            while (ciclos < actual.quantum && actual.pc < actual.instrucciones.size()) {
                string instruccion = actual.instrucciones[actual.pc];
                cout << ">> Proceso " << actual.pid << " ejecutando: " 
                     << actual.instrucciones[actual.pc] << endl;

                 ejecutarInstruccion(actual, instruccion);  // actualiza registros
                actual.pc++;
                ciclos++;

                 // Mostrar cambio tras ejecutar instrucción
                cout << "   Estado tras instrucción: PC=" << actual.pc
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            }

            cout << "Guardando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            if (actual.pc < actual.instrucciones.size()) {
                procesos.push(actual); // volver a encolar si aún tiene instrucciones
            } else {
                cout << ">> Proceso " << actual.pid << " ha terminado.\n";
            }
        }
    }

    void funcionTiempo(){
        // TO-DO: Implementar lógica de tiempo-> counter que llama tarea durante
        // n segundos. OJO, Ftiempo -> Finstruccion -> debe actualizar los ficheros
    }

};

