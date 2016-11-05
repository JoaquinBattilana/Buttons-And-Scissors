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

int esMovimientoValido(matriz_t tablero, movimiento_t puntos, punto_t dir, int (*cmp)(movimiento_t, char, char));

int condMovimientoTurno(movimiento_t puntos, char boton, char botonLeido);

int condMovimientoJugador(movimiento_t puntos, char boton, char botonLeido);

int realizarCorte(matriz_t * tablero, movimiento_t mov, punto_t dir);

void calcularDireccion(movimiento_t mov, punto_t * direccion);

int hayMovimientosValidos(matriz_t tablero);

int realizarCortePc(matriz_t * tablero);

int condMaxMov(int cantBotones, char boton, char botonPosActual);

int condMinMov(int cantBotones, char boton, char botonPosActual);

#endif