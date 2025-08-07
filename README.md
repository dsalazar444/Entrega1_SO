# Simulador de Planificación Round Robin en C++

Este proyecto es un **simulador de planificación de procesos Round Robin**, desarrollado en **C++** para sistemas tipo Unix (Ubuntu).  
Cada proceso tiene registros (`AX`, `BX`, `CX`), instrucciones en archivos separados, y es ejecutado según su `quantum`.  
El programa también detecta bucles infinitos (`JMP`) y genera un archivo de log con la traza completa de la ejecución.

---
## 👨‍💻 Autores

DANIELA SALAZAR AMAYA

GINNA ALEJANDRA VALENCIA MACUACE

EMMANUEL ALVAREZ CASTRILLON

## 📦 Proyecto
├── main.cpp # Punto de entrada del programa

├── Planificador.cpp # Lógica del planificador y del Round Robin

├── Proceso.cpp # Estructura del proceso y función para parsear el .txt

├── procesos.txt # Archivo con la definición de procesos

├── 1.txt, 2.txt, 3.txt... # Instrucciones por proceso (nombre = PID)

├── simulacion.log # Archivo generado con el seguimiento completo

└── README.md # Este archivo

---

## 🧪 ¿Qué hace el simulador?

- Carga procesos desde un archivo como `procesos.txt`.
- Asocia a cada proceso un archivo de instrucciones (ej. `4.txt` si el PID es 4).
- Ejecuta instrucciones como `ADD`, `SUB`, `INC`, `MUL`, `JMP`, `NOP`.
- Administra los procesos usando el algoritmo **Round Robin** según el `quantum` de cada proceso.
- Detecta bucles infinitos causados por instrucciones `JMP` repetitivas o encadenadas.
- Guarda todo el seguimiento en un archivo `simulacion.log`.

---

## ⚙️ Requisitos

- Ubuntu Linux
- VS Code instalado (opcional pero recomendado)
- Compilador g++ (`sudo apt install g++`)
- Extensión de C++ instalada en VS Code

---

## ▶️ Cómo compilar y ejecutar (VS Code + Ubuntu)

### 1. Abre la carpeta del proyecto en VS Code

```bash
code .
```
### 2. Compila el programa
```
g++ main.cpp
```
Asegúrate de tener main.cpp, Planificador.cpp, Proceso.cpp, procesos.txt y los txt con las instrucciones en el mismo directorio.

### 3. Ver el output del programa
```
 ./a.out
```
Este comando es necesario, inclusive si solo quieres ver el ouput a traves del archivo.log

## 🧾 Formato del archivo Prueba.txt
Cada línea representa un proceso con los siguientes campos:

pid: 4, ax=2, bx=1, cx=5, quantum=3

pid=5, ax=0, bx=4, cx=3, quantum=2

- Puedes usar pid=4 o pid: 4 → el sistema interpreta ambos formatos correctamente.
- Los campos ax, bx, cx y quantum deben ir con =.
- El campo pc se ignora, ya que todos los procesos inician con PC=0.

## 📝 Instrucciones soportadas
Cada archivo de instrucciones (ej. 4.txt) contiene una lista de instrucciones, una por línea. Las siguientes instrucciones están soportadas:
  

| Instrucción                | Descripción                                         |
| -------------------------- | --------------------------------------------------- |
| `ADD R1, R2` o `ADD R1, N` | Suma el valor de R2 o un número a R1                |
| `SUB R1, R2` o `SUB R1, N` | Resta a R1 el valor de R2 o un número               |
| `MUL R1, R2` o `MUL R1, N` | Multiplica R1 por R2 o un número                    |
| `INC R1`                   | Incrementa R1 en 1                                  |
| `JMP N`                    | Salta a la instrucción número N (empezando desde 0) |
| `NOP`                      | No hace nada, solo consume quantum                  |

## ↪️ Control de Bucles
El sistema incluye mecanismos inteligentes para evitar cuelgues por ciclos infinitos:

- Si se ejecuta la misma instrucción (por PC) más de 10 veces, se detecta como bucle.

- Si se detecta un JMP repetido más de 10 veces, se omite y se continúa con la siguiente instrucción.

- Si hay múltiples JMP encadenados entre varias instrucciones, el sistema detecta que se está repitiendo un conjunto cíclico y lo rompe.

Esto garantiza que ningún proceso se quede atascado indefinidamente.

## 🪂 Salida esperada (ejemplo):

[Cambio de contexto]

Cargando estado de Proceso 4: PC=0, AX=0, BX=2, CX=3

Proceso 4 ejecutando: ADD AX, BX

Estado tras instrucción: PC=1, AX=2, BX=2, CX=3

Proceso 4 ejecutando: SUB CX, 1

Estado tras instrucción: PC=2, AX=2, BX=2, CX=2
   
Proceso 4 ejecutando: JMP 1

⚠️ Se detectó bucle en PC=2. Saltando instrucción JMP.

Estado tras instrucción: PC=3, AX=2, BX=2, CX=2
   
Guardando estado de Proceso 4: PC=3, AX=2, BX=2, CX=2

Proceso 4 ha terminado.

