
#include <stdio.h>
#include <stdlib.h>

#define LIMPIAR_BUFFER() while(getchar()!=EOF)

typedef struct
{
    int inicial[2];
    int final[2];
} movimiento;

int validar(movimiento * mov, unsigned int dim, char ** matriz)
{
    int flag = 0;
    if (mov->inicial[1] != mov->final[1] || mov->inicial[0] != mov->final[0] || mov->final[0] != mov->inicial[0] - mov->inicial[1] + mov->final[1])
    {
        flag = 1;
        printf("Los botones no se encuentran en la misma horizontal, vertical o diagonal\n");
    }
    if (!flag && mov->inicial[0] > dim && mov->inicial[1] > dim)
    {
        flag = 1;
        printf("No existe la posición:[%d,%d]\n", mov->inicial[0], mov->inicial[1]);
    }
    if (!flag && mov->final[0] > dim && mov->final[1] > dim)
    {
        flag = 1;
        printf("No existe la posición:[%d,%d]\n", mov->final[0], mov->final[1]);
    }
    if (!flag && matriz[mov->inicial[0]][mov->inicial[1]] != matriz[mov->final[0]][mov->final[1]])
    {
        flag = 1;
        printf("Los botones no son del mismo color\n");
    }
    if (!flag && 1)                              //Funcion entre botones
    {
        flag = 1;
        printf("El corte no tiene una única variedad de botones\n");
    }

    return flag;
}

void leer_movimiento(movimiento * mov, size_t dim, char ** matriz)
{
    char estado, ultimo_caracter, flag = 1, caracter, caracter_final;
    do
    {
        ultimo_caracter = '\0';
        printf("Ingrese el comando:\n");
        switch (estado = getchar()) {
            case '[': {
                scanf("%d,%d] [%d,%d]%c", mov->inicial[0], mov->inicial[1], mov->final[0], mov->final[1], &ultimo_caracter);
                if (ultimo_caracter == '\n')
                    flag = 0;
                break;
            }
            case 's': {
                scanf("ave%c", ultimo_caracter);
                if (ultimo_caracter == ' ') {
                    //Guardar
                    printf("La partida se guardó exitosamente\n");
                }
                break;
            }
            case 'q':
            {
                scanf("uit%c, ultimo_caracter");
                if (ultimo_caracter == '\n')
                {
                    printf("¿Está seguro que quiere salir (Y/N)?\n");
                    caracter_final = '\0';
                    if ((caracter = getchar()) == 'Y' && (caracter_final = getchar()) == '\n')
                    {
                        printf("¿Desea guardar la partida antes de salir (Y/N)?\n");
                        if ((caracter = getchar()) == 'Y' && (caracter_final = getchar()) == '\n')
                        {
                            printf("Ingrese el nombre del archivo:\n");
                            //Guardar
                            //Sale
                        }else if (caracter == 'N' && caracter_final == '\n')
                            ;
                        else
                            ultimo_caracter = '\0';
                    }
                    else if (caracter != 'N' || caracter_final != '\n')
                        ultimo_caracter = '\0';
                }
            }
                if(ultimo_caracter != '\n')
                {
                    printf("Comando invalido\n");
                    LIMPIAR_BUFFER();
                }
        }
    } while (flag || validar(mov, dim, matriz));
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

int main(void){
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
