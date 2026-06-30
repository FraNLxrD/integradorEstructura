/*
 * abb.hpp
 * Modulo: Arbol Binario de Busqueda (ABB)
 * Descripcion: Implementacion de un ABB para almacenar valores enteros.
 *              Se utiliza para registrar los numeros ya insertados en el camino
 *              y verificar que no se repitan.
 */

#include <iostream>
using namespace std;

/* Tipo de dato que almacena el ABB */
typedef int tipodato;

/* Estructura de cada nodo del arbol */
struct nodo {
    nodo* izq;      // Puntero al hijo izquierdo
    nodo* der;      // Puntero al hijo derecho
    tipodato valor; // Valor almacenado en el nodo
};

/* Tipo puntero a nodo, representa la raiz del arbol */
typedef nodo* raiz;

/*
 * Pre:  Recibe una referencia a la raiz del arbol.
 * Post: Inicializa el arbol como vacio (raiz en NULL).
 */
void iniciarArbol(raiz &r) {
    r = NULL;
}

/*
 * Pre:  Recibe una referencia a la raiz del arbol.
 * Post: Retorna true si el arbol esta vacio, false en caso contrario.
 */
bool arbolVacio(raiz &r) {
    return r == NULL;
}

/*
 * Pre:  Recibe la raiz del arbol y un valor entero a insertar.
 * Post: Inserta el valor en la posicion correcta del ABB.
 *       Si el valor ya existe, no realiza ninguna insercion (sin duplicados).
 */
void cargarArbol(raiz &r, tipodato ingr) {
    raiz p = new nodo;
    raiz q = r;
    p->valor = ingr;
    p->izq = NULL;
    p->der = NULL;

    // Si el arbol esta vacio, el nuevo nodo se convierte en la raiz
    if (arbolVacio(r)) {
        r = p;
        return;
    }

    // Recorre el arbol para encontrar la posicion de insercion correcta
    while (q != NULL) {
        if (q->valor < p->valor) {
            // El valor es mayor, se va a la derecha
            if (q->der == NULL) {
                q->der = p;
                return;
            }
            q = q->der;
        } else if (q->valor > p->valor) {
            // El valor es menor, se va a la izquierda
            if (q->izq == NULL) {
                q->izq = p;
                return;
            }
            q = q->izq;
        } else {
            // El valor ya existe en el arbol, se libera el nodo creado
            delete p;
            return;
        }
    }
}

/*
 * Pre:  Recibe la raiz del arbol y un valor a buscar.
 * Post: Retorna true si el valor ya existe en el arbol, false en caso contrario.
 */
bool valorRepetido(raiz &r, tipodato comp) {
    if (arbolVacio(r)) { return false; }

    raiz p = r;
    while (p != NULL) {
        if (comp == p->valor) {
            return true;        // Valor encontrado
        } else if (comp < p->valor) {
            p = p->izq;         // Busca en el subarbol izquierdo
        } else {
            p = p->der;         // Busca en el subarbol derecho
        }
    }
    return false;
}

/*
 * Pre:  Recibe una referencia a la raiz del arbol.
 * Post: Libera toda la memoria ocupada por el arbol (recorrido postorden)
 *       y deja la raiz en NULL.
 */
void destruirArbol(raiz &r) {
    if (arbolVacio(r)) {
        return;
    }
    // Se destruyen primero los subarboles antes de eliminar el nodo actual
    destruirArbol(r->izq);
    destruirArbol(r->der);
    delete r;
    iniciarArbol(r);
}
