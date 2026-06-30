/*
 * main.cpp
 * Modulo Principal - Camino Minado
 * Descripcion: Punto de entrada del programa. Contiene el menu principal,
 *              el menu de juego.
 * Integrantes:
 *              - Castro Juan jose
 *              - Medrano Francisco Caleb
 */

#include <iostream>
#include <stdio.h>
#include <conio.h>       // _getch: espera una tecla en Windows
#include "logicaJuego.hpp"


using namespace std;

/* Declaraciones anticipadas */

void menuJugar() {
    if (cantidadDeJugadores() < 2) {
        cout << "-- Se necesitan al menos 2 jugadores --" << endl;
        return;
    }

    int op, tamanio;
    bool jugadoresSeleccionados = false, caminoGenerado = false;
    Jugador j1, j2;
    listaD camino;
    tope av1, av2;

    cout << "+-+ INICIANDO JUEGO +-+" << endl;
    iniciarListaD(camino);
    inicializarPila(av1);
    inicializarPila(av2);

    do {
        cout << "====== MENU JUGAR =======" << endl;
        cout << "1. Seleccionar Jugadores" << endl;
        cout << "2. Generar camino" << endl;
        cout << "3. Iniciar Juego" << endl;
        cout << "4. Regresar" << endl;
        cin >> op;

        switch (op) {
            case 1:
                seleccionarJugadores(j1, j2);
                jugadoresSeleccionados = true;
                break;

            case 2: {
                destruirCamino(camino);
                generarCamino(camino);
                string clave;
                cout << "Ingrese clave admin para ver el camino: ";
                cin >> clave;
                if (clave == "admin") {
                    mostrarCaminoVisual(camino, camino.tamanio);
                    caminoGenerado = true;
                } else {
                    cout << "CLAVE INCORRECTA" << endl;
                }
                break;
            }

            case 3:
                if (!jugadoresSeleccionados) {
                    cout << "-- Primero seleccione los jugadores --" << endl;
                    break;
                }
                if (!caminoGenerado) {
                    cout << "-- Primero genere el camino --" << endl;
                    break;
                }
                juego(j1, j2, av1, av2, camino);
                caminoGenerado = false; // Se requiere generar un nuevo camino para la proxima partida
                break;

            case 4:
                cout << "Regresando..." << endl;
                break;

            default:
                cout << "OPCION INCORRECTA" << endl;
                break;
        }
    } while (op != 4);
}

/*
 * Pre:  -
 * Post: Punto de entrada del programa.
 *       Si el archivo "datosJugadores.dat" no existe, lo crea con jugadores por defecto.
 *       Muestra el menu principal con opciones para registrar jugadores,
 *       mostrar jugadores, acceder al juego o salir del programa.
 */
int main() {
    FILE *f = fopen("datosJugadores.dat", "rb");
    if (f == NULL) {
        inicializarDefecto();
    } else {
        fclose(f);
    }

    int op;
    do {
        cout << "====== MENU ADMINISTRADOR =======" << endl;
        cout << "1. Registrar Jugadores" << endl;
        cout << "2. Mostrar Jugadores" << endl;
        cout << "3. Jugar" << endl;
        cout << "4. Salir" << endl;
        cin >> op;

        switch (op) {
            case 1: cargaJugador();    break;
            case 2: mostrarJugadores(); break;
            case 3: menuJugar();        break;
            case 4: cout << "FIN DE PROGRAMA" << endl; break;
            default: cout << "OPCION INCORRECTA" << endl; break;
        }
    } while (op != 4);

    return 0;
}
