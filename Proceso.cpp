#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

 /*Estructura que simulará los procesos gestionados por el programa.*/
struct Proceso {
    int pid;
    int pc;
    int ax, bx, cx;
    int quantum;
    string estado = "Listo";
    vector<string> instrucciones;

    /*Constructor del struct*/
    Proceso() = default;

    /*Función que imprime los datos (atributos) de un proceso*/
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

/*Funcion que:
  - Obtiene cada linea del txt (el que contiene los procesos, y luego el txt con las instrucciones)
  - Divide y limpia la cadena
  - Asigna esos valores a los atributos de un Proceso
  - Retorna el proceso. 
*/
Proceso parseLineaTxt(const string& linea) {
    Proceso p;
    p.pc = 0; // forzar PC a 0 siempre

    stringstream ss(linea);
    string campo;

    while (getline(ss, campo, ',')) {
        campo.erase(0, campo.find_first_not_of(" \t"));
        campo.erase(campo.find_last_not_of(" \t") + 1);

        // Aquí detectamos el tipo de separador SOLO para pid
        string clave, valor;
        if (campo.find("PID:") != string::npos) {
            size_t sep_pos = campo.find(':');
            clave = campo.substr(0, sep_pos);
            valor = campo.substr(sep_pos + 1);
        } else {
            size_t sep_pos = campo.find('=');
            if (sep_pos == string::npos) continue;

            clave = campo.substr(0, sep_pos);
            valor = campo.substr(sep_pos + 1);
        }

        // Limpiar espacios
        clave.erase(0, clave.find_first_not_of(" \t"));
        clave.erase(clave.find_last_not_of(" \t") + 1);
        valor.erase(0, valor.find_first_not_of(" \t"));
        valor.erase(valor.find_last_not_of(" \t") + 1);

        // Asignación
        if (clave == "PID") p.pid = stoi(valor);
        else if (clave == "AX") p.ax = stoi(valor);
        else if (clave == "BX") p.bx = stoi(valor);
        else if (clave == "CX") p.cx = stoi(valor);
        else if (clave == "Quantum") p.quantum = stoi(valor);
    }


    // Instrucciones desde archivo "<pid>.txt"
    ifstream instFile(to_string(p.pid) + ".txt");
    string inst;
    while (getline(instFile, inst)) {
        if (!inst.empty()) p.instrucciones.push_back(inst);
    }

    return p;
}
