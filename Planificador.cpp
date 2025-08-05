#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <cstdio>
//#include "imports/json.hpp"
#include "Proceso.cpp"

using namespace std;
//using json = nlohmann::json;

struct Planificador {
    queue<Proceso> procesos; //Contendrá elementos tipo Proceso

    /*
    Planificador(const string& filename) {
        ifstream archivo(filename); //Creamos objeto ifstream para abrir, cerrar y leer archivos. Abre automaticamente el archivo (json) al crear el objeto.
        json prcs; //Creamos un JSON
        archivo >> prcs; //Archivo lo guardamos en JSON de C++ para que este lo use.

        for (const auto& prc : prcs ){ //Recorremos la lista de diccionarios, almacenando cada item en prc
            procesos.push(Proceso(prc)); //Añadimos lo que "retorna" el constructor Proceso, a la queue
        }
    
    }*/


    /*Funcion encargada de:
      - Abrir archivo txt con los procesos
      - Obtener linea del archivo (cada proceso)
      - Pasarsela a parseLIneaTxt para que retorne el objeto, el cual será añadido a la cola de procesos de Planificador*/
    void agregarTodosDesdeTxt(const string& filepath) {
        ifstream archivo(filepath); //Creamos objeto ifstream con la ruta ingresada por el usuario, para abrir, cerrar y leer archivos.
        if (!archivo.is_open()) { 
            cerr << "No se pudo abrir el archivo: " << filepath << endl;
            return; //Cerramos si no se pudo abrir el archivo
        }
    
        string linea;
        while (getline(archivo, linea)) {  //Iteramos sobre las lineas del archivo y cada una la guardamos en la variable linea
            if (linea.empty()) continue; //Si la linea está vacía, saltamos a la siguiente iteración
            Proceso nuevo = parseLineaTxt(linea); 
            procesos.push(nuevo);
            cout << "Se cargó el proceso PID " << nuevo.pid << endl;
        }
    }

    /* ---------------COMENTE PORQUE GENERABA ERROR----------------
    void agregarDesdeTxt(const string& filepath) {
    Proceso nuevo(filepath);
    procesos.push(nuevo);
    nuevo.mostrar();
    }*/

    void recorrerProcesos(queue<Proceso> procesos) {
        //cout << "Tipo " << typeid(procesos.front()).name() << endl;

    }

    //------------- UNICA QUE MODIFICARÉ LÓGICA-------------
    int ejecutarInstruccion(Proceso& p, const string& instruccion) {
        string inst = instruccion;
        // Limpiar espacios iniciales/finales
        inst.erase(0, inst.find_first_not_of(" \t"));
        inst.erase(inst.find_last_not_of(" \t") + 1);

        if (inst == "NOP") return 0; // Si es NOP, no hacer nada

        //Obtenemos operando y argumentos, pues estan separados por un espacio
        //Ejemplo: "ADD AX, 1" se convierte en op="ADD" y args="AX, 1"
        size_t espacio = inst.find(' '); 
        string op = inst.substr(0, espacio);
        string args = inst.substr(espacio + 1);

        // Quitar espacios y separar por coma si hay
        size_t coma = args.find(',');
        string arg1 = args.substr(0, coma); //OBtenemos el primer argumento (antes de la coma)
        string arg2 = (coma != string::npos) ? args.substr(coma + 1) : ""; //SI hay coma, guardaremos en arg2 lo que hay después de la coma, sino, guardamos una cadena vacía

        // Limpiar espacios al inicio y final del string s, usando una funciopn lambda.
        auto trim = [](string& s) {
            s.erase(0, s.find_first_not_of(" \t"));
            s.erase(s.find_last_not_of(" \t") + 1);
        };

        trim(arg1); trim(arg2); //Llamamos a la función trim para eliminar espacios al inicio y final de los argumentos

        //Creamos un puntero al operadr 1, para evitar repetir código
        int* reg1 = nullptr;
        int val2 = 0;

        // Obtener puntero al registro destino
        if (arg1 == "AX") reg1 = &p.ax;
        else if (arg1 == "BX") reg1 = &p.bx;
        else if (arg1 == "CX") reg1 = &p.cx;

        // Si es una operación unaria (como INC)
        if (op == "INC") {
            if (reg1) (*reg1)++; //Si el puntero si apunta a algo (no nullptr), incrementamos el registro por medio de su puntero
            return 0;
        }
        if (op == "JMP") {
            if (stoi(arg1) < p.instrucciones.size()) { //Si el argumento es un número válido dentro del rango de instrucciones
                p.pc = stoi(arg1); //Si es un salto, actualizamos el PC al valor del argumento, pero no actualizamos su quantum
            return 1; //Unica forma de indicar que la instruccion que hemos ejecutado es la de JMP
            } 
            
        }

        // Si es binaria (ADD, SUB) -> arg2 no es vacío
        if (!arg2.empty()) {
            //Asignamos el valor del segundo argumento, dependiendo de si es un registro o un valor inmediato
            if (arg2 == "AX") val2 = p.ax;
            else if (arg2 == "BX") val2 = p.bx;
            else if (arg2 == "CX") val2 = p.cx;
            else val2 = stoi(arg2);  // valor inmediato (nuemrico)

            //definimos la operación a realizar
            if (reg1) {
                if (op == "ADD") *reg1 += val2;
                else if (op == "SUB") *reg1 -= val2;
                else if (op == "MUL") *reg1 -= val2;
            }
            return 0;    
        }
    }


    void roundRobin() {
        while (!procesos.empty()) { //Mientras que hayan procesos
            Proceso actual = procesos.front();
            procesos.pop();

            cout << "\n[Cambio de contexto]" << endl;
            cout << "Cargando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            actual.estado = "Ejecutando"; //Actualizamos el estado del proceso a "Ejecutando"
            int ciclos = 0; //Con este aseguraremos que se respete el quantum de cada proceso
            while (ciclos < actual.quantum && actual.pc < actual.instrucciones.size()) { //Mientras que ciclos sea menor al quantum (el proceso todavia tiene tiempo de CPU) y el PC sea menor al tamaño de las instrucciones (el proceso todavia tiene instrucciones por ejecutar)
                string instruccion = actual.instrucciones[actual.pc]; //Obtenemos la instrucción n del proceso, dada por el PC
                if (instruccion.empty()) continue; //Si la instrucción está vacía, saltamos a la siguiente iteración sin consumir quantum
                cout << ">> Proceso " << actual.pid << " ejecutando: " 
                     << actual.instrucciones[actual.pc] << endl;

                if (ejecutarInstruccion(actual, instruccion)) { //Llamamos a función para que ejecute la instrucción.
                    actual.instrucciones[actual.pc] = ""; //Si la instrucción es un salto, despues de su ejecución la reemplazaremos por vacio, Para este punto actual.pc ya tendrá el valor d ela instruccion a la que saltamos
                }  else actual.pc++;// actualizamos PC (siguiente instrucción) y ciclos (cada instrucción cuenta como un ciclo, restando 1 al quantum)
                //Fuera de else porque en cualquiera de los casos se consume un ciclo
                ciclos++;

                 // Mostrar cambio tras ejecutar instrucción
                cout << "   Estado tras instrucción: PC=" << actual.pc
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            }

            cout << "Guardando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            if (actual.pc < actual.instrucciones.size()) { 
                procesos.push(actual); // volver a encolar si aún tiene instrucciones
                actual.estado = "Listo";
            } else {
                cout << ">> Proceso " << actual.pid << " ha terminado.\n"; //El proceso no tiene más instrucciones, por lo que no se vuelve a encolar.
                actual.estado = "Terminado"; //Actualizamos el estado del proceso a "Terminado"
            }
        }
    }

};

