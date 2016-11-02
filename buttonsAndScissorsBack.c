#include <stdlib.h>
#include <stdio.h>
#include "random.h"
#include "buttonsAndScissorsBack.h"

static int buscarBoton(matriz_t tablero, punto_t pos, char boton);
static void calcularMovPc(matriz_t tablero, movimiento_t * mov);
static movimiento_t * sobreescribir(movimiento_t * mov_vec, movimiento_t mov, size_t * dim);
static movimiento_t * agregar(movimiento_t * mov_vec, movimiento_t mov, size_t * dim);
static int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, char boton, int (*cond)(int,char,char), size_t dim, movimiento_t * mov_vec);
static void limitesSubMatriz(punto_t p1, punto_t p2, int * minFil, int * maxFil, int * minCol, int * maxCol);

char dir_inc[INC_MAX][2] = {{0,1},{1,1},{1,0},{1,-1}}; //incremento direcciones DERECHA, D_ABAJO, ABAJO, I_ABAJO

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

static int buscarBoton(matriz_t tablero, punto_t pos, char boton)
{
    int flag = 0;
    int i;

    for(i=0; i<INC_MAX && !flag; i++) //para cada una de las cuatro direcciones...
    {
        punto_t direccion = {dir_inc[i][0], dir_inc[i][1]};
        punto_t limiteSuperior = {tablero.n-1,tablero.n-1};
        movimiento_t puntos = {pos, limiteSuperior};
        flag = esMovimientoValido(tablero, puntos, direccion, boton, condMovimientoTurno);
    }

    return flag;
}

int esMovimientoValido(matriz_t tablero, movimiento_t puntos, punto_t dir, char boton, int (*cmp)(movimiento_t, char, char))
{
    int i,j, minFil, maxFil, minCol, maxCol, flag = 0;
    char c;

    limitesSubMatriz(puntos.origen, puntos.destino, &minFil, &maxFil, &minCol, &maxCol);

    for(i=puntos.origen.x, j=puntos.origen.y; i>=minFil && i<=maxFil && j>=minCol && j<=maxCol && !flag; i+=dir.x, j+=dir.y)
    {
        if((c = tablero.v[i][j]) != VACIO && c != boton)
            flag = 2; //ya que debe salir del ciclo for pero luego retornar 0
        else if(cmp({{i,j}, puntos.destino}, boton, c))
            flag = 1;
    }

    return flag % 2; //para que en caso de el flag sea igual a 2, retorne 0
}

int condMovimientoTurno(movimiento_t puntos, char boton, char botonLeido)
{
    return (boton == botonLeido);
}

int condMovimientoJugador(movimiento_t puntos, char boton, char botonLeido)
{
    int valorRetorno = 0;
    if(boton == botonLeido)
        if(puntos.origen.x == puntos.destino.x && puntos.origen.y == puntos.destino.y)
            valorRetorno = 1;

    return valorRetorno;
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

static void limitesSubMatriz(punto_t p1, punto_t p2, int * minFil, int * maxFil, int * minCol, int * maxCol)
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

static void calcularMovPc(matriz_t tablero, movimiento_t * mov)
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


static int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, char boton, int (*cond)(int,char,char), size_t dim, movimiento_t * mov_vec)
{
    int i,j, cantBtns = 0;
    char c;

    for(i=pos.x, j=pos.y; (*cond)(cantBtns, tablero.v[i][j], boton); i+=dir.x, j+=dir.y)
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

int condMinMov(int cantBotones, char boton, char botonPosActual)
{
    int continuarCiclo = 0;
    if((botonPosActual == boton || botonPosActual == VACIO) && cantBotones < MIN_MOV)
        continuarCiclo = 1;

    return continuarCiclo;
}

int condMaxMov(int cantBotones, char boton, char botonPosActual)
{
    int continuarCiclo = 0;
    if(botonPosActual == boton || botonPosActual == VACIO)
        continuarCiclo = 1;

    return continuarCiclo;
}

static movimiento_t * sobreescribir(movimiento_t * mov_vec, movimiento_t mov, size_t * dim)
{
    *dim = 1;
    mov_vec = realloc(mov_vec, *dim * sizeof(*mov_vec)); //OPTIMISTA

    mov_vec[0] = mov;

    return  mov_vec;
}

static movimiento_t * agregar(movimiento_t * mov_vec, movimiento_t mov, size_t * dim)
{
    (*dim)++;
    mov_vec = realloc(mov_vec, *dim * sizeof(*mov_vec)); //OPTIMISTA

    mov_vec[*dim - 1] = mov;

    return mov_vec;
}
