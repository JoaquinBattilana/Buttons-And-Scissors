#ifndef TPE_BUTTONSANDSCISSORSFRONT_H

#define TPE_BUTTONSANDSCISSORSFRONT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random.h"
#include <ctype.h>
#include "buttonsAndScissorsBack.h"

enum opciones {JVSJ=1, JVSCPU, CARGAR, SALIR};
enum ia {NOPC=0, PC};

#define SAVEGAME 10
#define JUGADORES 2
#define BLOQUE 10
#define LIMPIAR_BUFFER() while(getchar()!='\n')

typedef struct{
    int botones_cortados;
    int esPC;
}jugador;

/*Esta estructura es el juego en si, que contiene el tablero, el modo de juego(0 para jugar contra otro jugador, 1 para jugar 
** contra la pc) y el turno actual del jugador)
*/

typedef struct tipoJuego{
    matriz_t tablero;
    int modoJuego;
    int turno;
} tipoJuego;

/*
** Imprime el menu y solicita al usuario que eliga una opcion. Tambien se valida lo 
** que el usuario ingrese, de ser incorrecto vuelve a solicitar una opcion. Retorna
** la opcion elegida
*/
int menu();

/*
** Soliciata al usuario una dimension para el tablero y valida que sea correcta.
** Retorna la dimension escogida.
*/
size_t validar_dim();

/*Se le pasa la estructura del juego y devuelve cargado la matriz. La matriz se carga desde un archivo de texto plano
** con el nombre de la dimension siguiendo el formato de la catedra
** . Devuelve error si el archivo no existe o tiene otro formato.
*/
int matrizDsdArchivo(tipoJuego * juego);

/*
** Imprime la cantidad de botones cortados por jugador y el tablero.
*/
void imprimirTablero(matriz_t tablero, jugador * jugadores);

/*Carga un juego guardado en la estructura juego para poder seguir jugando, devuelve error si el archivo no esta con 
** el formato de la catedra o no existe.
*/

int cargarJuego(tipoJuego * juego);

/*
** Le solicita al usuario un comando y lo valida. Si es un corte modifica la estructura mov.
** Si es quit retorna 1 sino siempre retorna 0. Si el usuario ingresa savegame guarda la partida.
*/
int leer_movimiento(movimiento_t * mov, tipoJuego * juego);

/*
** Dado un numero imprime el correspondiente error.
*/
void printError(int error);
/* Guarda el juego en un archivo con el nombre del vector nombreArchivo. El guardado se hace con el formato de la catedra
**, si no pudo guardar devuelve error
*/

int guardarJuego(char * nombreArchivo, tipoJuego * juego);

/*
** Le pregunta al usuario si desea volver al menu (retorna 2) o salir (retorna 1) del juego.
*/
int validar_volvermenu(void);

/*
** Dado un juego y vector de jugadores gestiona los turnos del juego. Y al final de cada turno
** se fija hay un ganador. Le pregunta al usuario si desea volver al menu (retorna 0) o salir (retorna 1).
*/
int jugar(tipoJuego * juego, jugador jugadores[JUGADORES]);

/*
** Imprime botones y tijeras en asii.
*/
void imprimirBanner();

#endif
