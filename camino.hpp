/*
 * camino.hpp
 * Modulo: Lista Doblemente Enlazada - Camino Minado
 * Descripcion: Implementacion de una lista doblemente enlazada que representa
 *              el camino del juego. Cada nodo es una baldosa con un numero
 *              de posicion, un valor numerico y un indicador de validez.
 */

#include <iostream>
#include <iomanip>
#include <time.h>
using namespace std;

/* Estructura de cada nodo (baldosa) del camino */
struct nodoLD {
    int valor;      // Numero asignado a la baldosa (0 si esta vacia)
    bool valid = 0; // Indica si la baldosa tiene un numero valido (true) o esta vacia (false)
    int nro;        // Posicion de la baldosa dentro del camino (0 a tamanio-1)
    nodoLD* ant;    // Puntero a la baldosa anterior
    nodoLD* sig;    // Puntero a la baldosa siguiente
};

/* Tipo puntero a nodo de la lista doblemente enlazada */
typedef nodoLD* tNodoD;

/* Estructura que representa la lista doblemente enlazada (el camino completo) */
struct listaD {
    tNodoD inicio;  // Puntero a la primera baldosa del camino
    tNodoD ultimo;  // Puntero a la ultima baldosa del camino
    int tamanio;    // Cantidad total de baldosas en el camino
};

/*
 * Pre:  Recibe una referencia a la lista.
 * Post: Inicializa la lista como vacia (punteros en NULL, tamanio en 0).
 */
void iniciarListaD(listaD &l) {
    l.inicio = NULL;
    l.ultimo = NULL;
    l.tamanio = 0;
}

/*
 * Pre:  Recibe una referencia a la lista.
 * Post: Retorna true si la lista esta vacia, false en caso contrario.
 */
bool isEmpty(listaD &l) {
    return l.inicio == NULL;
}

/*
 * Pre:  Recibe la lista, un valor, el numero de posicion y un indicador de validez.
 * Post: Inserta una nueva baldosa al final de la lista con los datos recibidos.
 *       Actualiza el puntero "ultimo" y el campo "sig" del nodo anterior.
 */
void insertarFinal(listaD &l, int valor, int nro, bool valido) {
    tNodoD p = new nodoLD;
    p->valor = valor;
    p->nro   = nro;
    p->valid = valido;

    if (isEmpty(l)) {
        // Lista vacia: el nuevo nodo es inicio y ultimo
        l.inicio = p;
        l.ultimo = p;
        p->sig = NULL;
        p->ant = NULL;
        return;
    }

    // Enlaza el nuevo nodo al final
    p->sig         = NULL;
    p->ant         = l.ultimo;
    l.ultimo->sig  = p;
    l.ultimo       = p;
}

/*
 * Pre:  Recibe la lista, un valor, el numero de posicion y un indicador de validez.
 * Post: Inserta una nueva baldosa al inicio de la lista con los datos recibidos.
 *       Actualiza el puntero "inicio" y el campo "ant" del nodo siguiente.
 */
void insertarInicio(listaD &l, int valor, int nro, bool valido) {
    tNodoD p = new nodoLD;
    p->valor = valor;
    p->nro   = nro;
    p->valid = valido;

    if (isEmpty(l)) {
        // Lista vacia: el nuevo nodo es inicio y ultimo
        l.inicio = p;
        l.ultimo = p;
        p->sig = NULL;
        p->ant = NULL;
        return;
    }

    // Enlaza el nuevo nodo al principio
    p->sig          = l.inicio;
    p->ant          = NULL;
    l.inicio->ant   = p;
    l.inicio        = p;
}

/*
 * Pre:  Recibe la lista y el numero de posicion (nro) a buscar.
 * Post: Recorre la lista y retorna el valor de la baldosa con el numero indicado.
 *       Si no se encuentra, imprime un mensaje de error y retorna -1.
 */
int consultarNodo(listaD &l, int nro) {
    tNodoD p = l.inicio;

    while (p != NULL) {
        if (p->nro == nro) {
            return p->valor; // Baldosa encontrada
        }
        p = p->sig;
    }

    cout << "-- ERROR NODO NO ENCONTRADO -- " << endl;
    return -1;
}

/*
 * Pre:  Recibe una referencia a la lista.
 * Post: Recorre e imprime todos los valores del camino en orden.
 *       Si la lista esta vacia, muestra un mensaje indicandolo.
 */
void mostrarCamino(listaD &l) {
    if (l.inicio == NULL) {
        cout << "--- LISTA VACIA ---" << endl;
        return;
    }
    tNodoD p = l.inicio;
    while (p != NULL) {
        cout << "<-->|" << p->valor << "|<---> ";
        p = p->sig;
    }
    cout << endl;
}

/*
 * Pre:  Recibe una referencia a la lista.
 * Post: Libera toda la memoria de los nodos del camino y reinicializa la lista.
 */
void destruirCamino(listaD &l) {
    tNodoD p = l.inicio;
    tNodoD aux;
    while (p != NULL) {
        aux = p;
        p   = p->sig;
        delete aux;
    }
    iniciarListaD(l);
}

/*
 * Pre:  Recibe la lista y el numero de posicion de una baldosa.
 * Post: Retorna true si la baldosa tiene un numero valido asignado,
 *       false si esta vacia o no se encuentra en la lista.
 */
bool baldosaValida(listaD &l, int nro) {
    tNodoD p = l.inicio;
    while (p != NULL) {
        if (p->nro == nro) return p->valid;
        p = p->sig;
    }
    return false;
}

/*
 * Pre:  Recibe la lista y el tamanio del camino.
 * Post: Muestra el camino de forma visual en filas de 10 baldosas,
 *       alternando la direccion de lectura para simular un tablero en serpentina.
 *       Las baldosas vacias se muestran en blanco y las validas muestran su valor.
 */
void mostrarCaminoVisual(listaD &l, int tamano) {
    // Se cargan todos los nodos en un arreglo para facilitar el acceso por indice
    tNodoD nodos[50];
    int total = 0;
    tNodoD aux = l.inicio;
    while (aux != NULL) {
        nodos[total++] = aux;
        aux = aux->sig;
    }

    int filaSize = 10; // Cantidad de baldosas por fila
    int fila = 0;
    int i = 0;

    while (i < total) {
        int fin = i + filaSize - 1;
        if (fin >= total) fin = total - 1;

        // Separador superior de la fila
        for (int k = i; k <= fin; k++) cout << "+----------";
        cout << "+" << endl;

        // Filas pares: se muestran de izquierda a derecha
        // Filas impares: se muestran de derecha a izquierda (efecto serpentina)
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

        // Separador inferior de la fila
        for (int k = i; k <= fin; k++) cout << "+----------";
        cout << "+" << endl;
        cout << endl;

        i += filaSize;
        fila++;
    }
}
