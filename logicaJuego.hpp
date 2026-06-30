/*
 * logicaJuego.hpp
 * Modulo: Logica del Juego
 * Descripcion: Contiene las funciones de generacion del camino, seleccion de jugadores
 *              y clasificacion de numeros segun sus propiedades matematicas
 *              (primo, amigo, capicua, perfecto). Estas propiedades determinan
 *              los efectos que ocurren cuando un jugador cae en una baldosa.
 */

#include <iostream>
#include <time.h>
#include "jugadores.hpp"
#include "abb.hpp"
#include "camino.hpp"

using namespace std;

/*
 * Pre:  Recibe un entero positivo x (posicion en la secuencia, comenzando en 1).
 * Post: Retorna el x-esimo numero de Fibonacci usando la secuencia desplazada
 *       (1, 2, 3, 5, 8, ...). Se utiliza para determinar cuantas baldosas validas
 *       se insertan de forma consecutiva antes de colocar una baldosa vacia.
 *       Si x <= 0 retorna 0.
 */
int fibonacci(int x) {
    if (x <= 0) return 0;
    if (x == 1) return 1;
    if (x == 2) return 2;
    return fibonacci(x - 1) + fibonacci(x - 2);
}

/*
 * Pre:  Recibe una referencia a una listaD vacia (o previamente destruida).
 * Post: Genera aleatoriamente el camino del juego con entre 30 y 50 baldosas.
 *       La primera y ultima baldosa siempre estan vacias (valor 0).
 *       Las demas baldosas alternan entre bloques de baldosas con valores unicos
 *       (cuya cantidad sigue la secuencia de Fibonacci) y baldosas vacias.
 *       Se utiliza un ABB para garantizar que no haya valores repetidos.
 *       La orientacion del camino (izquierda-derecha o derecha-izquierda)
 *       se elige aleatoriamente.
 */
void generarCamino(listaD &camino) {
    iniciarListaD(camino);
    srand(time(NULL));

    raiz historial;
    camino.tamanio = rand() % 21 + 30; // Tamanio entre 30 y 50
    int random, cantInser = 0, orientacion = rand() % 2;
    int fIndex = 1;

    iniciarArbol(historial);

    if (orientacion == 0) {
        // Camino de izquierda a derecha
        cout << "+-+ CAMINO IZQ - DER +-+" << endl;
        for (int x = 0; x < camino.tamanio; x++) {
            if (x == 0 || x == camino.tamanio - 1) {
                // Extremos siempre vacios
                insertarFinal(camino, 0, x, false);
            } else if (cantInser < fibonacci(fIndex)) {
                // Insertar baldosa con valor unico
                do {
                    random = rand() % 4999 + 1;
                } while (valorRepetido(historial, random));
                insertarFinal(camino, random, x, true);
                cargarArbol(historial, random);
                cantInser++;
            } else {
                // Baldosa vacia que separa los bloques
                insertarFinal(camino, 0, x, false);
                cantInser = 0;
                fIndex++;
                if (fIndex > 5) fIndex = 1; // Cicla entre los primeros 5 de Fibonacci
            }
        }
    } else {
        // Camino de derecha a izquierda
        cout << "+-+ CAMINO DER - IZQ +-+" << endl;
        for (int x = camino.tamanio - 1; x >= 0; x--) {
            if (x == 0 || x == camino.tamanio - 1) {
                insertarInicio(camino, 0, x, false);
            } else if (cantInser < fibonacci(fIndex)) {
                do {
                    random = rand() % 4999 + 1;
                } while (valorRepetido(historial, random));
                insertarInicio(camino, random, x, true);
                cargarArbol(historial, random);
                cantInser++;
            } else {
                insertarInicio(camino, 0, x, false);
                cantInser = 0;
                fIndex++;
                if (fIndex > 5) fIndex = 1;
            }
        }
    }

    cout << "+-+ CAMINO GENERADO +-+" << endl;
    cout << "+-+ tamanio: " << camino.tamanio << endl;
}

/*
 * Pre:  Deben existir al menos 2 jugadores en el archivo.
 * Post: Muestra la lista de jugadores registrados, solicita al usuario
 *       seleccionar dos jugadores distintos por numero de posicion,
 *       y carga sus datos en j1 y j2.
 */
void seleccionarJugadores(Jugador &j1, Jugador &j2) {
    FILE* archivo = fopen("datosJugadores.dat", "rb");
    if (archivo == NULL) {
        cout << "--SIN JUGADORES--" << endl;
        return;
    }

    if (cantidadDeJugadores() < 2) {
        cout << "Se necesitan al menos 2 jugadores." << endl;
        fclose(archivo);
        return;
    }

    // Carga todos los jugadores en un arreglo temporal para mostrarlos
    Jugador lista[50];
    int cant = 0;
    while (fread(&lista[cant], sizeof(Jugador), 1, archivo) == 1) {
        cant++;
    }
    fclose(archivo);

    cout << "---Seleccion de jugadores---" << endl;
    for (int i = 0; i < cant; i++) {
        cout << i + 1 << ". " << lista[i].nickname << " (" << lista[i].categoria << ")" << endl;
    }

    int op1, op2;

    // Seleccion del primer jugador con validacion de rango
    do {
        cout << "\nSeleccione Jugador 1 (1-" << cant << "): ";
        cin >> op1;
        if (op1 < 1 || op1 > cant) {
            cout << "Opcion incorrecta" << endl;
        }
    } while (op1 < 1 || op1 > cant);

    // Seleccion del segundo jugador: no puede repetirse ni salir del rango
    do {
        cout << "Selecione jugador 2 (1-" << cant << "): ";
        cin >> op2;
        if (op2 < 1 || op2 > cant) {
            cout << "Opcion incorrecta" << endl;
        } else if (op2 == op1) {
            cout << "Error: un jugador no puede enfrentarse a si mismo." << endl;
        }
    } while (op2 < 1 || op2 > cant || op2 == op1);

    j1 = lista[op1 - 1];
    j2 = lista[op2 - 1];
    cout << "\nJugador 1: " << j1.nickname << endl;
    cout << "Jugador 2: " << j2.nickname << endl;
}

/*
 * Pre:  Recibe un entero n.
 * Post: Retorna true si n es un numero capicua (se lee igual de izquierda a derecha
 *       que de derecha a izquierda), false en caso contrario.
 */
bool esCapicua(int n) {
    int original = n, invertido = 0;
    while (n > 0) {
        invertido = invertido * 10 + n % 10;
        n /= 10;
    }
    return original == invertido;
}

/*
 * Pre:  Recibe un entero n.
 * Post: Retorna true si n es un numero primo (divisible solo por 1 y por si mismo),
 *       false en caso contrario. Numeros menores a 2 no son primos.
 */
bool esPrimo(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

/*
 * Pre:  Recibe un entero n positivo.
 * Post: Retorna true si n es un numero perfecto (igual a la suma de sus
 *       divisores propios), false en caso contrario.
 *       Ejemplo: 6 = 1 + 2 + 3.
 */
bool esPerfecto(int n) {
    int suma = 0;
    for (int i = 1; i < n; i++)
        if (n % i == 0) suma += i;
    return suma == n;
}

/*
 * Pre:  Recibe dos enteros positivos a y b.
 * Post: Retorna true si a y b son numeros amigos entre si.
 *       Dos numeros son amigos si la suma de los divisores propios de a es igual a b
 *       y la suma de los divisores propios de b es igual a a.
 *       Ejemplo: 220 y 284.
 */
bool esAmigo(int a, int b) {
    int sumaA = 0, sumaB = 0;
    for (int i = 1; i < a; i++) if (a % i == 0) sumaA += i;
    for (int i = 1; i < b; i++) if (b % i == 0) sumaB += i;
    return sumaA == b && sumaB == a;
}

/*
 * Pre:  Recibe el valor de la baldosa actual (num) y el valor de la
 *       ultima baldosa visitada valida (ultimoVisitado).
 * Post: Clasifica el numero segun sus propiedades y retorna:
 *       1 si es primo (maligno),
 *       2 si es amigo con el ultimo visitado,
 *       3 si es capicua,
 *       4 si es perfecto,
 *       0 si no cumple ninguna de las condiciones anteriores (comun).
 *       El orden de evaluacion determina la prioridad en caso de que
 *       un numero cumpla mas de una condicion.
 */
int controlNumero(int num, int ultimoVisitado) {
    if (esPrimo(num))                   return 1;
    if (esAmigo(num, ultimoVisitado))   return 2;
    if (esCapicua(num))                 return 3;
    if (esPerfecto(num))                return 4;
    return 0;
}
