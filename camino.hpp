#include <iostream>
#include <time.h>
#include <iomanip>
#include "camino.hpp"
using namespace std;


struct nodoLD {
    int valor;
    bool valid=0;
    int nro;
    nodoLD *ant;
    nodoLD *sig;
};


typedef nodoLD* tNodoD;

struct listaD {
    tNodoD inicio;
    tNodoD ultimo;
};


void iniciarListaD(listaD &l) {
    l.inicio = NULL;
    l.ultimo = NULL;
};

bool isEmpty(listaD &l) {return l.inicio == NULL; }

void insertarFinal(listaD &l, int valor, int nro, bool valido) {
    tNodoD p= new nodoLD;
    p->valor = valor;
    p->nro = nro;
    p->valid= valido;
    if (isEmpty(l)) {
        l.inicio = p;
        l.ultimo = p;
        p->sig = NULL;
        p->ant = NULL;
        return;
    }

    p->sig= NULL;
    p->ant = l.ultimo;

    l.ultimo->sig = p;
    l.ultimo = p;
    return;

}

void insertarInicio(listaD &l, int valor, int nro, bool valido) {
    tNodoD p = new nodoLD;
    p->valor = valor;
    p->nro = nro;
    p->valid= valido;

    if (isEmpty(l)) {
        l.inicio = p;
        l.ultimo = p;
        p->sig = NULL;
        p->ant = NULL;
        return;
    }

    p->sig= l.inicio;
    p->ant= NULL;

    l.inicio->ant = p;
    l.inicio = p;
    return;

}

int consultarNodo(listaD &l, int nro) {
    tNodoD p = l.inicio;


    while (p!=NULL) {
        if (p->nro == nro) {
            return p->valor;
        }
        p = p->sig;
    }
    cout<<"-- ERROR NODO NO ENCONTRADO -- "<<endl;
    return -1;
}

void mostrarCamino(listaD &l) {
    if (l.inicio == NULL) {
        cout << "--- LISTA VACIA ---" << endl;
        return;
    }
    tNodoD p = l.inicio;
    while (p!=NULL) {
        cout<<"<-->|"<<p->valor<<"|<---> ";
        p = p->sig;
    }
    cout<<endl;
}

void destruirCamino(listaD &l) {
    tNodoD p = l.inicio;
    tNodoD aux;
    while (p != NULL) {
        aux = p;
        p = p->sig;
        delete aux;
    }
    iniciarListaD(l);
}
// Pre: recibe una lista, un arbol para control de repetidos y el tamano por referencia
// Post: genera el camino con tamano aleatorio entre 30 y 50, minado segun Fibonacci
void generarCamino(listaD &l, raiz &r, int &tamano) {
    // Tamano aleatorio entre 30 y 50
    tamano = 30 + rand() % 21;

    // Primeros 5 elementos de Fibonacci
    int fib[5] = {1, 1, 2, 3, 5};

    // Crear todas las baldosas vacias
    for (int i = 0; i < tamano; i++) {
        insertarFinal(l, 0, i, false);
    }

    // Decidir direccion aleatoriamente: 0=izquierda a derecha, 1=derecha a izquierda
    int direccion = rand() % 2;

    // Posicionarse en la segunda o anteultima baldosa (primera y ultima quedan libres)
    tNodoD actual;
    if (direccion == 0)
        actual = l.inicio->sig;
    else
        actual = l.ultimo->ant;

    int iFib = 0;
    bool terminar = false;
    // Recorrer el camino minando segun la serie de Fibonacci
    while (!terminar && actual != NULL && actual->sig != NULL && actual->ant != NULL) {
        // Generar numero unico no repetido entre 1 y 4999
        int num;
        do {
            num = 1 + rand() % 4999;
        } while (valorRepetido(r, num));
        // Asignar el numero a la baldosa actual
        actual->valor = num;
        actual->valid = true;
        // Guardar en el ABB para control de repetidos
        cargarArbol(r, num);
        // Calcular salto: fibonacci[i] + 1 posicion libre
        int salto = fib[iFib % 5] + 1;
        // Avanzar en la direccion correspondiente
        for (int i = 0; i < salto && !terminar; i++) {
            if (direccion == 0) {
                // Si no hay mas espacio adelante, terminar
                if (actual->sig == NULL || actual->sig->sig == NULL) {
                    terminar = true;
                } else {
                    actual = actual->sig;
                }
            } else {
                // Si no hay mas espacio atras, terminar
                if (actual->ant == NULL || actual->ant->ant == NULL) {
                    terminar = true;
                } else {
                    actual = actual->ant;
                }
            }
        }
        iFib++;
    }
    cout << "CAMINO MINADO" << endl;
}

// Pre: recibe una lista y el tamano del camino
// Post: muestra el camino visualmente en filas alternadas como un tablero
void mostrarCaminoVisual(listaD &l, int tamano) {
    // Guardar todos los nodos en un arreglo para poder recorrerlos por indice
    tNodoD nodos[50];
    int total = 0;
    tNodoD aux = l.inicio;
    while (aux != NULL) {
        nodos[total++] = aux;
        aux = aux->sig;
    }
    int filaSize = 10; // baldosas por fila
    int fila = 0;
    int i = 0;
    while (i < total) {
        int fin = i + filaSize - 1;
        if (fin >= total) fin = total - 1;
        // Separador superior
        for (int k = i; k <= fin; k++) cout << "+----------";
        cout << "+" << endl;
        // Filas pares: izquierda a derecha / Filas impares: derecha a izquierda
        if (fila % 2 == 0) {
            for (int j = i; j <= fin; j++) {
                if (nodos[j]->valid)
                    cout << "|  " << left << setw(6) << nodos[j]->valor << "  ";
                else
                    cout << "|          ";
            }
        } else {
            for (int j = fin; j >= i; j--) {
                if (nodos[j]->valid)
                    cout << "|  " << left << setw(6) << nodos[j]->valor << "  ";
                else
                    cout << "|          ";
            }
        }
        cout << "|" << endl;
        // Separador inferior
        for (int k = i; k <= fin; k++) cout << "+----------";
        cout << "+" << endl;
        cout << endl;

        i += filaSize;
        fila++;
    }
}