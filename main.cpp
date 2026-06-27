//
// Created by Fran on 10/6/2026.
//
#include <iostream>
#include <stdio.h>
#include "logicaJuego.hpp"
using namespace std;


void verMenuAdministrador(int op1) {
    bool ban=false;
    do {
        cout<<"====== MENU ADMINISTRADOR ======="<<endl;
        cout<<"1. Registrar Jugadores"<<endl;
        cout<<"2. Mostrar Jugadores"<<endl;
        cout<<"3. Jugar"<<endl;
        cout<<"4. Salir"<<endl;
        cin>>op1;

        switch (op1) {
            case 1:
                cargaJugador();
                ban=true;
                break;
            case 2:
                mostrarJugadores();
                break;
            case 3:
                if (ban==true && cantidadDeJugadores()>=2) {
                    cout<<"+-+ INICIANDO JUEGO +-+"<<endl;
                }
                break;
            case 4:
                cout<<"Saliendo..."<<endl;
                break;
            default:
                cout<<"-- opcion invalida -- "<<endl;
                break;
        }
    }while(op1!=3 && op1!=4);
};

void verMenuJugar(int op2) {

    cout<<"====== MENU JUGAR ======="<<endl;
    cout<<"1. Seleccionar Jugadores"<<endl;
    cout<<"2. Generar camino"<<endl;
    cout<<"3. Iniciar Juego"<<endl;
    cout<<"4. Regresar"<<endl;
    cin>>op2;

}

void efectoNumero(int numero, Jugador j, tope av) {

}


void juego(Jugador &j1, Jugador &j2,tope &av1,tope &av2,int tamanio, listaD &camino) {
    int puntoPartida,quienInicia;

    Jugador aux; //TOTALMENTE INNECESARIO


    if (j1.categoria == 'S' || j2.categoria == 'S') {

        cout << "Punto de partida (0=Izquierda, 1=Derecha): ";
        cin >> puntoPartida;

        cout << "Quien inicia (1=" << j1.nickname << ", 2=" << j2.nickname << "): ";
        cin >> quienInicia;

        // Si el Senior eligió la derecha, lo convertimos en j2
        if (puntoPartida == 1) {

            if (j1.categoria == 'S') {
                aux = j1;
                j1 = j2;
                j2 = aux;
            }
            // Si el Senior es j2 ya está en la derecha, no hago nada.
        }
        // Si eligió la izquierda
        else {

            if (j2.categoria == 'S') {
                aux = j1;
                j1 = j2;
                j2 = aux;
            }
            // Si el Senior es j1 ya está en la izquierda.
        }

    } else {

        srand(time(NULL));

        // Sorteo quién queda a la izquierda
        if (rand() % 2) {
            aux = j1;
            j1 = j2;
            j2 = aux;
        }
        quienInicia = rand() % 2 + 1;
    }

    cargarPila(av1, -1);          // j1 siempre sale desde la izquierda
    cargarPila(av2, tamanio + 1); // j2 siempre sale desde la derecha

    srand(time(NULL));


    while (av1->valor<tamanio || av2->valor>0) {
        if (quienInicia==1) {
            int teclado;
            int dado = rand()% 6 + 1;
            cout<<j1.nickname<<" tira los dados .."<<endl;
            cin>>teclado;
            cout<<"Dado maligno: "<<dado<<endl;
            for (int x=0;x<dado;x++) {
                cargarPila(av1, av1->valor-1);
            }
            cout<<"CASILLA n°"<<av1->valor<<" = "<<consultarNodo(camino,av1->valor)<<endl;
        }
    }


}


int main() {

    int op1,op2,tamanio;
    Jugador j1,j2;
    tope av1,av2;

    listaD camino;


    inicializarDefecto();

    //inicializaciones
    iniciarListaD(camino);

    do {

        verMenuAdministrador(op1);

        verMenuJugar(op2);
        bool ban1=false;
        bool ban2=false;


        switch (op1) {
            case 1:
                seleccionarJugadores(j1,j2);
                ban1=true;
                break;
            case 2:
                generarCamino(camino,tamanio);
                ban2=true;
                break;
            case 3:
                break;
            case 4:
                cout<<"Regresando..."<<endl;
                break;
        }


    }while(op1!=4);

}
