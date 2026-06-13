#include <iostream>
#include <time.h>
#include "jugadores.hpp"
#include "abb.hpp"
#include "camino.hpp"
#include "pilaAvance.hpp"


using namespace std;

int fibonacci(int x) {
    switch (x) {
        case 1:
            return 2;
        case 2:
            return 3;
        case 3:
            return 5;
        case 5:
            return 8;
        case 8:
            return 1;
        default:
            return 1;
    }
}

void generarCamino() {
    listaD camino;
    iniciarListaD(camino);
    srand(time(NULL));
    int orientacion = rand() % 2;
    int tamaño = rand() % 21 + 30;


    int random;
    raiz historial;

    //asignacion random

    int cantFibo=0;
    int cantInser=0;


    iniciarArbol(historial);
    if (orientacion == 0) {
        for (int x=0; x<tamaño; x++) {

            if (cantFibo !=cantInser) {
                do {
                    random = rand() % 4999 + 1;
                }while (valorRepetido(random));

                insertarFinal(camino,random,x,true);
                cantInser++;
            }else {
                cantInser=0;
                cantFibo=fibonacci(cantFibo);
                insertarFinal(camino,0,x,false);
            }
        }
    }else {
        for (int x=tamaño-1; x>=0; x--) {

            if (cantFibo !=cantInser) {
                do {
                    random = rand() % 4999 + 1;
                }while (valorRepetido(random));

                insertarInicio(camino,random,x,true);
                cantInser++;
            }else {
                cantInser=0;
                cantFibo=fibonacci(cantFibo);
                insertarInicio(camino,0,x,false);
            }

        }

    }
}