#include <iostream>
#include <time.h>
#include "jugadores.hpp"
#include "abb.hpp"
#include "camino.hpp"



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



void generarCamino(listaD &camino, int &tamanio) {

    iniciarListaD(camino);
    srand(time(NULL));
    int orientacion = rand() % 2;
    tamanio = rand() % 21 + 30;


    int random;
    raiz historial;

    //asignacion random

    int cantFibo=0;
    int cantInser=0;


    iniciarArbol(historial);
    if (orientacion == 0) {
    	cout<<"+-+ CAMINO IZQ - DER +-+"<<endl;
        for (int x=0; x<tamanio; x++) {

            if (cantFibo !=cantInser) {
                do {
                    random = rand() % 4999 + 1;
                }while (valorRepetido(historial,random));

                insertarFinal(camino,random,x,true);
                cantInser++;
            }else {
                cantInser=0;
                cantFibo=fibonacci(cantFibo);
                insertarFinal(camino,0,x,false);
            }
        }
    }else {
    	cout<<"+-+ CAMINO DER - IZQ +-+"<<endl;
        for (int x=tamanio-1; x>=0; x--) {

            if (cantFibo !=cantInser) {
                do {
                    random = rand() % 4999 + 1;
                }while (valorRepetido(historial,random));

                insertarInicio(camino,random,x,true);
                cantInser++;
            }else {
                cantInser=0;
                cantFibo=fibonacci(cantFibo);
                insertarInicio(camino,0,x,false);
            }

        }

    }

	cout<<"+-+ CAMINO GENERADO +-+"<<endl;
	cout<<"+-+ TAMAÑO: "<<tamanio<<endl;

}





void seleccionarJugadores(Jugador &j1, Jugador &j2){
	//verifica que exitan jugadores o esta vacio
	FILE* archivo=fopen("datosJugadores.bin","rb");
	if(archivo==NULL){
		cout<<"--SIN JUGADORES--"<<endl;
		return;
	}
	//cuenta los jugadores

	if (cantidadDeJugadores()<2){
		cout<<"Se necesitan al menos 2 jugadores."<<endl;
		fclose(archivo);
		return;
	}
	//carga todos los arreglos temporal
	Jugador lista[50];
	int cant=0;
	while (fread(&lista[cant],sizeof(Jugador),1,archivo)==1){
		cant++;
	}
	fclose(archivo);
	//mostrar lista
	cout<<"---Seleccion de jugadores---"<<endl;
	for(int i=0;i<cant;i++){
		cout<<i+1<<". "<<lista[i].nickname<<" ("<<lista[i].categoria<<")"<<endl;
	}
	
	int op1,op2;
	
	//seleccion jugador 1
	do{
		cout<<"\nSeleccione Jugador 1 (1-"<<cant<<"): ";
		cin>>op1;
		if(op1<1||op1>cant){
			cout<<"Opcion incorrecta"<<endl;
		}
	}while(op1<1||op1>cant);
	
	//seleccion jugador 2-no puede ser el mismo
	do{
		cout<<"Selecione jugador 2 (1-"<<cant<<"): ";
		cin>>op2;
		if(op2<1||op2>cant){
			cout<<"Opcion incorrecta"<<endl;
		}else{
			if (op2==op1){
				cout<<"Error: un jugador no puede enfrentarse a si mismo."<<endl;
			}
		}
	}while(op2<1||op2>cant||op2==op1);
	
	j1=lista[op1-1];
	j2=lista[op2-1];
	cout<<"\njugador 1: "<<j1.nickname<<endl;
	cout<<"Jugador 2: "<<j2.nickname<<endl;

}
// Pre: recibe un numero y el ultimo numero visitado
// Post: retorna el tipo: 0=comun, 1=primo, 2=amigo, 3=capicua, 4=perfecto
bool esCapicua(int n) {
	int original = n, invertido = 0;
	while (n > 0) {
		invertido = invertido * 10 + n % 10;
		n /= 10;
	}
	return original == invertido;
}

bool esPrimo(int n) {
	if (n < 2) return false;
	for (int i = 2; i * i <= n; i++)
		if (n % i == 0) return false;
	return true;
}

bool esPerfecto(int n) {
	int suma = 0;
	for (int i = 1; i < n; i++)
		if (n % i == 0) suma += i;
	return suma == n;
}

bool esAmigo(int a, int b) {
	// dos numeros son amigos si la suma de divisores de a = b y viceversa
	int sumaA = 0, sumaB = 0;
	for (int i = 1; i < a; i++) if (a % i == 0) sumaA += i;
	for (int i = 1; i < b; i++) if (b % i == 0) sumaB += i;
	return sumaA == b && sumaB == a;
}
int controlNumero(int num, int ultimoVisitado) {
    if (esPrimo(num))    return 1;
    if (esAmigo(num, ultimoVisitado)) return 2;
    if (esCapicua(num))  return 3;
    if (esPerfecto(num)) return 4;
    return 0;
}
