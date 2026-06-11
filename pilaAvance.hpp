#include <iostream>

using namespace std;

;
struct nodo {
    int valor;
    nodo* abajo;
};

typedef nodo* tope;

void inicializarPila(tope &p) {
    p=NULL;
}

void iniciarAvance(tope &p, bool der, int tam) {
    p->abajo= NULL;
    if (der) {
        p->valor= tam;
    }else {
        p->valor= 0;
    }
}

bool pilaVacia(tope &p) {
    return p == NULL;
}

void cargarPila(tope &p, int ingr) {
    tope q= new nodo;
    q->valor = ingr;
    if (pilaVacia(p)) {
        q->abajo= NULL;
        p=q;
        return;
    }
    q->abajo= p;
    p = q;
}

void destruirPila(tope &p) {
    if (pilaVacia(p)) {return;}
    while (p!=NULL) {
        tope q = p->abajo;
        delete p;
        p = q;
    }
}




