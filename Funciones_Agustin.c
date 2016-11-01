
#include <stdio.h>
#include <stdlib.h>

#define LIMPIAR_BUFFER() while(getchar()!=EOF)

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

int validar(movimiento_t * mov, matriz_t tablero)
{
    int flag = 0;
    if (mov->origen.y != mov->destino.y || mov->origen.x != mov->destino.x || mov->destino.x != mov->origen.x - mov->origen.y + mov->destino.y)
    {
        flag = 1;
        printf("Los botones no se encuentran en la misma horizontal, vertical o diagonal\n");
    }
    if (!flag && mov->origen.x > dim && mov->origen.y > dim)
    {
        flag = 1;
        printf("No existe la posición:[%d,%d]\n", mov->inicial[0], mov->inicial[1]);
    }
    if (!flag && mov->destino.x > dim && mov->destino.y > dim)
    {
        flag = 1;
        printf("No existe la posición:[%d,%d]\n", mov->final[0], mov->final[1]);
    }
    if (!flag && tablero.v[mov->origen.x][mov->origen.y] != tablero.v[mov->destino.x][mov->destino.y])
    {
        flag = 1;
        printf("Los botones no son del mismo color\n");
    }
    punto_t direccion;
    calcularDireccion(*mov, &direccion);
    if (!flag && esMovimientoValido(tablero, mov->origen, *mov, direccion))
    {
        flag = 1;
        printf("El corte no tiene una única variedad de botones\n");
    }

    return flag;
}

void leer_movimiento(movimiento_t * mov, matriz_t tablero)
{
    char estado, ultimo_caracter, flag = 1, caracter, caracter_final, vector[16];
    printf("Ingrese el comando:\n");
    do
    {
        ultimo_caracter = 0;
        fgets(vector, 16, stdin);
        sscanf("%d,%d] [%d,%d]%c", mov->origen.x, mov->origen.y, mov->destino.x, mov->destino.y, &ultimo_caracter);
        if(ultimo_caracter == '\n')
            flag = validar(mov, tablero);
       
    }while (flag);
}

int menu()
{
    int respuesta;
    char ultimo_caracter;
    do
    {
        ultimo_caracter = '\0';
        printf("Escribir el numero de la opción deseada:\n");
        printf("1-Juego de dos jugadores\n");
        printf("2-Juego contra computadora\n");
        printf("3-Recuperar un juego grabado\n");
        printf("4-Terminar\n");
        scanf("%d%c", &respuesta, &ultimo_caracter);
        if (ultimo_caracter != '\n')
        {
            LIMPIAR_BUFFER();
        }
    } while (ultimo_caracter != '\n' && respuesta < 0 && respuesta > 5);

    return respuesta;
}


int main(void){ //coregir struct
    int i = menu();
    char ** matriz;
    movimiento mov;
    matriz = malloc(5*sizeof(*matriz));
    for(i=0; i<5; i++){
        *(matriz+i) = malloc(5*sizeof(**matriz));
    }
    leer_movimiento(&mov, 5, matriz);
    printf("%d", i);
}
