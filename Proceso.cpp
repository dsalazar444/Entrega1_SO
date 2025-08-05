#include <string>
#include <vector>
#include <fstream>
#include <iostream>
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

    /*/FUNCIONES
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

    }*/

     // Constructor desde archivo TXT
    Proceso() = default;
    

    void mostrar() {
        cout << "\n===== Proceso PID " << pid << " =====\n";
        cout << "PC: " << pc << "\nAX: " << ax << "\nBX: " << bx << "\nCX: " << cx;
        cout << "\nQuantum: " << quantum << "\nEstado: " << estado << "\nInstrucciones:\n";
        for (const auto& inst : instrucciones) {
            cout << "  - " << inst << '\n';
        }
        cout << "=========================\n";
    }





};
/*Función usada para recorrer el txt y obtener los datos para crear los n objetos Proceso, creandolos.
Esta funcion no es de Proceso proque no necesitamos de un objeto tipo Proceso para usarla.*/
Proceso parseLineaTxt(const string& linea) {
    Proceso p;
    stringstream ss(linea);
    string campo;
    int numeroBloque=0; //Para determinar que simbolo buscar (si : o =)
    int eq_pos;

    while (getline(ss, campo, ',')) { //Obtenemos cada campo separado por coma (guardamos en var campo) e iteramos sobre ellos
        campo.erase(0, campo.find_first_not_of(" \t"));
        campo.erase(campo.find_last_not_of(" \t") + 1);
        //eliminamos desde inicio (0) hasta la posición del primer caracter que no sea espacio o tab (eliminamos los espacios en blanco) 

        if (numeroBloque == 0) size_t eq_pos = campo.find(':'); //Si es el primer bloque (PID), se usa :, por lo que buscamos la posición de :
        else size_t eq_pos = campo.find('='); // Si no es el primer bloque, se usa =, por lo que buscamos la posición de =

        numeroBloque++; //Aumentamos el número de bloque para saber si es el primer bloque o no (para saber si usamos : o =)
       //En dato tipo size_t (usado para guardar tamaños), almacenamos la posicion del signo :
        if (eq_pos == string::npos) continue; //Si no hay signo = (find retorna npos), saltamos a la siguiente iteración
    
        //Dividimos la cadena en dos, clave para antes del = y valor para después del =
        //Ejemplo: "pid=1" se convierte en clave="pid" y valor="1"
        string clave = campo.substr(0, eq_pos);
        string valor = campo.substr(eq_pos + 1);
    
        //Limpiamos espacios en blanco al inicio y al final de valor
        valor.erase(0, valor.find_first_not_of(" \t"));
        valor.erase(valor.find_last_not_of(" \t") + 1);
    
        if (clave == "pid") p.pid = stoi(valor); // Convertimos el valor a entero para almacenarlo en los atributos del proceso
        else if (clave == "pc") p.pc = stoi(valor);
        else if (clave == "ax") p.ax = stoi(valor);
        else if (clave == "bx") p.bx = stoi(valor);
        else if (clave == "cx") p.cx = stoi(valor);
        else if (clave == "quantum") p.quantum = stoi(valor);
    }

    // Leer instrucciones desde archivo "<pid>.txt" (se leen desde acá porque a pesar de que provengan de diferentes rutas, siguen siendo datos del mismo proceso)
    ifstream instFile(to_string(p.pid) + ".txt"); //Creamos un ifstream llamado instFile, que abrirá el archivo con el nombre del PID del proceso y extensión .txt (1.txt)
    if (!instFile.is_open()) {
        cout << "Error: No se pudo abrir el archivo de instrucciones para PID " << p.pid << endl;
        return p; // Si no se pudo abrir el archivo, retornamos el proceso sin instrucciones
    }
    string inst;
    while (getline(instFile, inst)) { //Cada linea del archivo la guardamos en la variable inst e iteramos sobre ellas.
        if (!inst.empty()) p.instrucciones.push_back(inst); //Si no está vacia, la añadimos a las instrucciones del proceso.
    }

    return p;
}