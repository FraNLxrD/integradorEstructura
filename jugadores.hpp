
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
using namespace std;

struct Jugador {
    char nombre[30];
    char apellido[30];
    char nickname[30];
    int puntaje;
    char categoria;
};


void agregarJugador(Jugador j) {
    FILE* archivoLect = fopen("datosJugadores.bin","rb");
    if(archivoLect!=NULL) {
        Jugador t;

        while (fread(&t,sizeof(Jugador),1,archivoLect)==1) {
            if (strcmp(t.nickname, j.nickname) == 0) {
                cout<<"--ERRROR: JUGADOR YA EXISTENTE"<<endl;
                fclose(archivoLect);
                return;
            }
        }
    }

    FILE* archivo= fopen("datosJugadores.bin", "ab");
    fwrite(&j, sizeof(Jugador), 1, archivo);
    cout<<"$ JUGADOR - "<<j.nickname<<" - CARGADO CON EXITO $"<<endl;
    fclose(archivo);
}


void cargaJugador() {
    Jugador J;
    cout<<"======= CARGA DE JUGADOR ========"<<endl;
    do {
        cout<<"Nombre: "; cin>>J.nombre;
        cout<<endl;
    }while(J.nombre[0]=='\0');
    do {
        cout<<"Apellido: "; cin>>J.apellido;
        cout<<endl;
    }while(J.apellido[0]=='\0');
    do {
        cout<<"Nickname: "; cin>>J.nickname;
        cout<<endl;
    }while(J.nickname[0]=='\0');

    J.categoria='J';
    J.puntaje=0;

    agregarJugador(J);
}

void inicializarDefecto() {



    FILE* archivo = fopen("datosJugadores.bin","wb");

    if (archivo!=NULL) {
        fclose(archivo);
        return;
    }

    //CARGA JUGADOR 1 (POR DEFECTO)
    Jugador j1={"CARLOS","GARCIA","charlySNM",0,'S'};


    //CARGA JUGADOR 2 (POR DEFECTO)
    Jugador j2={"CHIZZO","NAPOLI","fabianStone67",0,'S'};

    //CARGA JUGADOR 3 (POR DEFECTO)
    Jugador j3={"OSCAR","MORO","elBata",0,'J'};

    //CARGA JUGADOR 4 (POR DEFECTO)
    Jugador j4={"FABIANA","CANTILO","losTwist",0,'J'};


    fwrite(&j1,sizeof(Jugador),1,archivo);
    fwrite(&j2,sizeof(Jugador),1,archivo);
    fwrite(&j3,sizeof(Jugador),1,archivo);
    fwrite(&j4,sizeof(Jugador),1,archivo);

    fclose(archivo);

    cout<<"-- JUGADORES CREADOS POR DEFECTO --"<<endl;

}


void mostrarJugadores() {
    FILE* archivo = fopen("datosJugadores.bin","rb");

    if (archivo!=NULL) {

        fseek(archivo,0,SEEK_END);
        if (ftell(archivo)==0) {
            cout<<"--- SIN JUGADORES ---"<<endl;
            fclose(archivo);
            return;
        }
        rewind(archivo);
        Jugador t;

        cout<<"==== JUGADORES REGISTRADOS ====\n"<<endl;
        while (fread(&t,sizeof(Jugador),1,archivo)==1) {
            cout<<"NOMBRE: "<<t.nombre<<endl;
            cout<<"APELLIDO: "<<t.apellido<<endl;
            cout<<"NICKNAME: "<<t.nickname<<endl;
            cout<<"CATEGORIA: "<<t.categoria<<endl;
            cout<<endl;
        }
        fclose(archivo);
    }else {
        cout<<"-- ERROR: ARCHIVO -- "<<endl;
    }

}

int cantidadDeJugadores() {
    FILE* archivo = fopen("datosJugadores.bin","rb");

    if (archivo!=NULL) {

        fseek(archivo,0,SEEK_END);
        if (ftell(archivo)==0) {
            fclose(archivo);
            return 0;
        }
        rewind(archivo);
        Jugador t;
        int contador=0;

        while (fread(&t,sizeof(Jugador),1,archivo)==1) {
            contador++;
        }
        fclose(archivo);
        return contador;
    }else {
        return -1;
    }
}













