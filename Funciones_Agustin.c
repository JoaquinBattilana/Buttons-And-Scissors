#include "buttonsAndScissorsBack.h"

#include <stdio.h>
#include <stdlib.h>

#define LIMPIAR_BUFFER while(getchar()!=EOF)
#define BLOQUE 10

char * vec_error[] = {
    "",
    "Comando invalido\n",
    "Nombre nulo\n",
    "Nombre demasiado largo\n",
    "Los botones no se encuentran en la misma horizontal, vertical o diagonal\n",
    "",
    "",
    "Los botones no son del mismo color\n",
    "El corte no tiene una única variedad de botones\n"
};

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

int validar(movimiento_t * mov, matriz_t tablero)
{
    int flag = 0;
    if (mov->origen.y != mov->destino.y || mov->origen.x != mov->destino.x || mov->destino.x != mov->origen.x - mov->origen.y + mov->destino.y)
        flag = 4;
    if (!flag && mov->origen.x > tablero.n && mov->origen.y > tablero.n)
        flag = 5;
    if (!flag && mov->destino.x > tablero.n && mov->destino.y > tablero.n)
        flag = 6;
    if (!flag && tablero.v[mov->origen.x][mov->origen.y] != tablero.v[mov->destino.x][mov->destino.y])
        flag = 7;
    punto_t direccion;
    calcularDireccion(*mov, &direccion);
    if (!flag && esMovimientoValido(tablero, mov->origen, *mov, direccion))
        flag = 8;
    return flag;
}

   

void leer_movimiento(movimiento_t * mov, matriz_t tablero)
{
    char ultimo_caracter, flag_error = 0, vector[267], nombre_archivo[256];
    int i, k;
    printf("Ingrese el comando:\n");
    do
    {
        ultimo_caracter = 0;
        fgets(vector, 267, stdin);
        sscanf("[%d,%d] [%d,%d]%c", mov->origen.x, mov->origen.y, mov->destino.x, mov->destino.y, &ultimo_caracter);
        if(ultimo_caracter == '\n')
            flag_error = validar(mov, tablero);
        else if(vector[0] == 's' && vector[1] == 'a' && vector[2] == 'v' && vector[3] == 'e' 
                && vector[4] == 'f' && vector[5] == 'i' && vector[6] == 'l' && vector[7] == 'e'
                && vector[8] == ' ')
             {
                for(i = 9, k = 0 ; vector[i] != '\n' && i < 267; i++, k++)
                    nombre_archivo[k] = vector[i];
                nombre_archivo[++k] = '\0';
                if(i == 267)
                {
                    flag_error = 3;
                    LIMPIAR_BUFFER;
                }
                else if(k == 1)
                        flag_error = 2;
                     else
                        flag_error = guardarJuego(nombre_archivo, tablero);
             }
        else if(!strcmp(vector, "quit"))
             {
                  printf("¿Está seguro que quiere salir (Y/N)?\n");
                  fgets(vector, 3, stdin);
                  if(vector[0]=='Y' && vector[1]=='\n')
                  {
                     printf("¿Desea guardar la partida antes de salir (Y/N)?\n");
                     fgets(vector, 3, stdin);
                     if(vector[0]=='Y' && vector[1]=='\n')
                     {
                        printf("Ingrese el nombre del archivo:\n");
                        for(k = 0 ; ultimo_caracter = getchar() != '\n' && i < 256; k++)
                            nombre_archivo[k] = vector[k];
                        nombre_archivo[++k] = '\0';
                        if(i == 256)
                        {
                            flag_error = 3;
                            LIMPIAR_BUFFER;
                        }
                        else if(k == 1)
                               flag_error = 2;
                             else
                               flag_error = guardarJuego(nombre_archivo, tablero);
                     }
                     else if(vector[0]=='N' && vector[1]=='\n')
                             exit(1);
                          else
                              flag_error = 1;
                  }
                  else if(vector[0]=='N' && vector[1]=='\n')
                             exit(1);
                       else
                           flag_error =1;
             }
             else
             {
                 flag_error = 1;
                 LIMPIAR_BUFFER;
             }
        if(flag_error == 5)
            printf("No existe la posición:[%d,%d]\n", mov->origen.x, mov->origen.y);
        else if(flag_error == 6)
                 printf("No existe la posición:[%d,%d]\n", mov->destino.x, mov->destino.y);
             else
                 printf("Error: %s", vec_error[flag_error]);
    }while (flag_error);
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
    
}

