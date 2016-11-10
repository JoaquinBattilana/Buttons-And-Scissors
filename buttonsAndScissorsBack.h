#ifndef _BUTTONS_AND_SCISSORS_BACK

#define _BUTTONS_AND_SCISSORS_BACK

#define INC_MAX 4
#define VACIO ' '
#define MIN_MOV 2

typedef struct
{
    char ** v;
    int n;
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

int realizarCorte(matriz_t * tablero, movimiento_t mov, punto_t dir);

void calcularDireccion(movimiento_t mov, punto_t * direccion);

int hayMovimientosValidos(matriz_t tablero);

int realizarCortePc(matriz_t * tablero);

int hayBtnsEntreMedio(matriz_t tablero, movimiento_t puntos, punto_t dir)

#endif