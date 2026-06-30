/*
 * pilaAvance.hpp
 * Modulo: Pila Dinamica de Avance
 * Descripcion: Implementacion de una pila dinamica de enteros.
 *              Se utiliza para registrar el recorrido (posiciones visitadas)
 *              de cada jugador a lo largo del camino durante la partida.
 */

#include <iostream>
using namespace std;

/* Estructura de cada nodo de la pila */
struct nodoP {
    int valor;      // Posicion (numero de baldosa) almacenada en el nodo
    nodoP* abajo;   // Puntero al nodo inferior de la pila
};

/* Tipo puntero al nodo superior de la pila (tope) */
typedef nodoP* tope;

/*
 * Pre:  Recibe una referencia al tope de la pila.
 * Post: Inicializa la pila como vacia (tope en NULL).
 */
void inicializarPila(tope &p) {
    p = NULL;
}

/*
 * Pre:  Recibe una referencia al tope de la pila.
 * Post: Retorna true si la pila esta vacia, false en caso contrario.
 */
bool pilaVacia(tope &p) {
    return p == NULL;
}

/*
 * Pre:  Recibe una referencia al tope de la pila (no debe estar vacia para un resultado valido).
 * Post: Extrae y retorna el valor del nodo en el tope de la pila.
 *       Si la pila esta vacia, retorna -1 como indicador de error.
 *       Libera la memoria del nodo extraido.
 */
int desapilarPila(tope &p) {
    if (pilaVacia(p)) {
        return -1; // Indicador de error: pila vacia
    }

    tope q = p;
    int dato = q->valor;

    p = p->abajo; // El tope pasa al nodo siguiente
    delete q;     // Se libera el nodo extraido

    return dato;
}

/*
 * Pre:  Recibe una referencia al tope de la pila y un valor entero a apilar.
 * Post: Crea un nuevo nodo con el valor recibido y lo inserta en el tope de la pila.
 */
void cargarPila(tope &p, int ingr) {
    tope q = new nodoP;
    q->valor = ingr;

    if (pilaVacia(p)) {
        // Si la pila esta vacia, el nuevo nodo queda solo
        q->abajo = NULL;
        p = q;
        return;
    }

    // El nuevo nodo apunta al nodo que era el tope anterior
    q->abajo = p;
    p = q;
}

/*
 * Pre:  Recibe una referencia al tope de la pila.
 * Post: Libera toda la memoria ocupada por los nodos de la pila
 *       y deja el tope en NULL.
 */
void destruirPila(tope &p) {
    if (pilaVacia(p)) { return; }

    while (p != NULL) {
        tope q = p->abajo;
        delete p;
        p = q;
    }
}
