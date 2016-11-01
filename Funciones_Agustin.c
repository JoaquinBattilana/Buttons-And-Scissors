
#include <stdio.h>
#include <stdlib.h>

#define LIMPIAR_BUFFER while(getchar()!=EOF)

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

void(char * s)
{
    for(i=0; c=getchar() !=

void leer_movimiento(movimiento_t * mov, matriz_t tablero)
{
    char estado, ultimo_caracter, flag_error = 0, vector[16], flag_salida = 1, *nombre_archivo;
    printf("Ingrese el comando:\n");
    do
    {
        flag = 1
        ultimo_caracter = 0;
        fgets(vector, 16, stdin);
        sscanf("[%d,%d] [%d,%d]%c", mov->origen.x, mov->origen.y, mov->destino.x, mov->destino.y, &ultimo_caracter);
        if(ultimo_caracter == '\n')
            flag_salida= validar(mov, tablero);
        else if(!strcmp(vector, "savefile "))
             {
                \\guardar nombre dinamicamente
             }
        else if(!strcmp(vector, "quit"))
             {
                  printf("¿Está seguro que quiere salir (Y/N)?\n");
                  fgets(vector, 16, stdin);
                  if(vector[0]=='Y' && vector[1]=='\n')
                  {
                     printf("¿Desea guardar la partida antes de salir (Y/N)?\n");
                     fgets(vector, 16, stdin);
                     if(vector[0]=='Y' && vector[1]=='\n')
                     {
                        printf("Ingrese el nombre del archivo:\n");
                        \\guarda nombre dinamicamente
                        flag_salida = 0;
                     }
                     else if(vector[0]=='N' && vector[1]=='\n')
                             flag_salida = 0;
                  }
                  else if(vector[0]=='N' && vector[1]=='\n')
                             flag = 1;
             }
             else
             {
                 flag_error = 1;
                 LIMPIAR_BUFFER;
             }
                     
        }
        if(flag_error)
            printf("comando invalido\n");
    }while (flag_salida);
}

int menu()
{
    char respuesta;
    char ultimo_caracter;
    do
    {
        ultimo_caracter = '\0';
        printf("Escribir el numero de la opción deseada:\n");
        printf("1-Juego de dos jugadores\n");
        printf("2-Juego contra computadora\n");
        printf("3-Recuperar un juego grabado\n");
        printf("4-Terminar\n");
        scanf("%c%c", &respuesta, &ultimo_caracter);
        if (ultimo_caracter != '\n')
        {
            LIMPIAR_BUFFER;
        }
    } while (ultimo_caracter != '\n' || respuesta < '1' || respuesta > '4');

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
