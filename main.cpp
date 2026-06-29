#include <iostream>
#include <stdio.h>
#include <conio.h> // _getch Esperar tecla en Windows
#include "logicaJuego.hpp"
#include "pilaAvance.hpp"

using namespace std;

void intercambio(Jugador &j1, Jugador &j2);
void finalPartida(Jugador &ganador, Jugador &perdedor, int gan, int pen, bool &rach);

// aca se configura de donde salen y kien empiesa
void configurarPartida(Jugador &j1, Jugador &j2, bool &j1Izquierda) {
    if ((j1.categoria == 'S' && j2.categoria == 'S') || (j1.categoria == 'J' && j2.categoria == 'J')) {
        // si son de la misma catgoria se elije al asar
        if (rand() % 2 == 0) intercambio(j1, j2);
        j1Izquierda = (rand() % 2 == 0);
    } else {
        // el sennior elije todo pa el y el otro se aguanta
        Jugador *senior = (j1.categoria == 'S') ? &j1 : &j2;
        Jugador *junior = (j1.categoria == 'S') ? &j2 : &j1;
        int op;
        cout << senior->nickname << " (Senior) elige quien inicia (1=" << senior->nickname << ", 2=" << junior->nickname << "): ";
        cin >> op;
        if (op == 2) intercambio(*senior, *junior);
        // despues de intercmabiar, j1 siempre tira primero
        cout << j1.nickname << " (quien inicia), elige de donde salir (0=Izquierda, 1=Derecha): ";
        cin >> op;
        j1Izquierda = (op == 0);
    }
}

void procesamientoNumero(tope &av1, Jugador &j1, int posActual, bool &inm1, int &mal1, int valorBaldosa, bool &rach, int startPos, int dir, int targetPos) {
    switch (controlNumero(valorBaldosa, posActual)) {
        case 1:
            cout<<"NUMERO PRIMO (maligno)"<<endl;
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
            cargarPila(av1, startPos);
            rach = false;
            break;
        case 2:
            cout<<"NUMERO AMIGO"<<endl;
            cout<<j1.nickname<<" obtiene inmunidad"<<endl;
            inm1 = true;
            break;
        case 3:
            cout<<"NUMERO CAPICUA"<<endl;
            cout<<j1.nickname<<" Retrocede 2 baldosas"<<endl;
            for (int x=0; x<2; x++) {
                if (av1->valor != startPos && av1->abajo != NULL) desapilarPila(av1);
            }
            rach = false;
            break;
        case 4:
            cout<<"NUMERO PERFECTO"<<endl;
            cout<<j1.nickname<<" avanza 3 baldosas"<<endl;
            for (int x=0; x<3; x++) {
                if (av1->valor == targetPos) break;
                cargarPila(av1, av1->valor + dir);
            }
            break;
        default:
            cout<<"NUMERO comun"<<endl;
            cout<<j1.nickname<<" no hace nada.."<<endl;
            break;
    }
}

int controlPuntaje(int x) {
    if (x < 1500) return 10;
    if (x < 3500) return 20;
    if (x < 5000) return 25;
    return 30;
}

void finalPartida(Jugador &ganador, Jugador &perdedor, int gan, int pen, bool &rach) {
    FILE *archivo = fopen("datosJugadores.dat", "rb+");

    if (archivo == NULL) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Actualizar estadisticas
    ganador.puntaje += gan;
    if (rach) { ganador.racha++; } else { ganador.racha = 0; }
    if (ganador.racha == 2) {
        ganador.categoria = 'S';
        ganador.racha = 0;
    }

    perdedor.puntaje -=pen;
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

void intercambio(Jugador &j1, Jugador &j2){
    Jugador aux = j1;
    j1 = j2;
    j2 = aux;
}

void juego(Jugador &j1, Jugador &j2, tope &av1, tope &av2, listaD &camino) {
    srand(time(NULL));
    bool j1Izquierda;
    configurarPartida(j1, j2, j1Izquierda);

    int start1, dir1, target1;
    int start2, dir2, target2;
    // vemos para donde caminan los pibes segun donde arrancan
    if (j1Izquierda) {
        start1 = 0; dir1 = 1; target1 = camino.tamanio - 1;
        start2 = camino.tamanio - 1; dir2 = -1; target2 = 0;
    } else {
        start1 = camino.tamanio - 1; dir1 = -1; target1 = 0;
        start2 = 0; dir2 = 1; target2 = camino.tamanio - 1;
    }

    cargarPila(av1, start1);
    cargarPila(av2, start2);
    int mal1=0, mal2=0;
    bool inm1=false, inm2=false;
    bool rach1=true, rach2=true;
    int puntaje1=0, puntaje2=0;
    int posActual1, posActual2;
    bool end=false;

    while (!end) {
        int dado, valorBaldosa;
        bool extraTurn;

        // ---- TURNO DEL JUGADOR 1 ----
        // saca posision y tira el dado xq es su turno
        posActual1 = (av1->abajo == NULL) ? 0 : consultarNodo(camino, av1->abajo->valor);
        dado = rand() % 6 + 1;
        cout << "\n=====================================\n";
        cout<<j1.nickname<<" tira los dados .."<<endl;
        _getch(); // esperar tecla
        cout<<"Dado: "<<dado<<endl;

        do {
            extraTurn = false;
            for (int x=0; x<dado; x++) {
                if (av1->valor == target1) break;
                cargarPila(av1, av1->valor + dir1);
            }

            valorBaldosa = consultarNodo(camino, av1->valor);
            cout<<"CASILLA nro "<<av1->valor<<" = "<<valorBaldosa<<endl;
            puntaje1 += controlPuntaje(valorBaldosa);

            if (baldosaValida(camino, av1->valor)) {
                if (controlNumero(valorBaldosa, posActual1) == 1 && inm1) {
                    cout<<"NUMERO PRIMO (maligno) con inmunidad! Se salva y tira de nuevo!"<<endl;
                    inm1 = false;
                    dado = rand() % 6 + 1;
                    _getch();
                    cout<<"Nuevo Dado: "<<dado<<endl;
                    extraTurn = true;
                    posActual1 = valorBaldosa;
                } else {
                    procesamientoNumero(av1, j1, posActual1, inm1, mal1, valorBaldosa, rach1, start1, dir1, target1);
                }
            } else {
                cout<<"Baldosa vacia, suma 10 puntos"<<endl;
                puntaje1 += 10;
            }
        } while (extraTurn);

        if ((dir1 == 1 && av1->valor >= target1) || (dir1 == -1 && av1->valor <= target1)) {


            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            cout<<j1.nickname<<" CRUZO EL CAMINO"<<endl;
            cout<<j1.nickname<<" GANAA"<<endl;
            cout<<"----------------------------------------"<<endl;
            if (rach1) { cout<<"BONUS NO RETROCESOS (x2)\n"<<endl; puntaje1 = puntaje1*2; }
            //CONTROL PUNTAJE
            int penalizacion = puntaje2 * 0.05;
            //FIN CONTROL PUNTAJE

            cout<<j1.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje1<<endl;
            cout<<j2.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje2<<endl;

            cout<<j1.nickname<<"SE LE SUMA: "<<puntaje1<<endl;
            cout<<j2.nickname<<" ES PENALIZADO, PIERDE: "<<penalizacion<<endl;
            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            finalPartida(j1, j2, puntaje1, penalizacion, rach1);
            break;
        }
        if (mal1 == 3) {

            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            cout<<j1.nickname<<" CAYO 3 VECES EN UN NUMERO MALIGNO"<<endl;
            cout<<j2.nickname<<" GANAA"<<endl;
            cout<<"----------------------------------------"<<endl;
            if (rach2) { cout<<"BONUS NO RETROCESOS (x2)\n"<<endl; puntaje2 = puntaje2*2; }

            int penalizacion = puntaje1 * 0.05;

            cout<<j1.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje1<<endl;
            cout<<j2.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje2<<endl;

            cout<<j2.nickname<<"SE LE SUMA: "<<puntaje2<<endl;
            cout<<j1.nickname<<" ES PENALIZADO, PIERDE: "<<penalizacion<<endl;
            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            finalPartida(j2, j1, puntaje2, penalizacion, rach2);
            break;
        }


        // ---- TURNO DEL JUGADOR 2 ----
        // le toka al otro q tira los dados y se fija en el arbol de numeros
        posActual2 = (av2->abajo == NULL) ? 0 : consultarNodo(camino, av2->abajo->valor);
        dado = rand() % 6 + 1;
        cout << "\n=====================================\n";
        cout<<j2.nickname<<" tira los dados .."<<endl;
        _getch(); // esperar tecla
        cout<<"Dado: "<<dado<<endl;

        do {
            extraTurn = false;
            for (int x=0; x<dado; x++) {
                if (av2->valor == target2) break;
                cargarPila(av2, av2->valor + dir2);
            }

            valorBaldosa = consultarNodo(camino, av2->valor);
            cout<<"CASILLA nro "<<av2->valor<<" = "<<valorBaldosa<<endl;
            puntaje2 += controlPuntaje(valorBaldosa);

            if (baldosaValida(camino, av2->valor)) {
                if (controlNumero(valorBaldosa, posActual2) == 1 && inm2) {
                    cout<<"NUMERO PRIMO (maligno) con inmunidad! Se salva y tira de nuevo!"<<endl;
                    inm2 = false;
                    dado = rand() % 6 + 1;
                    _getch();
                    cout<<"Nuevo Dado: "<<dado<<endl;
                    extraTurn = true;
                    posActual2 = valorBaldosa;
                } else {
                    procesamientoNumero(av2, j2, posActual2, inm2, mal2, valorBaldosa, rach2, start2, dir2, target2);
                }
            } else {
                cout<<"Baldosa vacia, suma 10 puntos"<<endl;
                puntaje2 += 10;
            }
        } while (extraTurn);

        if ((dir2 == 1 && av2->valor >= target2) || (dir2 == -1 && av2->valor <= target2)) {
            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            cout<<j2.nickname<<" CRUZO EL CAMINO"<<endl;
            cout<<j2.nickname<<" GANAA"<<endl;
            cout<<"----------------------------------------"<<endl;
            if (rach2) { cout<<"BONUS NO RETROCESOS (x2)"<<endl; puntaje2 = puntaje2*2; }

            int penalizacion = puntaje1 * 0.05;

            cout<<j1.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje1<<endl;
            cout<<j2.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje2<<endl;

            cout<<j2.nickname<<"SE LE SUMA: "<<puntaje2<<endl;
            cout<<j1.nickname<<" ES PENALIZADO, PIERDE: "<<penalizacion<<endl;
            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            finalPartida(j2, j1, puntaje2, penalizacion, rach2);
            break;
        }
        if (mal2 == 3) {

            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            cout<<j2.nickname<<" CAYO 3 VECES EN UN NUMERO MALIGNO"<<endl;
            cout<<j1.nickname<<" GANAA"<<endl;
            cout<<"----------------------------------------"<<endl;
            if (rach1) { cout<<"BONUS NO RETROCESOS (x2)"<<endl; puntaje1 = puntaje1*2; }

            int penalizacion = puntaje2 * 0.05;

            cout<<j1.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje1<<endl;
            cout<<j2.nickname<<" PUNTAJE EN PARTIDA: "<<puntaje2<<endl;

            cout<<j1.nickname<<"SE LE SUMA: "<<puntaje1<<endl;
            cout<<j2.nickname<<" ES PENALIZADO, PIERDE: "<<penalizacion<<endl;
            cout<<"\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
            finalPartida(j1, j2, puntaje1, penalizacion, rach1);
            break;
        }
    }
    destruirCamino(camino);
    destruirPila(av1);
    destruirPila(av2);
}

void menuJugar(){
    if (cantidadDeJugadores() < 2) {
        cout<<"-- Se necesitan al menos 2 jugadores --"<<endl;
        return;
    }
    int op, tamanio;
    bool jugadoresSeleccionados = false, caminoGenerado = false;
    Jugador j1, j2;
    listaD camino;
    tope av1, av2;
    cout<<"+-+ INICIANDO JUEGO +-+"<<endl;
    iniciarListaD(camino);
    inicializarPila(av1);
    inicializarPila(av2);
    do {
        cout<<"====== MENU JUGAR ======="<<endl;
        cout<<"1. Seleccionar Jugadores"<<endl;
        cout<<"2. Generar camino"<<endl;
        cout<<"3. Iniciar Juego"<<endl;
        cout<<"4. Regresar"<<endl;
        cin>>op;

        switch (op) {
            case 1:
                seleccionarJugadores(j1, j2);
                jugadoresSeleccionados = true;
                break;
            case 2: {
                destruirCamino(camino);
                generarCamino(camino);
                string clave;
                cout<<"Ingrese clave admin para ver el camino: ";
                cin>>clave;
                if (clave == "admin"){
                    mostrarCaminoVisual(camino, camino.tamanio);
                    caminoGenerado = true;
                }else
                    cout<<"CLAVE INCORRECTA"<<endl;
                break;
            }
            case 3:
                if (!jugadoresSeleccionados) {
                    cout<<"-- Primero seleccione los jugadores --"<<endl;
                    break;
                }
                if (!caminoGenerado) {
                    cout<<"-- Primero genere el camino --"<<endl;
                    break;
                }
                juego(j1, j2, av1, av2, camino);
                caminoGenerado = false; // lo apagamos pa q arme uno nuevo dsp
                break;
            case 4:
                cout<<"Regresando..."<<endl;
                break;
            default:
                cout<<"OPCION INCORRECTA"<<endl;
                break;
        }
    } while (op != 4);
}

int main() {
    FILE *f = fopen("datosJugadores.dat", "rb");
    if (f == NULL) {
        inicializarDefecto();
    } else {
        fclose(f);
    }

    int op;
    do {
        cout<<"====== MENU ADMINISTRADOR ======="<<endl;
        cout<<"1. Registrar Jugadores"<<endl;
        cout<<"2. Mostrar Jugadores"<<endl;
        cout<<"3. Jugar"<<endl;
        cout<<"4. Salir"<<endl;
        cin>>op;
        switch (op) {
            case 1: cargaJugador(); break;
            case 2:mostrarJugadores(); break;
            case 3: menuJugar(); break;
            case 4: cout<<"FIN DE PROGRAMA"<<endl; break;
            default: cout<<"OPCION INCORRECTA"<<endl; break;
        }
    } while (op != 4);

    return 0;
}
