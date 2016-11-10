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

/*
 * Dado un tablero, un movimiento anteriormente validado y la direccion de dicho movimiento,
 * realiza el correspondiente corte desde el punto de origen del movimiento hacia el
 * punto de origen, dejando los casilleros que recorre vacios.
 * Retorna la cantidad de botones cortados.
 */
int realizarCorte(matriz_t * tablero, movimiento_t mov, punto_t dir);

/*
 * Dado un movimiento anteriormente validado, se calcula la direccion que debe utilizarse
 * para ir desde el punto de origen hacia el punto de destino.
 * Dicha direccion es retornada a traves del parametro de salida 'direccion'.
 */
void calcularDireccion(movimiento_t mov, punto_t * direccion);

/*
 * Dado un tablero, retorna 0 si no quedan movimientos validos por realizar
 * y retorna distinto de 0 si aun queda alguno.
 */
int hayMovimientosValidos(matriz_t tablero);

/*
 * Dado un tablero y habiendo validado que aun hayan movimientos por realizar,
 * esta funcion realiza un corte, eligiendo entre los de maxima cantidad
 * de botones o los de minima cantidad de botones pseudoaleatoriamente.
 * Retorna la cantidad de botones cortados.
 */
int realizarCortePc(matriz_t * tablero);

/*
 * Dado un tablero, un movimiento y una direccion,
 * retorna 0 si no hay botones de distinto color
 * entre el putno de origen y el punto de destino
 * o distinto de 0 si los hay.
 */
int hayBtnsEntreMedio(matriz_t tablero, movimiento_t puntos, punto_t dir)

#endif