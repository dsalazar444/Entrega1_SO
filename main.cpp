#include "Planificador.cpp"
//No hay que include Proceso porque eso lo hicimos en Planificador, entonces
//aquí no es necesario.

int main() {
    Planificador p("./documento.JSON");

    cout << "\nPon el nombre de tu archivo txt: ";
    string filepath;
    cin >> filepath;

    p.agregarTodosDesdeTxt(filepath);

    cout << "\n=== Todos los procesos cargados desde TXT ===\n";
    queue<Proceso> copia = p.procesos;  // para no vaciar la cola original

    while (!copia.empty()) {
        copia.front().mostrar();
        copia.pop();
    }

    p.roundRobin();
    return 0;
}