#include "Planificador.cpp"
//No hay que include Proceso porque eso lo hicimos en Planificador, entonces
//aquí no es necesario.

int main() {
    Planificador p("./documento.JSON");
    //p.recorrerProcesos(p.procesos); //-> Era para verificar que se estuvieran
    //agregando a la queu como dato Proceso
    p.roundRobin();
    return 0;
}