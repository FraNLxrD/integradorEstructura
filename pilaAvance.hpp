#include <iostream>

using namespace std;


struct nodoP {
    int valor;
    nodoP* abajo;
};

typedef nodoP* tope;

void inicializarPila(tope &p) {
    p=NULL;
}


bool pilaVacia(tope &p) {
    return p == NULL;
}

void cargarPila(tope &p, int ingr) {
    tope q= new nodoP;
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




