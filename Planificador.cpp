#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "Proceso.cpp"

using namespace std;

struct Planificador {
    queue<Proceso> procesos;

    // Constructor sin JSON
    Planificador() = default;

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

   void ejecutarInstruccion(Proceso& p, const string& inst) {
        stringstream ss(inst);
        string opcode, op1, op2;
        ss >> opcode >> op1;

        // Eliminar comas y espacios
        if (op1.back() == ',') op1.pop_back();
        ss >> op2;

        int* reg1 = getRegistro(p, op1);
        int* reg2 = getRegistro(p, op2);
        int val2 = (reg2 != nullptr) ? *reg2 : (op2 != "" ? stoi(op2) : 0);

        if (opcode == "ADD" && reg1)
            *reg1 += val2;
        else if (opcode == "SUB" && reg1)
            *reg1 -= val2;
        else if (opcode == "MUL" && reg1)
            *reg1 *= val2;
        else if (opcode == "INC" && reg1)
            ++(*reg1);
        else if (opcode == "NOP") {
            // no hace nada
        }
        else if (opcode == "JMP") {
            int salto = stoi(op1);
            p.pc = salto - 1;  // -1 porque el roundRobin luego hace ++
        }
            else {
                cout << ">> Instrucción desconocida: " << inst << endl;
        }
    }

    int* getRegistro(Proceso& p, const string& nombre) {
        if (nombre == "AX") return &p.ax;
        if (nombre == "BX") return &p.bx;
        if (nombre == "CX") return &p.cx;
        return nullptr;
    }

    void roundRobin() {

        ofstream log("simulacion.log");
        unordered_map<int, int> contadorPorPC;
        const int MAX_REP_POR_PC = 10;
        while (!procesos.empty()) {
            Proceso actual = procesos.front();
            procesos.pop();

            cout << "\n[Cambio de contexto]\n";
            log << "\n[Cambio de contexto]\n";
            cout << "Cargando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;
            log << "Cargando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            int ciclos = 0;
            while (ciclos < actual.quantum && actual.pc < actual.instrucciones.size()) {
                string instruccion = actual.instrucciones[actual.pc];
                cout << ">> Proceso " << actual.pid << " ejecutando: " << instruccion << endl;
                log << ">> Proceso " << actual.pid << " ejecutando: " << instruccion << endl;
                contadorPorPC[actual.pc]++;
                if (contadorPorPC[actual.pc] > MAX_REP_POR_PC) {
                    cout << "⚠️ Se detectó ciclo en PC=" << actual.pc << ". Forzando avance.\n";
                    log << "⚠️ Se detectó ciclo en PC=" << actual.pc << ". Forzando avance.\n";
                    actual.pc++; // saltar la instrucción problemática
                    break;       // salir del quantum o puedes seguir si prefieres
                }

                ejecutarInstruccion(actual, instruccion);


                actual.pc++;
                ciclos++;

                cout << "   Estado tras instrucción: PC=" << actual.pc
                     << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;
                log << "   Estado tras instrucción: PC=" << actual.pc
                     << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;
            }

            cout << "Guardando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;
            log << "Guardando estado de Proceso " << actual.pid << ": PC=" << actual.pc 
                 << ", AX=" << actual.ax << ", BX=" << actual.bx << ", CX=" << actual.cx << endl;

            if (actual.pc < actual.instrucciones.size()) {
                procesos.push(actual);
            } else {
                cout << ">> Proceso " << actual.pid << " ha terminado.\n";
                log << ">> Proceso " << actual.pid << " ha terminado.\n";
            }
        }
    }
};
