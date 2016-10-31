#include "backEnd.h"
#include "frontEnd.h" //despues poner el nombre original de los archivos como lo pide la consigna

#define JVSJ 1
#define JVSCPU 2
#define CARGAR 3
#define SALIR 4

typedef struct{
    size_t botones_cortados;
    size_t esPC;
}jugador;

int validar_dim(void){
    size_t dim = 0;
    char ultimo_caracter;
    do{
        ultimo_caracter = '\0';
        printf("Ingrese la dimención del tablero (mínimo 5 y máximo 30)\n");
        scanf("%d%c", &dim, &ultimo_caracter);
    }
    while(ultimo_carracter != '\n' && dim < 5 && dim > 30)
    return dim;
}   //ESTO ES FRONTEND PAJEROSSSSSSS xDDDD (igual supongo que fue para probarlo)

int validar_volvermenu(void){ //?? Hmmm
    

int main(void)
{
    size_t opcion, dim, flag = 1, turno, flag_2 = 1;
    matriz_t tablero;
    opcion = menu();
    jugador jugadores[2];
    char caracter, caracter_final;
    while(flag_2){
            switch(opcion){
                case JVSJ:
                case JVSCPU:{
                  tablero.n = validar_dim();
                  tablero.v = matrizDsdArchivo(tablero);
                  jugadores[0].botones_cortados = jugadores[1].botones_cortados = 0;
                  jugadores[0].esPC = 0;
                  juagadores[1].esPC = (opcion == JVSCPU)? 1 : 0;
                  randomize();
                  turno = randInt(0,1);
                  break;
                }
                case CARGAR:{
            
                break;
                }
                case SALIR:{
                flag = 0;
                flag_2 = 2;
                }
            }
            while(flag){
                printf("Botones J1: %d, J2: %d\n", jugadores[0].botones_cortados, jugadores[1].botones_cortados);
                PRINTEO TABLERO;
                printf("Turno jugador %d\n", turno+1);
                switch(jugadores[turno].esPC){
                    case 0:{
                        leermovimiento(mov, tablero.n, tablero.v);                                   \\NO CREE LA VARIABLE MOV PORQUE NOSE COMO LO IBAMOS A HACER
                        jugadores[turno].botones_cortados = realizarcorte(tablero, ALGO, ALGO, ALGO) \\NOSE COMO LO IBAMOS A HACER
                    }
                    case 1:{
                        FUNCION MOV MAQUINA
                    }
                }
                if((hayMovimientosValidos(tablero))!){ //AGUSTIN LA CONCHA DE TU HERMANA APRENDE A PONER EL NOT :@
                    printf("Gano el jugador %d\n", turno+1);
                    liberarMatrizCuadrada(tablero.v,tablero.n);
                    flag_2 = validar_volvermenu();
                }
                else
                    turno = (turno + 1) % 2;
            }
    }
    return 0;
}
