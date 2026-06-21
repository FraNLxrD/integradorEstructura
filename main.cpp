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


    if(j1.categoria=='S'||j2.categoria=='S'){
        Jugador &senior=(j1.categoria=='S')? j1:j2;
        cout<<"\n"<<senior.nickname<<" es Senior. Elige:"<<endl;
        //RECONTRA INNECESARIO
        if (senior.nickname==j2.nickname) {
            aux=j1;
            j1=j2;
            j2=aux;
        }


        cout<<"Punto de partida (0=izquierda, 1=derecha): ";
        cin>>puntoPartida;
        cout<<"Quien inicia (1="<<j1.nickname<<", 2="<<j2.nickname<<"): ";
        cin>>quienInicia;



    }else{
        srand(time(NULL));
        puntoPartida=rand()%2;
        quienInicia=rand()%2+1;
        cout<<"\n Punto de partida asignado: "<<(puntoPartida==0 ? "Izquierda": "Derecha")<<endl;
        cout<<"Inicia: "<<(quienInicia==1 ? j1.nickname:j2.nickname)<<endl;
    }

    if (puntoPartida==0) {
        cargarPila(av1,0);
        cargarPila(av2,tamanio);
    }else {
        cargarPila(av1,tamanio);
        cargarPila(av2,0);
    }
    srand(time(NULL));


    while (av1->valor > tamanio || av1->valor < 0 || av2->valor > tamanio || av2->valor < 0) {



        if (quienInicia==1) {
            int dado= (rand() % 6) + 1;
            int pos;
            cout<<"JUGADOR 1 ("<<j1.nickname<<") TIRA DADO"<<endl;
            cout<<"DADO: "<<dado<<endl;
            for (int x=0;x<dado;x++) {
                pos = av1->valor + 1;
                cargarPila(av1,pos);
            }
            efectoNumero(consultarNodo(camino,pos),j1,av1);

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
