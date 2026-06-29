#include <iostream>
#include <iomanip>
#include <time.h>
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
    int tamanio;
};


void iniciarListaD(listaD &l) {
    l.inicio = NULL;
    l.ultimo = NULL;
    l.tamanio = 0;
}

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

// Pre: recibe la lista y un numero de nodo
// Post: retorna true si la baldosa tiene numero valido
bool baldosaValida(listaD &l, int nro) {
    tNodoD p = l.inicio;
    while (p != NULL) {
        if (p->nro == nro) return p->valid;
        p = p->sig;
    }
    return false;
}

// Pre: recibe la lista y el tamano del camino
// Post: muestra el camino visualmente en filas alternadas como un tablero
void mostrarCaminoVisual(listaD &l, int tamano) {
    tNodoD nodos[50];
    int total = 0;
    tNodoD aux = l.inicio;
    while (aux != NULL) {
        nodos[total++] = aux;
        aux = aux->sig;
    }

    int filaSize = 10;
    int fila = 0;
    int i = 0;

    while (i < total) {
        int fin = i + filaSize - 1;
        if (fin >= total) fin = total - 1;

        // Separador superior
        for (int k = i; k <= fin; k++) cout << "+----------";
        cout << "+" << endl;

        // Filas pares: izq a der / Filas impares: der a izq
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
