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

typedef struct tipoJuego{
    matriz_t tablero;
    int modoJuego;
    int turno;
} tipoJuego;


int menu();
size_t validar_dim();
int matrizDsdArchivo(tipoJuego * juego);
void imprimirTablero(matriz_t tablero);
int cargarJuego(tipoJuego * juego);
int leer_movimiento(movimiento_t * mov, tipoJuego * juego);
void printError(int error);
int guardarJuego(char * nombreArchivo, tipoJuego * juego);
int validar_volvermenu(void);
int jugar(tipoJuego * juego, jugador jugadores[JUGADORES]);

#endif
