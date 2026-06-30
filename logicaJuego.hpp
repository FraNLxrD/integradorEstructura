/*
 * logicaJuego.hpp
 * Modulo: Logica del Juego
 * Descripcion: Contiene las funciones de generacion del camino, seleccion de jugadores
 *              y clasificacion de numeros segun sus propiedades matematicas
 *              (primo, amigo, capicua, perfecto). Estas propiedades determinan
 *              los efectos que ocurren cuando un jugador cae en una baldosa.
*               Y las funciones que gestionan el flujo
 *              de una partida: configuracion, turnos, efectos de baldosas
 *              y finalizacion con actualizacion de estadisticas.
 */

#include <iostream>
#include <time.h>
#include "jugadores.hpp"
#include "camino.hpp"
#include "pilaAvance.hpp"
using namespace std;

/*
 * Pre:  Recibe un entero positivo x (posicion en la secuencia, comenzando en 1).
 * Post: Retorna el x-esimo numero de Fibonacci usando la secuencia desplazada
 *       (1, 2, 3, 5, 8, ...). Se utiliza para determinar cuantas baldosas validas
 *       se insertan de forma consecutiva antes de colocar una baldosa vacia.
 *       Si x <= 0 retorna 0.
 */

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
void intercambio(Jugador &j1, Jugador &j2);
void finalPartida(Jugador &ganador, Jugador &perdedor, int gan, int pen, bool &rach);

/*
 * Pre:  Recibe referencias a dos jugadores y una variable booleana j1Izquierda.
 * Post: Determina quien tira primero y desde que extremo sale cada jugador.
 *       Si ambos son de la misma categoria, la eleccion es aleatoria.
 *       Si son de distinta categoria, el jugador Senior elige el orden de salida
 *       y el jugador que inicia elige el extremo del que parte.
 *       Al terminar, j1 es siempre quien inicia la partida.
 *       j1Izquierda indica si j1 comienza desde la izquierda (true) o la derecha (false).
 */
void configurarPartida(Jugador &j1, Jugador &j2, bool &j1Izquierda) {
    if ((j1.categoria == 'S' && j2.categoria == 'S') ||
        (j1.categoria == 'J' && j2.categoria == 'J')) {
        // Misma categoria: se elige al azar quien inicia y desde donde
        if (rand() % 2 == 0) intercambio(j1, j2);
        j1Izquierda = (rand() % 2 == 0);
    } else {
        // Distinta categoria: el Senior toma las decisiones
        Jugador *senior = (j1.categoria == 'S') ? &j1 : &j2;
        Jugador *junior = (j1.categoria == 'S') ? &j2 : &j1;

        int op;
        cout << senior->nickname << " (Senior) elige quien inicia (1=" << senior->nickname
             << ", 2=" << junior->nickname << "): ";
        cin >> op;
        if (op == 2) intercambio(*senior, *junior);

        // Despues del intercambio, j1 es quien inicia
        cout << j1.nickname << " (quien inicia), elige de donde salir (0=Izquierda, 1=Derecha): ";
        cin >> op;
        j1Izquierda = (op == 0);
    }
}

/*
 * Pre:  Recibe la pila de avance, el jugador, la posicion actual, el estado de inmunidad,
 *       el contador de caidas en numeros maligno, el valor de la baldosa, el estado de racha,
 *       la posicion de inicio, la direccion de avance y la posicion objetivo.
 * Post: Aplica el efecto correspondiente segun la clasificacion del numero de la baldosa:
 *       - Primo (maligno, caso 1): si tiene inmunidad se salva y la pierde; si no, vuelve al inicio,
 *         incrementa el contador de maligno y anula la racha.
 *       - Amigo (caso 2): el jugador obtiene inmunidad para la siguiente baldosa maligna.
 *       - Capicua (caso 3): el jugador retrocede 2 posiciones (si es posible) y se anula la racha.
 *       - Perfecto (caso 4): el jugador avanza 3 posiciones adicionales (sin superar el objetivo).
 *       - Comun (caso 0): no ocurre ningun efecto adicional.
 */
void procesamientoNumero(tope &av1, Jugador &j1, int posActual, bool &inm1,
                         int &mal1, int valorBaldosa, bool &rach,
                         int startPos, int dir, int targetPos) {
    switch (controlNumero(valorBaldosa, posActual)) {
        case 1:
            cout<<"NUMERO PRIMO (maligno)"<<endl;
            if (inm1) {
                cout<<j1.nickname<<" tiene inmunidad, se salva"<<endl;
                inm1 = false;
                break;
            }
            mal1++;
            cout<<"PERDIDA DE RECORRIDO"<<endl;
            cout<<j1.nickname<<" VUELVE A EMPEZAR"<<endl;
            cout<<"Contador de numeros maligno: "<<mal1<<endl;
            destruirPila(av1);
            cargarPila(av1, startPos); // Regresa al punto de partida
            rach = false;
            break;
        case 2:
            cout<<"NUMERO AMIGO"<<endl;
            cout<<j1.nickname<<" obtiene inmunidad"<<endl;
            inm1 = true;
            break;

        case 3: // Numero capicua
            cout << "NUMERO CAPICUA" << endl;
            cout << j1.nickname << " Retrocede 2 baldosas" << endl;
            // Retrocede hasta 2 posiciones sin salirse del inicio
            for (int x = 0; x < 2; x++) {
                if (av1->valor != startPos && av1->abajo != NULL)
                    desapilarPila(av1);
            }
            rach = false;
            break;

        case 4: // Numero perfecto
            cout << "NUMERO PERFECTO" << endl;
            cout << j1.nickname << " avanza 3 baldosas" << endl;
            // Avanza hasta 3 posiciones sin superar el objetivo
            for (int x = 0; x < 3; x++) {
                if (av1->valor == targetPos) break;
                cargarPila(av1, av1->valor + dir);
            }
            break;

        default: // Numero comun
            cout << "NUMERO comun" << endl;
            cout << j1.nickname << " no hace nada.." << endl;
            break;
    }
}

/*
 * Pre:  Recibe el valor de una baldosa (x).
 * Post: Retorna los puntos que otorga dicha baldosa segun su valor:
 *       - Menor a 1500: 10 puntos.
 *       - Entre 1500 y 3499: 20 puntos.
 *       - Entre 3500 y 4999: 25 puntos.
 *       - 5000 o mayor: 30 puntos.
 */
int controlPuntaje(int x) {
    if (x < 1500) return 10;
    if (x < 3500) return 20;
    if (x < 5000) return 25;
    return 30;
}

/*
 * Pre:  Recibe referencias al ganador y al perdedor de la partida,
 *       el puntaje a sumar al ganador (gan), la penalizacion a restar al perdedor (pen)
 *       y el estado de racha del ganador.
 * Post: Actualiza el puntaje, la racha y la categoria de ambos jugadores en el archivo.
 *       Si el ganador gano sin retrocesos (rach == true), se incrementa su racha.
 *       Si la racha llega a 2 victorias, el jugador asciende a categoria Senior y la racha se resetea.
 *       El perdedor siempre pierde su racha y se le descuenta la penalizacion del puntaje.
 *       Los cambios se persisten en el archivo binario "datosJugadores.dat".
 */
void finalPartida(Jugador &ganador, Jugador &perdedor, int gan, int pen, bool &rach) {
    FILE *archivo = fopen("datosJugadores.dat", "rb+");

    if (archivo == NULL) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Actualizar estadisticas del ganador
    ganador.puntaje += gan;
    if (rach) {
        ganador.racha++;
    } else {
        ganador.racha = 0;
    }
    // Si acumulo 2 victorias con racha, asciende a Senior
    if (ganador.racha == 2) {
        ganador.categoria = 'S';
        ganador.racha = 0;
    }

    // Actualizar estadisticas del perdedor
    perdedor.puntaje -= pen;
    perdedor.racha = 0;

    Jugador aux;

    // Buscar y reescribir el registro del ganador en el archivo
    rewind(archivo);
    while (fread(&aux, sizeof(Jugador), 1, archivo)) {
        if (strcmp(aux.nickname, ganador.nickname) == 0) {
            fseek(archivo, -(long)sizeof(Jugador), SEEK_CUR);
            fwrite(&ganador, sizeof(Jugador), 1, archivo);
            break;
        }
    }

    // Buscar y reescribir el registro del perdedor en el archivo
    rewind(archivo);
    while (fread(&aux, sizeof(Jugador), 1, archivo)) {
        if (strcmp(aux.nickname, perdedor.nickname) == 0) {
            fseek(archivo, -(long)sizeof(Jugador), SEEK_CUR);
            fwrite(&perdedor, sizeof(Jugador), 1, archivo);
            break;
        }
    }

    fclose(archivo);
}

/*
 * Pre:  Recibe referencias a dos jugadores.
 * Post: Intercambia los datos de j1 y j2 usando una variable auxiliar.
 */
void intercambio(Jugador &j1, Jugador &j2) {
    Jugador aux = j1;
    j1 = j2;
    j2 = aux;
}

/*
 * Pre:  Recibe dos jugadores seleccionados, sus pilas de avance inicializadas
 *       y un camino generado valido.
 * Post: Ejecuta el bucle principal de la partida.
 *       En cada iteracion, cada jugador tira un dado y avanza sobre el camino.
 *       Se aplican los efectos de las baldosas y se controlan las condiciones de victoria:
 *       - Un jugador llega al extremo opuesto del camino.
 *       - Un jugador cae 3 veces en numeros maligno.
 *       Al finalizar la partida, se llama a finalPartida() para actualizar estadisticas
 *       y se liberan el camino y las pilas de avance.
 */
void juego(Jugador &j1, Jugador &j2, tope &av1, tope &av2, listaD &camino) {
    srand(time(NULL));
    bool j1Izquierda;
    configurarPartida(j1, j2, j1Izquierda);

    int start1, dir1, target1;
    int start2, dir2, target2;

    // Determina la posicion de inicio, direccion y objetivo de cada jugador
    if (j1Izquierda) {
        start1  = 0;                   dir1  =  1; target1 = camino.tamanio - 1;
        start2  = camino.tamanio - 1;  dir2  = -1; target2 = 0;
    } else {
        start1  = camino.tamanio - 1;  dir1  = -1; target1 = 0;
        start2  = 0;                   dir2  =  1; target2 = camino.tamanio - 1;
    }

    // Posiciona a cada jugador en su punto de partida
    cargarPila(av1, start1);
    cargarPila(av2, start2);

    int  mal1 = 0,    mal2 = 0;        // Contador de caidas en numeros maligno
    bool inm1 = false, inm2 = false;   // Estado de inmunidad de cada jugador
    bool rach1 = true, rach2 = true;   // Indica si el jugador no ha retrocedido aun
    int  puntaje1 = 0, puntaje2 = 0;   // Puntaje acumulado en la partida
    int  posActual1, posActual2;        // Valor de la ultima baldosa valida visitada
    bool end = false;

    while (!end) {
        int dado, valorBaldosa;
        bool extraTurn;

        // ============================================================
        // TURNO DEL JUGADOR 1
        // ============================================================

        // La posicion actual es el valor de la penultima baldosa visitada
        posActual1 = (av1->abajo == NULL) ? 0 : consultarNodo(camino, av1->abajo->valor);

        // Lanzamiento del dado
        dado = rand() % 6 + 1;
        cout << "\n=====================================\n";
        cout << j1.nickname << " tira los dados .." << endl;
        _getch(); // Espera que el jugador presione una tecla
        cout << "Dado: " << dado << endl;

        do {
            extraTurn = false;

            // Avanza tantas posiciones como indica el dado (sin superar el objetivo)
            for (int x = 0; x < dado; x++) {
                if (av1->valor == target1) break;
                cargarPila(av1, av1->valor + dir1);
            }

            valorBaldosa = consultarNodo(camino, av1->valor);
            cout << "CASILLA nro " << av1->valor << " = " << valorBaldosa << endl;
            puntaje1 += controlPuntaje(valorBaldosa);

            if (baldosaValida(camino, av1->valor)) {
                // Caso especial: numero primo con inmunidad activa otorga turno extra
                if (controlNumero(valorBaldosa, posActual1) == 1 && inm1) {
                    cout << "NUMERO PRIMO (maligno) con inmunidad! Se salva y tira de nuevo!" << endl;
                    inm1 = false;
                    dado = rand() % 6 + 1;
                    _getch();
                    cout << "Nuevo Dado: " << dado << endl;
                    extraTurn = true;
                    posActual1 = valorBaldosa;
                } else {
                    procesamientoNumero(av1, j1, posActual1, inm1, mal1,
                                        valorBaldosa, rach1, start1, dir1, target1);
                }
            } else {
                // Baldosa vacia: suma puntos de bonificacion
                cout << "Baldosa vacia, suma 10 puntos" << endl;
                puntaje1 += 10;
            }
        } while (extraTurn);

        // Verificar si el jugador 1 llego al extremo opuesto (condicion de victoria)
        if ((dir1 ==  1 && av1->valor >= target1) ||
            (dir1 == -1 && av1->valor <= target1)) {

            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;
            cout << j1.nickname << " CRUZO EL CAMINO" << endl;
            cout << j1.nickname << " GANAA" << endl;
            cout << "----------------------------------------" << endl;

            // Bonus de racha: se duplica el puntaje si no hubo retrocesos
            if (rach1) { cout << "BONUS NO RETROCESOS (x2)\n" << endl; puntaje1 *= 2; }

            int penalizacion = (int)(puntaje2 * 0.05);

            cout << j1.nickname << " PUNTAJE EN PARTIDA: " << puntaje1 << endl;
            cout << j2.nickname << " PUNTAJE EN PARTIDA: " << puntaje2 << endl;
            cout << j1.nickname << " SE LE SUMA: " << puntaje1 << endl;
            cout << j2.nickname << " ES PENALIZADO, PIERDE: " << penalizacion << endl;
            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;

            finalPartida(j1, j2, puntaje1, penalizacion, rach1);
            break;
        }

        // Verificar si el jugador 1 cayo 3 veces en numeros maligno (condicion de derrota)
        if (mal1 == 3) {
            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;
            cout << j1.nickname << " CAYO 3 VECES EN UN NUMERO MALIGNO" << endl;
            cout << j2.nickname << " GANAA" << endl;
            cout << "----------------------------------------" << endl;

            if (rach2) { cout << "BONUS NO RETROCESOS (x2)\n" << endl; puntaje2 *= 2; }

            int penalizacion = (int)(puntaje1 * 0.05);

            cout << j1.nickname << " PUNTAJE EN PARTIDA: " << puntaje1 << endl;
            cout << j2.nickname << " PUNTAJE EN PARTIDA: " << puntaje2 << endl;
            cout << j2.nickname << " SE LE SUMA: " << puntaje2 << endl;
            cout << j1.nickname << " ES PENALIZADO, PIERDE: " << penalizacion << endl;
            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;

            finalPartida(j2, j1, puntaje2, penalizacion, rach2);
            break;
        }

        // ============================================================
        // TURNO DEL JUGADOR 2
        // ============================================================

        posActual2 = (av2->abajo == NULL) ? 0 : consultarNodo(camino, av2->abajo->valor);

        dado = rand() % 6 + 1;
        cout << "\n=====================================\n";
        cout << j2.nickname << " tira los dados .." << endl;
        _getch();
        cout << "Dado: " << dado << endl;

        do {
            extraTurn = false;

            for (int x = 0; x < dado; x++) {
                if (av2->valor == target2) break;
                cargarPila(av2, av2->valor + dir2);
            }

            valorBaldosa = consultarNodo(camino, av2->valor);
            cout << "CASILLA nro " << av2->valor << " = " << valorBaldosa << endl;
            puntaje2 += controlPuntaje(valorBaldosa);

            if (baldosaValida(camino, av2->valor)) {
                if (controlNumero(valorBaldosa, posActual2) == 1 && inm2) {
                    cout << "NUMERO PRIMO (maligno) con inmunidad! Se salva y tira de nuevo!" << endl;
                    inm2 = false;
                    dado = rand() % 6 + 1;
                    _getch();
                    cout << "Nuevo Dado: " << dado << endl;
                    extraTurn = true;
                    posActual2 = valorBaldosa;
                } else {
                    procesamientoNumero(av2, j2, posActual2, inm2, mal2,
                                        valorBaldosa, rach2, start2, dir2, target2);
                }
            } else {
                cout << "Baldosa vacia, suma 10 puntos" << endl;
                puntaje2 += 10;
            }
        } while (extraTurn);

        // Verificar si el jugador 2 llego al extremo opuesto (condicion de victoria)
        if ((dir2 ==  1 && av2->valor >= target2) ||
            (dir2 == -1 && av2->valor <= target2)) {

            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;
            cout << j2.nickname << " CRUZO EL CAMINO" << endl;
            cout << j2.nickname << " GANAA" << endl;
            cout << "----------------------------------------" << endl;

            if (rach2) { cout << "BONUS NO RETROCESOS (x2)" << endl; puntaje2 *= 2; }

            int penalizacion = (int)(puntaje1 * 0.05);

            cout << j1.nickname << " PUNTAJE EN PARTIDA: " << puntaje1 << endl;
            cout << j2.nickname << " PUNTAJE EN PARTIDA: " << puntaje2 << endl;
            cout << j2.nickname << " SE LE SUMA: " << puntaje2 << endl;
            cout << j1.nickname << " ES PENALIZADO, PIERDE: " << penalizacion << endl;
            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;

            finalPartida(j2, j1, puntaje2, penalizacion, rach2);
            break;
        }

        // Verificar si el jugador 2 cayo 3 veces en numeros maligno (condicion de derrota)
        if (mal2 == 3) {
            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;
            cout << j2.nickname << " CAYO 3 VECES EN UN NUMERO MALIGNO" << endl;
            cout << j1.nickname << " GANAA" << endl;
            cout << "----------------------------------------" << endl;

            if (rach1) { cout << "BONUS NO RETROCESOS (x2)" << endl; puntaje1 *= 2; }

            int penalizacion = (int)(puntaje2 * 0.05);

            cout << j1.nickname << " PUNTAJE EN PARTIDA: " << puntaje1 << endl;
            cout << j2.nickname << " PUNTAJE EN PARTIDA: " << puntaje2 << endl;
            cout << j1.nickname << " SE LE SUMA: " << puntaje1 << endl;
            cout << j2.nickname << " ES PENALIZADO, PIERDE: " << penalizacion << endl;
            cout << "\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;

            finalPartida(j1, j2, puntaje1, penalizacion, rach1);
            break;
        }
    }

    // Liberar memoria de las estructuras dinamicas utilizadas
    destruirCamino(camino);
    destruirPila(av1);
    destruirPila(av2);
}

/*
 * Pre:  -
 * Post: Muestra el submenu de juego con opciones para seleccionar jugadores,
 *       generar el camino e iniciar la partida.
 *       Verifica que existan al menos 2 jugadores antes de continuar.
 *       El camino generado solo es visible si se ingresa la clave de administrador.
 *       El juego no puede iniciarse sin haber seleccionado jugadores y generado el camino.
 */