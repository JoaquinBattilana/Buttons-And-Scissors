#include <stdlib.h>
#include "random.h" //para los randInt

#define INC_MAX 4
#define VACIO 0

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

char dir_inc[INC_MAX][2] = {{0,1},{1,1},{1,0},{1,-1}}; //incremento direcciones DERECHA, D_ABAJO, ABAJO, I_ABAJO

enum {NADA=0, SOBREESCRIBIR, AGREGAR};

#define MIN_MOV 2

typedef struct
{
    punto_t origen;
    punto_t destino;
    int cantBotones;
} movimiento_t;

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
        flag = esMovimientoValido(tablero, pos, (movimiento_t) {punto1, punto2}, direccion, boton);
    }

    return flag;
}

int esMovimientoValido(matriz_t tablero, punto_t pos, movimiento_t mov, punto_t dir, char boton)
{
    int flag = 0;
    int i,j;
    char c;
    int minFil, maxFil, minCol, maxCol;
    puntoMaxMin(mov.origen, mov.destino, &minFil, &maxFil, &minCol, &maxCol);

    for(i=pos.x, j=pos.y; i>=minFil && i<=maxFil && j>=minCol && j<=maxCol && !flag; i+=dir.x, j+=dir.y)
    {
        if((c = tablero.v[i][j]) != VACIO && c != boton)
            flag = 2; //ya que debe salir del ciclo for pero luego retornar 0
        else if(c == boton)
            flag = 1;
    }

    return flag % 2; //para que en caso de el flag ser 2, retorne 0
}

punto_t calcularDireccion(movimiento_t mov)
{
    punto_t direccion = {mov.destino.x - mov.origen.x, mov.destino.y - mov.origen.y};
    if(direccion.x != 0)
        direccion.x = direccion.x / abs(direccion.x);
    if(direccion.y != 0)
        direccion.y = direccion.y / abs(direccion.y);

    return direccion;
}

int realizarCorte(matriz_t tablero, movimiento_t mov, punto_t dir)
{
    int i,j;
    int botonesCortados = 0;

    for(i=mov.origen.x, j=mov.origen.y; i != mov.destino.x && j != mov.destino.y; i+=dir.x, j+=dir.y)
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

int realizarCortePc(matriz_t tablero)
{
    movimiento_t mov = calcularMovPc(tablero);
    punto_t direccion = calcularDireccion(mov);
    mov.cantBotones = realizarCorte(tablero, mov, direccion);
    return mov.cantBotones;
}

movimiento_t calcularMovPc(matriz_t tablero)
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
                    mov_vec = calcularMovPcEnDir(tablero, posActual, direccion, c, condMinMov, compMinMov, &dim, mov_vec);
                else //Movimiento Maximo
                    mov_vec = calcularMovPcEnDir(tablero, posActual, direccion, c, condMaxMov, compMaxMov, &dim, mov_vec);

            }

        }

        if(++j == tablero.n)
        {
            i++;
            j=0;
        }
    }

    int indice = randInt(0, dim);

    return mov_vec[indice];
}


movimiento_t* calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, char boton, int (*cond)(int),int (*cmp)(movimiento_t*, size_t, int), size_t * nDim, movimiento_t* mov_vec)
{
    int i,j;
    int cantBtns = 0;
    int ret;
    size_t dim = *nDim;
    char c;

    for(i=pos.x, j=pos.y; ((c = tablero.v[i][j]) == boton || c == VACIO) && (*cond)(cantBtns); i+=dir.x, j+=dir.y)
    {
        if(c == boton)
        {
            cantBtns++;

            if(cantBtns >= MIN_MOV) {
                ret = (*cmp)(mov_vec, dim, cantBtns);

                switch (ret) {
                    case NADA:
                        break;

                    case SOBREESCRIBIR: {
                        dim = 1;
                        mov_vec = realloc(mov_vec, dim * sizeof(*mov_vec)); //OPTIMISTA
                        mov_vec[0] = {pos, {i, j}, cantBtns};
                        break;
                    }

                    case AGREGAR: {
                        dim++;
                        mov_vec = realloc(mov_vec, dim * sizeof(*mov_vec));
                        mov_vec[dim - 1] = {pos, {i, j}, cantBtns};
                        break;
                    }

                }
            }
        }
    }

    *nDim = dim;
    return mov_vec;
}

int condMinMov(int cantBotones)
{
    int continuarCiclo = 0;
    if(cantBotones < MIN_MOV)
        continuarCiclo = 1;

    return continuarCiclo;
}

int condMaxMov(int cantBotones)
{
    return 1;
}

int compMaxMov(movimiento_t* v, size_t n, int cantBtns)
{
    int valorRet = NADA;
    if(n == 0)
        valorRet = SOBREESCRIBIR;
    else if(v[0].cantBotones < cantBtns)
        valorRet = SOBREESCRIBIR;
    else if(v[0].cantBotones == cantBtns)
        valorRet = AGREGAR;

    return valorRet;
}


int compMinMov(movimiento_t* v, size_t n, int cantBtns)
{
    int valorRet = NADA;
    if(n == 0)
        valorRet = SOBREESCRIBIR;
    else if(v[0].cantBotones == cantBtns)
        valorRet = AGREGAR;

    return valorRet;
}
