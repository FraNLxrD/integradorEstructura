#include <iostream>
using namespace std;


struct nodo {
    int valor;
    nodo *ant;
    nodo *sig;
};


typedef nodo *tListaD;

struct listaD {
    tListaD inicio;
    tListaD ultimo;
};

void iniciarListaD(tListaD &l) {
    l.inicio = NULL;
    L.ultimo = NULL;
};
