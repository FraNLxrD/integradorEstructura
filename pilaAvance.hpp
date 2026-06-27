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

int desapilarPila(tope &p) {
    if (pilaVacia(p)) {
        return -1; // o el valor que quieras usar para indicar error
    }

    tope q = p;
    int dato = q->valor;

    p = p->abajo;
    delete q;

    return dato;
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




