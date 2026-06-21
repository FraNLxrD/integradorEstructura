#include <iostream>
#include <time.h>
using namespace std;


struct nodoLD {
    int valor;
    bool valid=0;
    int nro;
    nodo *ant;
    nodo *sig;
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

bool isEmpty(listaD &l) {}

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
    p->ant= l.ultimo;

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