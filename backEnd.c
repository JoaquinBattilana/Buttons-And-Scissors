#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define INC_MAX 4
#define VACIO 0

#define MAX(a, b) a>b? a : b
#define MIN(a, b) a<b? a : b

//enum errores {ERROR=0, E_MEM_DIN, E_FORMATO,E_};
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

char dir_inc[INC_MAX][2] = {{0,1},{1,1},{1,0},{1,-1}}; //incremento direcciones DERECHA, D_ABAJO, ABAJO, I_ABAJO

int hayMovimientosValidos(matriz_t tablero) //chequea luego de cada turno para saber si hay un ganador
{
    int flag = 0;
    char c;
    int i=0, j=0;
    while(i<tablero.n && j<tablero.n && !flag)
    {
        if((c = tablero.v[i][j]) != VACIO)
        {
            punto_t pos = {i, j};
            flag = buscarBoton(tablero, pos, c);
        }

        if(++j == tablero.n)
        {
            i++;
            j=0;
        }
    }

    return flag;
}

int buscarBoton(matriz_t tablero, punto_t pos, char boton)
{
    int flag = 0;
    int i;

    for(i=0; i<INC_MAX && !flag; i++)
    {
        punto_t direccion = {dir_inc[i][0], dir_inc[i][1]};
        punto_t punto1 = {0,0};
        punto_t punto2 = {tablero.n-1,tablero.n-1};
        flag = esMovimientoValido(tablero, pos, punto1, punto2, direccion, boton);
    }

    return flag;
}

int esMovimientoValido(matriz_t tablero, punto_t pos, punto_t p1, punto_t p2, punto_t dir, char boton)
{
    int flag = 0;
    int i,j;
    char c;
    int minFil, maxFil, minCol, maxCol;
    puntoMaxMin(p1, p2, &minFil, &maxFil, &minCol, &maxCol);

    for(i=pos.x, j=pos.y; i>=minFil && i<=maxFil && j>=minCol && j<=maxCol && !flag; i+=dir.x, j+=dir.y)
    {
        if((c = tablero.v[i][j]) != VACIO && c != boton)
            flag = 2; //ya que debe salir del ciclo for pero luego retornar 0
        else if(c == boton)
            flag = 1;
    }

    return flag % 2; //para que en caso de el flag ser 2, retorne 0
}

punto_t calcularDireccion(punto_t origen, punto_t destino)
{
    punto_t direccion = {destino.x - origen.x, destino.y - origen.y};
    if(direccion.x != 0)
        direccion.x = direccion.x / abs(direccion.x);
    if(direccion.y != 0)
        direccion.y = direccion.y / abs(direccion.y);

    return direccion;
}

int realizarCorte(matriz_t tablero, punto_t origen, punto_t destino, punto_t dir)
{
    int i,j;
    int botonesCortados = 0;

    for(i=origen.x, j=origen.y; i != destino.x && j != destino.y; i+=dir.x, j+=dir.y)
    {
        if(tablero.v[i][j] != VACIO)
        {
            tablero.v[i][j] = VACIO;
            botonesCortados++;
        }
    }

    tablero.v[i][j] = VACIO;
    botonesCortados++;

    return botonesCortados;
}

void puntoMaxMin(punto_t p1, punto_t p2, int * minFil, int * maxFil, int * minCol, int * maxCol)
{
    *minFil = MIN(p1.x, p2.x);
    *maxFil = MAX(p2.x, p2.x);
    *minCol = MIN(p1.y, p2.y);
    *maxCol = MAX(p2.y, p2.y);
    return;
}
