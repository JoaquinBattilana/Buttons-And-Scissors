#include <stdlib.h>
#include <stdio.h>
#include "random.h"

#define INC_MAX 4
#define VACIO 0
#define MIN_MOV 2

#define MAX(a, b) a>b? a : b
#define MIN(a, b) a<b? a : b

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

char dir_inc[INC_MAX][2] = {{0,1},{1,1},{1,0},{1,-1}}; //incremento direcciones DERECHA, D_ABAJO, ABAJO, I_ABAJO

int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, char boton, int (*cond)(int), size_t dim, movimiento_t * mov_vec);
void limitesSubMatriz(punto_t p1, punto_t p2, int * minFil, int * maxFil, int * minCol, int * maxCol);
int esMovimientoValido(matriz_t tablero, punto_t pos, movimiento_t mov, punto_t dir, char boton);
movimiento_t * sobreescribir(movimiento_t * mov_vec, movimiento_t mov, size_t * dim);
movimiento_t * agregar(movimiento_t * mov_vec, movimiento_t mov, size_t * dim);
int realizarCorte(matriz_t * tablero, movimiento_t mov, punto_t dir);
void calcularDireccion(movimiento_t mov, punto_t * direccion);
int buscarBoton(matriz_t tablero, punto_t pos, char boton);
void calcularMovPc(matriz_t tablero, movimiento_t * mov);
int hayMovimientosValidos(matriz_t tablero);
int realizarCortePc(matriz_t * tablero);
void imprimirTablero(matriz_t tablero);
int condMaxMov(int cantBotones);
int condMinMov(int cantBotones);

void imprimirTablero(matriz_t tablero)
{
    printf("\n   ");

    int i, j;

    for(i = 0; i < tablero.n; i++) //referencia numerica para las columnas
        printf(" %d", i);

    i=0;

    printf("\n");

    while(i < tablero.n)
    {
        printf("\n");

        for(j = 0; j < tablero.n; j++)
        {
            if(j==0)
                printf("%d  ", i); //referencia numerica para las filas

            printf(" %c", tablero.v[i][j]);
        }

        if(j == tablero.n)
        {
            i++;
        }
    }

    printf("\n\n");
    return;
}


int hayMovimientosValidos(matriz_t tablero) //invocar luego de cada turno para saber si hay un ganador
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
        punto_t limiteInferior = {0,0};
        punto_t limiteSuperior = {tablero.n-1,tablero.n-1};
        movimiento_t mov = {limiteInferior, limiteSuperior};
        flag = esMovimientoValido(tablero, pos, mov, direccion, boton);
    }

    return flag;
}

int esMovimientoValido(matriz_t tablero, punto_t pos, movimiento_t mov, punto_t dir, char boton)
{
    int i,j, minFil, maxFil, minCol, maxCol, flag = 0;
    char c;

    limitesSubMatriz(mov.origen, mov.destino, &minFil, &maxFil, &minCol, &maxCol);

    for(i=pos.x, j=pos.y; i>=minFil && i<=maxFil && j>=minCol && j<=maxCol && !flag; i+=dir.x, j+=dir.y)
    {
        if((c = tablero.v[i][j]) != VACIO && c != boton)
            flag = 2; //ya que debe salir del ciclo for pero luego retornar 0
        else if(c == boton)
            flag = 1;
    }

    return flag % 2; //para que en caso de el flag sea igual a 2, retorne 0
}

void calcularDireccion(movimiento_t mov, punto_t * direccion)
{
    direccion->x = mov.destino.x - mov.origen.x;
    direccion->y = mov.destino.y - mov.origen.y;

    if(direccion->x != 0)
        direccion->x = direccion->x / abs(direccion->x);
    if(direccion->y != 0)
        direccion->y = direccion->y / abs(direccion->y);

    return;
}

int realizarCorte(matriz_t * tablero, movimiento_t mov, punto_t dir)
{
    int i, j, botonesCortados = 0;

    for(i=mov.origen.x, j=mov.origen.y; i != mov.destino.x && j != mov.destino.y; i+=dir.x, j+=dir.y)
    {
        if(tablero->v[i][j] != VACIO)
        {
            tablero->v[i][j] = VACIO;
            botonesCortados++;
        }
    }

    tablero->v[i][j] = VACIO;
    botonesCortados++;

    return botonesCortados;
}

void limitesSubMatriz(punto_t p1, punto_t p2, int * minFil, int * maxFil, int * minCol, int * maxCol)
{
    *minFil = MIN(p1.x, p2.x);
    *maxFil = MAX(p1.x, p2.x);
    *minCol = MIN(p1.y, p2.y);
    *maxCol = MAX(p1.y, p2.y);

    return;
}

int realizarCortePc(matriz_t * tablero)
{
    movimiento_t mov;
    calcularMovPc(*tablero, &mov);
    punto_t direccion;
    calcularDireccion(mov, &direccion);
    mov.cantBotones = realizarCorte(tablero, mov, direccion);

    return mov.cantBotones;
}

void calcularMovPc(matriz_t tablero, movimiento_t * mov)
{
    size_t dim = 0;
    movimiento_t * mov_vec = NULL;
    int i=0, j=0;
    char c;
    int estrategia = randInt(0, 1);

    while(i<tablero.n && j<tablero.n)
    {
        if((c = tablero.v[i][j]) != VACIO)
        {
            int k;
            for(k=0; k<INC_MAX; k++)
            {
                punto_t direccion = {dir_inc[i][0], dir_inc[i][1]};
                punto_t posActual = {i,j};
                if(estrategia == 0) //Movimiento Minimo
                    dim = calcularMovPcEnDir(tablero, posActual, direccion, c, condMinMov, dim, mov_vec);
                else //Movimiento Maximo
                    dim = calcularMovPcEnDir(tablero, posActual, direccion, c, condMaxMov, dim, mov_vec);

            }

        }

        if(++j == tablero.n)
        {
            i++;
            j=0;
        }
    }

    int indice = randInt(0, dim-1);

    *mov = mov_vec[indice];

    return;
}


int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, char boton, int (*cond)(int), size_t dim, movimiento_t * mov_vec)
{
    int i,j, cantBtns = 0;
    char c;

    for(i=pos.x, j=pos.y; ((c = tablero.v[i][j]) == boton || c == VACIO) && (*cond)(cantBtns); i+=dir.x, j+=dir.y)
    {
            cantBtns++;
            if(cantBtns >= MIN_MOV)
            {
                if((dim!=0 && mov_vec[0].cantBotones <= cantBtns) || dim==0)
                {
                    if(dim == 0)
                    {
                        movimiento_t mov = {pos, {i,j}, cantBtns};
                        sobreescribir(mov_vec, mov, &dim);
                    }

                    else if(mov_vec[0].cantBotones < cantBtns)
                    {
                        movimiento_t mov = {pos, {i,j}, cantBtns};
                        sobreescribir(mov_vec, mov, &dim);
                    }
                    else if(mov_vec[0].cantBotones == cantBtns)
                    {
                        movimiento_t mov = {pos, {i,j}, cantBtns};
                        agregar(mov_vec, mov, &dim);
                    }
                }
            }
    }

    return dim;
}

int condMinMov(int cantBotones)
{
    int continuarCiclo = 0;
    if(cantBotones < MIN_MOV)
        continuarCiclo = 1; //continua solo si el movimiento no alcanzo la cantidad de botones del mov minimo

    return continuarCiclo;
}

int condMaxMov(int cantBotones)
{
    return 1; //para que no afecte la condicion del ciclo for que lo invoca
}

movimiento_t * sobreescribir(movimiento_t * mov_vec, movimiento_t mov, size_t * dim)
{
    *dim = 1;
    mov_vec = realloc(mov_vec, *dim * sizeof(*mov_vec)); //OPTIMISTA

    mov_vec[0] = mov;

    return  mov_vec;
}

movimiento_t * agregar(movimiento_t * mov_vec, movimiento_t mov, size_t * dim)
{
    (*dim)++;
    mov_vec = realloc(mov_vec, *dim * sizeof(*mov_vec)); //OPTIMISTA

    mov_vec[*dim - 1] = mov;

    return mov_vec;
}
