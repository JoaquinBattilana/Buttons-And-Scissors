#include "buttonsAndScissorsFront.h"

int main(void){
    imprimirBanner();
    randomize(); //setea semilla
    int opcion,salir=0,error=SIN_ERROR;
    tipoJuego juego;
    jugador jugadores[JUGADORES] = {{0}};
    do{
        opcion=menu();
        switch(opcion){
            case JVSJ:
                juego.modoJuego = 0;
            case JVSCPU:
                juego.tablero.n= validar_dim();
                error = matrizDsdArchivo(&juego);
                juego.turno=(randInt(1,JUGADORES)); //se decide pseudoaleatoriamente que jugador comienza
                if (error)
                    printError(error);
                else {
                    if (opcion == JVSCPU) {
                        jugadores[JUGADORES - 1].esPC = 1;
                        juego.modoJuego = 1;
                    }
                    salir = jugar(&juego, jugadores);
                }
                break;
            case CARGAR:
                error = cargarJuego(&juego);
                if (error)
                    printError(error);
                else
                {
                if(juego.modoJuego==1)
                    jugadores[JUGADORES-1].esPC=1;
                salir = jugar(&juego, jugadores);
                }
                break;
            case SALIR:
                salir = 1;
                break;
        }
    }while(!salir);
    return 0;
}
