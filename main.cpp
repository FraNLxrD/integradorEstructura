//
// Created by Fran on 10/6/2026.
//
#include <iostream>
#include <stdio.h>
#include "jugadores.hpp"
using namespace std;


void verMenuAdministrador(int op) {
    do {
        cout<<"====== MENU ADMINISTRADOR ======="<<endl;
        cout<<"1. Registrar Jugadores"<<endl;
        cout<<"2. Mostrar Jugadores"<<endl;
        cout<<"4. Jugar"<<endl;
        cout<<"5. Salir"<<endl;
        cin>>op;
    }while(op!=4);
};





int main() {

    Jugador lol;;
    //cargaJugador(lol);

    mostrarJugadores();
}
