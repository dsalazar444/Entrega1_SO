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

Proceso parseLineaTxt(const string& linea) {
        Proceso p;
        stringstream ss(linea);
        string campo;
        
        while (getline(ss, campo, ',')) {
            campo.erase(0, campo.find_first_not_of(" \t"));
            campo.erase(campo.find_last_not_of(" \t") + 1);
        
            size_t eq_pos = campo.find('=');
            if (eq_pos == string::npos) continue;
        
            string clave = campo.substr(0, eq_pos);
            string valor = campo.substr(eq_pos + 1);
        
            valor.erase(0, valor.find_first_not_of(" \t"));
            valor.erase(valor.find_last_not_of(" \t") + 1);
        
            if (clave == "pid") p.pid = stoi(valor);
            else if (clave == "pc") p.pc = stoi(valor);
            else if (clave == "ax") p.ax = stoi(valor);
            else if (clave == "bx") p.bx = stoi(valor);
            else if (clave == "cx") p.cx = stoi(valor);
            else if (clave == "quantum") p.quantum = stoi(valor);
            else if (clave == "instrucciones") {
                p.instrucciones.clear();
                p.instrucciones.push_back(valor);  // también puedes dividir por espacio o coma si hay varias
            }
        }
    
        return p;
    }