#ifndef _BUTTONS_AND_SCISSORS_BACK

#define _BUTTONS_AND_SCISSORS_BACK

#include "stdlib.h"
#define INC_MAX 4
#define VACIO ' '
#define MIN_MOV 2
#define BLOQUE_MEM 10

enum errores {SIN_ERROR=0, E_MEM_DIN,E_ARCHIVO_MATRICES,E_ABRIR_ARCHIVO,
    E_CREAR_ARCHIVO, E_ARCHIVO_MAL,COMANDO_INVALIDO,DIR_INVALIDA,ENTRE_BOTONES,MISMO_BOT,FUERA_MATRIZ_1,FUERA_MATRIZ_2,NO_PRINT};

typedef struct
{
    char ** v;
    size_t n;
} matriz_t;

typedef struct
{
    int x;
    int y;
} punto_t;

typedef struct
{
    punto_t origen;
    punto_t destino;
    int cantBotones;
} movimiento_t;

/*
** Dado un tablero, un movimiento anteriormente validado y la direccion de dicho movimiento,
** realiza el correspondiente corte desde el punto de origen del movimiento hacia el
** punto de origen, dejando los casilleros que recorre vacios.
** Retorna la cantidad de botones cortados.
*/
int realizarCorte(matriz_t * tablero, movimiento_t movimiento, punto_t direccion);

/*
** Dado un movimiento anteriormente validado, se calcula la direccion que debe utilizarse
** para ir desde el punto de origen hacia el punto de destino.
** Dicha direccion es retornada a traves del parametro de salida 'direccion'.
*/
void calcularDireccion(movimiento_t movimiento, punto_t * direccion);

/*
** Dado un tablero, retorna 0 si no quedan movimientos validos por realizar
** y retorna distinto de 0 si aun queda alguno.
*/
int hayMovimientosValidos(matriz_t tablero);

/*
** Dado un tablero y habiendo validado que aun hayan movimientos por realizar,
** esta funcion realiza un corte, eligiendo entre los de maxima cantidad
** de botones o los de minima cantidad de botones pseudoaleatoriamente.
** Incrementa los botones cortados por la PC en un parametro de salida.
** Retorna 0 si no hubo error, o distinto si los hubo.
*/
int realizarCortePc(matriz_t * tablero, int * btnsPC);

/*
** Dado un tablero, un movimiento y una direccion,
** retorna 0 si no hay botones de distinto color
** entre el putno de origen y el punto de destino
** o distinto de 0 si los hay.
*/
int hayBtnsEntreMedio(matriz_t tablero, movimiento_t movimiento, punto_t direccion);

/*
** Dado un movimiento y un tablero, retorna 0 si el movimiento es valido
** y distinto de 0 si no lo es.
*/
int validarMovimiento(movimiento_t * movimiento, matriz_t tablero);

#endif
