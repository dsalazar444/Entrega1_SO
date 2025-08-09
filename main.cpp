#include "Planificador.cpp"

int main() {
    Planificador p;

    /*cout << "\nPon el nombre de tu archivo txt: "; // por si depronto quiere poner el nombre del archivo desde consola
    string filepath;
    cin >> filepath;*/

    //Obtenemos los procesos del txt
    p.agregarTodosDesdeTxt("procesos.txt");

    cout << "\n=== Todos los procesos cargados desde TXT ===\n";
    queue<Proceso> copia = p.procesos;

    //Imprimimos copia de los procesos cargados del txt
    while (!copia.empty()) {
        copia.front().mostrar();
        copia.pop();
    }

    //Llamamos al round Robin
    p.roundRobin();
    return 0;
}
