/*
 * jugadores.hpp
 * Modulo: Gestion de Jugadores
 * Descripcion: Contiene la estructura Jugador y todas las operaciones relacionadas
 *              con el registro, almacenamiento y visualizacion de jugadores.
 *              Los datos se persisten en el archivo binario "datosJugadores.dat".
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
using namespace std;

/*
 * Estructura que representa a un jugador del sistema.
 * - nombre, apellido, nickname: datos de identificacion del jugador.
 * - puntaje: acumulado a lo largo de las partidas.
 * - categoria: 'J' para Junior, 'S' para Senior.
 * - racha: contador de victorias consecutivas sin retrocesos.
 */
struct Jugador {
    char nombre[30];
    char apellido[30];
    char nickname[30];
    int puntaje;
    char categoria;
    int racha;
};

/*
 * Pre:  Recibe un Jugador con todos sus campos cargados.
 * Post: Verifica que el nickname no este duplicado en el archivo.
 *       Si no existe, agrega el jugador al final del archivo binario.
 *       Si ya existe un jugador con ese nickname, muestra un mensaje de error.
 */
void agregarJugador(Jugador j) {
    FILE* archivoLect = fopen("datosJugadores.dat", "rb");
    if (archivoLect != NULL) {
        Jugador t;
        // Recorre el archivo buscando un nickname duplicado
        while (fread(&t, sizeof(Jugador), 1, archivoLect) == 1) {
            if (strcmp(t.nickname, j.nickname) == 0) {
                cout << "--ERRROR: JUGADOR YA EXISTENTE" << endl;
                fclose(archivoLect);
                return;
            }
        }
        fclose(archivoLect);
    }

    // Si el nickname es unico, escribe el jugador al final del archivo
    FILE* archivo = fopen("datosJugadores.dat", "ab");
    fwrite(&j, sizeof(Jugador), 1, archivo);
    cout << "$ JUGADOR - " << j.nickname << " - CARGADO CON EXITO $" << endl;
    fclose(archivo);
}

/*
 * Pre:  -
 * Post: Solicita al usuario nombre, apellido y nickname por teclado.
 *       Crea un Jugador con categoria 'J' (Junior), puntaje 0 y racha 0,
 *       y lo registra en el archivo mediante agregarJugador().
 */
void cargaJugador() {
    Jugador J;
    cout << "======= CARGA DE JUGADOR ========" << endl;

    // Se repite la entrada hasta que el campo no este vacio
    do {
        cout << "\nNombre: ";
        cin >> J.nombre;
    } while (J.nombre[0] == '\0');

    do {
        cout << "\nApellido: ";
        cin >> J.apellido;
    } while (J.apellido[0] == '\0');

    do {
        cout << "\nNickname: ";
        cin >> J.nickname;
    } while (J.nickname[0] == '\0');

    // Todo jugador nuevo comienza como Junior con puntaje y racha en cero
    J.categoria = 'J';
    J.puntaje   = 0;
    J.racha     = 0;

    agregarJugador(J);
}

/*
 * Pre:  -
 * Post: Crea cuatro jugadores con datos predefinidos y los registra en el archivo.
 *       Se utiliza cuando el archivo de jugadores no existe al iniciar el programa.
 */
void inicializarDefecto() {
    Jugador j1 = {"CARLOS",  "GARCIA", "charlySNM",     0, 'S', 0};
    Jugador j2 = {"CHIZZO",  "NAPOLI", "fabianStone67", 0, 'S', 0};
    Jugador j3 = {"OSCAR",   "MORO",   "elBata",        0, 'J', 0};
    Jugador j4 = {"FABIANA", "CANTILO","losTwist",      0, 'J', 0};

    agregarJugador(j1);
    agregarJugador(j2);
    agregarJugador(j3);
    agregarJugador(j4);

    cout << "-- JUGADORES CREADOS POR DEFECTO --" << endl;
}

/*
 * Pre:  -
 * Post: Abre el archivo de jugadores y muestra por pantalla
 *       los datos de todos los jugadores registrados.
 *       Si el archivo no existe o esta vacio, muestra un mensaje indicandolo.
 */
void mostrarJugadores() {
    FILE* archivo = fopen("datosJugadores.dat", "rb");

    if (archivo != NULL) {
        fseek(archivo, 0, SEEK_END);
        if (ftell(archivo) == 0) {
            cout << "--- SIN JUGADORES ---" << endl;
            fclose(archivo);
            return;
        }
        rewind(archivo);

        Jugador t;
        cout << "==== JUGADORES REGISTRADOS ====\n" << endl;

        while (fread(&t, sizeof(Jugador), 1, archivo) == 1) {
            cout << "NOMBRE: "    << t.nombre    << endl;
            cout << "APELLIDO: "  << t.apellido  << endl;
            cout << "NICKNAME: "  << t.nickname  << endl;
            cout << "CATEGORIA: " << t.categoria << endl;
            cout << "PUNTAJE: "   << t.puntaje   << endl;
            cout << endl;
        }
        fclose(archivo);
    } else {
        cout << "-- ERROR: ARCHIVO -- " << endl;
    }
}

/*
 * Pre:  -
 * Post: Cuenta y retorna la cantidad de jugadores almacenados en el archivo.
 *       Si el archivo no existe, retorna 0.
 */
int cantidadDeJugadores() {
    FILE* archivo = fopen("datosJugadores.dat", "rb");

    if (archivo != NULL) {
        Jugador t;
        int contador = 0;
        while (fread(&t, sizeof(Jugador), 1, archivo) == 1)
            contador++;
        fclose(archivo);
        return contador;
    } else {
        return 0;
    }
}
