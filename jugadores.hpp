
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
    int racha;
};

void agregarJugador(Jugador j) {
    FILE* archivoLect = fopen("datosJugadores.dat","rb");
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
    FILE* archivo= fopen("datosJugadores.dat", "ab");
    fwrite(&j, sizeof(Jugador), 1, archivo);
    cout<<"$ JUGADOR - "<<j.nickname<<" - CARGADO CON EXITO $"<<endl;
    fclose(archivo);
}

void cargaJugador() {
    Jugador J;
    cout<<"======= CARGA DE JUGADOR ========"<<endl;
    do {
        cout<<"\nNombre: "; cin>>J.nombre;
    }while(J.nombre[0]=='\0');
    do {
        cout<<"\nApellido: "; cin>>J.apellido;
    }while(J.apellido[0]=='\0');
    do {
        cout<<"\nNickname: "; cin>>J.nickname;
    }while(J.nickname[0]=='\0');

    J.categoria='J';
    J.puntaje=0;
    J.racha=0;
    agregarJugador(J);
}

void inicializarDefecto() {
    //CARGA JUGADOR 1 (POR DEFECTO)
    Jugador j1={"CARLOS","GARCIA","charlySNM",0,'S',0};
    agregarJugador(j1);
    //CARGA JUGADOR 2 (POR DEFECTO)
    Jugador j2={"CHIZZO","NAPOLI","fabianStone67",0,'S',0};
    agregarJugador(j2);
    //CARGA JUGADOR 3 (POR DEFECTO)
    Jugador j3={"OSCAR","MORO","elBata",0,'J',0};
    agregarJugador(j3);
    //CARGA JUGADOR 4 (POR DEFECTO)
    Jugador j4={"FABIANA","CANTILO","losTwist",0,'J',0};
    agregarJugador(j4);

    cout<<"-- JUGADORES CREADOS POR DEFECTO --"<<endl;
}

void mostrarJugadores() {
    FILE* archivo = fopen("datosJugadores.dat","rb");

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
            cout<<"PUNTAJE: "<<t.puntaje<<endl;
            cout<<endl;
        }
        fclose(archivo);
    }else {
        cout<<"-- ERROR: ARCHIVO -- "<<endl;
    }

}

int cantidadDeJugadores() {
    FILE* archivo = fopen("datosJugadores.dat","rb");

    if (archivo!=NULL) {
        Jugador t;
        int contador=0;
        while (fread(&t,sizeof(Jugador),1,archivo)==1)
            contador++;
        fclose(archivo);
        return contador;
    }else {
        return 0;
    }
}













