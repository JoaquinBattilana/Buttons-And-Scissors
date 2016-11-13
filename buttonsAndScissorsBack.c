#include <stdlib.h>
#include <stdio.h>
#include "random.h"
#include "buttonsAndScissorsBack.h"

static int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, int (*cond)(int,char,char), size_t * dim, movimiento_t ** pmov_vec);
static int buscarBoton(matriz_t tablero, punto_t pos);
static int calcularMovPc(matriz_t tablero, movimiento_t * mov);
static int esMovimientoValido(matriz_t tablero, movimiento_t puntos, punto_t dir, int (*cmp)(movimiento_t, char, char));
static int agregarMovimiento(movimiento_t ** pmov_vec, movimiento_t mov, size_t * dim);
static int condMovimientoTurno(movimiento_t puntos, char boton, char botonLeido);
static int condMovimientoJugador(movimiento_t puntos, char boton, char botonLeido);
static int condMaxMov(int cantBotones, char boton, char botonPosActual);
static int condMinMov(int cantBotones, char boton, char botonPosActual);

static char dir_inc[INC_MAX][2] = {{0,1},{1,1},{1,0},{1,-1}}; //incremento direcciones DERECHA, D_ABAJO, ABAJO, I_ABAJO

int hayBtnsEntreMedio(matriz_t tablero, movimiento_t puntos, punto_t dir)
{ 
	return !esMovimientoValido(tablero, puntos, dir, condMovimientoJugador);
}

int validarMovimiento(movimiento_t * mov, matriz_t tablero)
{
    double pendiente;
    int flag = SIN_ERROR;
    if(mov->destino.x >= tablero.n || mov->destino.y >= tablero.n || mov->destino.x < 0 || mov->destino.y < 0) //fuera de matriz
        flag = FUERA_MATRIZ_2;
    else if (mov->origen.x >= tablero.n || mov->origen.y >= tablero.n || mov->origen.x < 0 || mov->origen.y < 0) //fuera de matriz
        flag = FUERA_MATRIZ_1;
    else if (mov->origen.x == mov->destino.x && mov->origen.y == mov->destino.y) //mismo punto
        flag = MISMO_BOT;
    else if (tablero.v[mov->origen.x][mov->origen.y] !=    tablero.v[mov->destino.x][mov->destino.y])
        flag = ENTRE_BOTONES;
    else if (mov->origen.x-mov->destino.x != 0)
    {
        pendiente = ((double)mov->destino.y - mov->origen.y)/(mov->destino.x - mov->origen.x);
        if (pendiente != 0 && pendiente != 1 && pendiente != -1)
            flag = DIR_INVALIDA;
    }
    else
    {
        punto_t direccion;
        calcularDireccion(*mov, &direccion);
        if(hayBtnsEntreMedio(tablero, *mov, direccion))
            flag = ENTRE_BOTONES;
    }

    return flag;
}

int hayMovimientosValidos(matriz_t tablero) //se invoca luego de cada turno para saber si hay un ganador
{
    int flag = 0;
    int i=0, j=0;
    while(i>=0 && i<tablero.n && j<tablero.n && j>=0 && !flag)
    {
        if(tablero.v[i][j] != VACIO)
        {
            punto_t pos = {i, j};
            flag = buscarBoton(tablero, pos);
        }

        if(++j == tablero.n)
        {
            i++;
            j=0;
        }
    }

    return flag;
}

static int buscarBoton(matriz_t tablero, punto_t pos)
{
    int flag = 0;
    int i;

    for(i=0; i<INC_MAX && !flag; i++) //para cada una de las cuatro direcciones...
    {
        punto_t direccion = {dir_inc[i][0], dir_inc[i][1]};
        punto_t limiteSuperior = {tablero.n-1,tablero.n-1};
        movimiento_t puntos = {pos, limiteSuperior};
        flag = esMovimientoValido(tablero, puntos, direccion, condMovimientoTurno);
    }

    return flag;
}

static int esMovimientoValido(matriz_t tablero, movimiento_t puntos, punto_t dir, int (*cmp)(movimiento_t, char, char))
{
    int i,j, flag = 0;
    char c, boton = tablero.v[puntos.origen.x][puntos.origen.y];

    for(i=puntos.origen.x+dir.x, j=puntos.origen.y+dir.y; i>=0 && i<tablero.n && j>=0 && j<tablero.n && !flag; i+=dir.x, j+=dir.y)
    {
        punto_t posActual = {i,j};
        movimiento_t p = {posActual, puntos.destino};

        if((c = tablero.v[i][j]) != VACIO && c != boton)
            flag = 2; //ya que debe salir del ciclo for pero luego retornar 0
        else
            flag = (*cmp)(p, boton, c);
    }

    return flag % 2; //para que en caso de el flag sea igual a 2, retorne 0
}

static int condMovimientoTurno(movimiento_t puntos, char boton, char botonLeido)
{
    return (boton == botonLeido);
}

static int condMovimientoJugador(movimiento_t puntos, char boton, char botonLeido)
{
    int valorRetorno = 0;
    if(puntos.origen.x == puntos.destino.x && puntos.origen.y == puntos.destino.y)
    {
        valorRetorno = 2; //el punto de origen no tiene un boton del mismo color
        if(boton == botonLeido)
            valorRetorno = 1;
    }

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

    for(i=mov.origen.x, j=mov.origen.y; i != mov.destino.x || j != mov.destino.y; i+=dir.x, j+=dir.y)
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

int realizarCortePc(matriz_t * tablero, int * btnsPC)
{
    movimiento_t mov;
    int estadoError = calcularMovPc(*tablero, &mov);
    if(estadoError == SIN_ERROR)
    {
        punto_t direccion;
        calcularDireccion(mov, &direccion);
        *btnsPC += realizarCorte(tablero, mov, direccion);
    }

    return estadoError;
}

static int calcularMovPc(matriz_t tablero, movimiento_t * mov)
{
    int estadoError = SIN_ERROR;
    size_t dim = 0;
    movimiento_t * mov_vec = NULL;
    int i=0, j=0;
    int estrategia = randInt(0, 1);

    while(estadoError==SIN_ERROR && i<tablero.n && j<tablero.n)
    {
        if(tablero.v[i][j] != VACIO)
        {
            int k;
            for(k=0; k<INC_MAX; k++)
            {
                punto_t direccion = {dir_inc[k][0], dir_inc[k][1]};
                punto_t posActual = {i,j};
                if(estrategia == 0) //Movimiento Minimo
                     estadoError = calcularMovPcEnDir(tablero, posActual, direccion, condMinMov, &dim, &mov_vec);
                else                //Movimiento Maximo
                     estadoError = calcularMovPcEnDir(tablero, posActual, direccion, condMaxMov, &dim, &mov_vec);

            }

        }

        if(++j == tablero.n)
        {
            i++;
            j=0;
        }
    }

    if(estadoError == SIN_ERROR)
    {
        int indice = randInt(0, dim - 1);

        *mov = mov_vec[indice];
    }

    if(mov_vec != NULL)
        free(mov_vec);

    return estadoError;
}


static int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, int (*cond)(int,char,char), size_t * dim, movimiento_t ** pmov_vec)
{
    int i,j, cantBtns = 0, estadoError = SIN_ERROR;
    char boton = tablero.v[pos.x][pos.y];

    for(i=pos.x, j=pos.y;estadoError == SIN_ERROR && i>=0 && i<tablero.n && j>=0 && j<tablero.n && (*cond)(cantBtns, boton, tablero.v[i][j]); i+=dir.x, j+=dir.y)
    {
        if(tablero.v[i][j] == boton)
        {
            cantBtns++;
            if (cantBtns >= MIN_MOV)
            {
              if (*dim == 0 || (*pmov_vec)->cantBotones <= cantBtns)
                {
                    if (*dim == 0 || (*pmov_vec)->cantBotones < cantBtns)
                        *dim = 0;

                    movimiento_t mov = {pos, {i, j}, cantBtns};
                    estadoError = agregarMovimiento(pmov_vec, mov, dim);
                }
            }
        }
    }

    return estadoError;
}

static int condMinMov(int cantBotones, char boton, char botonPosActual)
{
    int continuarCiclo = 0;
    if((botonPosActual == boton || botonPosActual == VACIO) && cantBotones < MIN_MOV)
        continuarCiclo = 1;

    return continuarCiclo;
}

static int condMaxMov(int cantBotones, char boton, char botonPosActual)
{
    int continuarCiclo = 0;
    if(botonPosActual == boton || botonPosActual == VACIO)
        continuarCiclo = 1;

    return continuarCiclo;
}

static int agregarMovimiento(movimiento_t ** pmov_vec, movimiento_t mov, size_t * dim)
{
    int estadoError = SIN_ERROR;
    if(*dim%BLOQUE_MEM == 0)
        *pmov_vec = realloc(*pmov_vec, (*dim + BLOQUE_MEM) * sizeof(**pmov_vec));
    (*dim)++;
    if(*pmov_vec != NULL)
        (*pmov_vec)[*dim - 1] = mov;
    else
        estadoError = E_MEM_DIN;

    return estadoError;
}
