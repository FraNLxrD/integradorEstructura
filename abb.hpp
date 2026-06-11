#include <iostream>
using namespace std;

typedef int tipodato;
struct nodo {
    nodo* izq;
    nodo* der;
    tipodato valor;
};

typedef nodo* raiz;
void iniciarArbol(raiz &r) {
    r=NULL;
}

bool arbolVacio (raiz &r) {
    return r==NULL;
}

void cargarArbol(raiz &r, tipodato ingr) {
    raiz p = new nodo;
    raiz q= r;
    p->valor=ingr;
    p->izq=NULL;
    p->der=NULL;
    if (arbolVacio(r)) {
        r = p;
        return;
    }
    while (q!=NULL) {

        if (q->valor<p->valor) {
            if (q->der==NULL) {
                q->der=p;
                return;
            }
            q=q->der;
        }

        if (q->valor>p->valor) {
            if (q->izq==NULL) {
                q->izq=p;
                return;
            }
            q=q->izq;
        }

        if (q->valor=p->valor) {
            delete p;
            return;
        }
    }
}

bool valorRepetido(raiz &r, tipodato comp) {
    if (arbolVacio(r)) {return false;}
    raiz p= r;
    while (p!=NULL) {
        if (comp==p->valor) {
            return true;
        }

        if (comp<p->valor) {
            p=p->izq;
        }
        if (comp>p->valor) {
            p=p->der;
        }
    }
    return false;
}

void destruirArbol(raiz &r) {

    if (arbolVacio(r)) {
        return;
    }

    destruirArbol(r->izq);
    destruirArbol(r->der);

    delete r;
    iniciarArbol(r);
}