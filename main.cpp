//
// Created by Fran on 10/6/2026.
//
#include <iostream>
#include <stdio.h>
#include "logicaJuego.hpp"
#include "pilaAvance.hpp"

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

void procesamientoNumero(tope &av1, Jugador &j1,int posActual, bool &inm1, int &mal1, int valorBaldosa, bool rach) {
    switch (controlNumero(valorBaldosa,posActual)) {
        case 1:

            cout<<"¡¡¡¡¡¡ NUMERO PRIMO (maligno) !!!!!!!"<<endl;
            if (inm1) {
                cout<<j1.nickname<<" tiene inmunidad, se salva"<<endl;
                inm1 = false;
                break;
            }
            mal1++;
            cout<<"PERDIDA DE RECORRIDO"<<endl;
            cout<<j1.nickname<<" VUELVE A EMPEZAR"<<endl;
            cout<<"Contador de numeros maligno: "<<mal1<<endl;
            destruirPila(av1);
            cargarPila(av1, -1);
            rach=false;
            break;
        case 2:
            cout<<"¡¡¡¡¡¡ NUMERO AMIGO !!!!!!!"<<endl;
            cout<<j1.nickname<<" obtiene inmunidad"<<endl;
            inm1 = true;
            break;
        case 3:
            cout<<"¡¡¡¡¡¡ NUMERO CAPICUA !!!!!!!"<<endl;
            cout<<j1.nickname<<" Retrocede 2 baldosas"<<endl;
            for (int x=0;x<2;x++) {
                desapilarPila(av1);
            }
            rach=false;
            break;
        case 4:
            cout<<"¡¡¡¡¡¡ NUMERO PERFECTO !!!!!!!"<<endl;
            cout<<j1.nickname<<" avanza 3 baldosas"<<endl;
            for (int x=0;x<3;x++) {
                cargarPila(av1, av1->valor+1);
            }
        default:
            cout<<"¡¡¡¡¡¡ NUMERO comun !!!!!!!"<<endl;
            cout<<j1.nickname<<" no hace nada.."<<endl;
            break;
    }
}

int controlPuntaje(int x) {
    if (x<1500) {
        return 10;
    }
    if (x<3500) {
        return 20;
    }
    if (x<5000) {
        return 25;
    }
    return 30;

}


void finalPartida(Jugador &ganador, Jugador &perdedor, int gan,int pen,bool &rach) {
    FILE *archivo = fopen("datosJugadores.bin", "rb+");

    if (archivo == NULL) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Actualizar estadísticas
    ganador.puntaje+=gan;
    if (rach) {ganador.racha++;}else{ganador.racha=0;};
    if (ganador.racha==2) {
        ganador.categoria='S';
        ganador.racha=0;
    }

    perdedor.puntaje -= pen;
    perdedor.racha = 0;

    Jugador aux;

    // Guardar ganador
    rewind(archivo);

    while (fread(&aux, sizeof(Jugador), 1, archivo)) {
        if (strcmp(aux.nickname, ganador.nickname) == 0) {
            fseek(archivo, -sizeof(Jugador), SEEK_CUR);
            fwrite(&ganador, sizeof(Jugador), 1, archivo);
            break;
        }
    }

    // Guardar perdedor
    rewind(archivo);

    while (fread(&aux, sizeof(Jugador), 1, archivo)) {
        if (strcmp(aux.nickname, perdedor.nickname) == 0) {
            fseek(archivo, -sizeof(Jugador), SEEK_CUR);
            fwrite(&perdedor, sizeof(Jugador), 1, archivo);
            break;
        }
    }

    fclose(archivo);
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


    int mal1=0,mal2=0;
    bool inm1=false,inm2=false;
    bool rach1=true,rach2=true;
    int puntaje1=0, puntaje2=0;
    int posActual1,posActual2;
    bool end=false;
    while (!end) {

        //ESTO ES SOLO CUANDO INICIA 1
        if (quienInicia==1) {
            int teclado;

            int dado;
            int valorBaldosa;

            posActual1= (av1->valor==-1)? 0 : consultarNodo(camino,av1->valor);


            dado = rand()% 6 + 1;
            cout<<j1.nickname<<" tira los dados .."<<endl;
            cin>>teclado;
            cout<<"Dado maligno: "<<dado<<endl;
            for (int x=0;x<dado;x++) {
                cargarPila(av1, av1->valor+1);
            }
            valorBaldosa=consultarNodo(camino,av2->valor);
            cout<<"CASILLA n°"<<av1->valor<<" = "<<consultarNodo(camino,av1->valor)<<endl;
            puntaje1 += controlPuntaje(valorBaldosa);
            procesamientoNumero(av1,j1,posActual1,inm1,mal1,valorBaldosa,rach1);


            posActual2= (av2->valor==tamanio+1)? 0 : consultarNodo(camino,av2->valor);

            dado = rand()% 6 + 1;
            cout<<j2.nickname<<" tira los dados .."<<endl;
            cin>>teclado;
            cout<<"Dado maligno: "<<dado<<endl;
            for (int x=0;x<dado;x++) {
                cargarPila(av2, av2->valor-1);
            }
            valorBaldosa=consultarNodo(camino,av2->valor);
            cout<<"CASILLA n°"<<av2->valor<<" = "<<valorBaldosa<<endl;
            puntaje2 += controlPuntaje(valorBaldosa);
            procesamientoNumero(av2,j2,posActual2,inm2,mal2,valorBaldosa,rach2);

            if (av1->valor>tamanio || mal2==3) {
                if (av1->valor>tamanio) {
                    cout<<j1.nickname<<" CRUZO EL CAMINO"<<endl;
                }else {
                    cout<<j2.nickname<<" CAYO 3 VECES EN UN NUMERO MALIGNO"<<endl;
                }

                cout<<j1.nickname<<" GANAA"<<endl;

                if (rach1 == true) {
                    cout<<"BONUS NO RETROCESOS (x2)"<<endl;
                    puntaje1 =puntaje1*2;
                }
                cout<<j1.nickname<<" PUNTAJE TOTAL "<<puntaje1<<endl;
                cout<<j2.nickname<<" PUNTAJE TOTAL "<<puntaje2<<endl;

                cout<<"\n "<<j2.nickname<<" ES PENALIZADO, PIERDE: "<<puntaje2*0.05<<endl;
                finalPartida(j1,j2,puntaje1,puntaje2*0.05,rach1);
                end=true;
            }

            if (av2->valor<0 || mal1==3) {
                if (av2->valor<0) {
                    cout<<j2.nickname<<" CRUZO EL CAMINO"<<endl;
                }else {
                    cout<<j1.nickname<<" CAYO 3 VECES EN UN NUMERO MALIGNO"<<endl;
                }
                cout<<j2.nickname<<" GANAA"<<endl;

                if (rach2==true) {
                    cout<<"BONUS NO RETROCESOS (x2)"<<endl;
                    puntaje2 =puntaje2*2;
                }
                cout<<j1.nickname<<" PUNTAJE TOTAL "<<puntaje1<<endl;
                cout<<j2.nickname<<" PUNTAJE TOTAL "<<puntaje2<<endl;

                cout<<"\n "<<j1.nickname<<" ES PENALIZADO, PIERDE: "<<puntaje1*0.05<<endl;

                finalPartida(j2,j1,puntaje2,puntaje1*0.05,rach2);
                end=true;
            }

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
