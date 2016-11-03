#include <stdio.h>
#include <stdlib.h>
#include "random.h"
#include <ctype.h>
#include "back.h"
#define LIMPIAR_BUFFER while(getchar()!=EOF)

enum opciones {JVSJ=1, JVSCPU, CARGAR, SALIR};
enum ia {NOPC=0, PC};
enum errores {SIN_ERROR=0, ERROR, E_MEM_DIN,E_ARCHIVO_MATRICES,E_ABRIR_ARCHIVO, E_NOMBRE_ARCHIVO,
    E_CREAR_ARCHIVO, E_ARCHIVO_MAL,NO_SOBRESCRIBIR};

#define JUGADORES 2
#define MAXLARGOARCHIVO 255
#define MAXNPALABRA 9
#define BLOQUE 10


typedef struct{
    size_t botones_cortados;
    size_t esPC;
}jugador;


typedef struct
{
    char ** posicion;
    int dim;
} matriz_t;

typedef struct tipoJuego{
    matriz_t tablero;
    size_t modoJuego;
    size_t turno;
} tipoJuego;

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


int menu();
int validar_dim();
int matrizDsdArchivo(tipoJuego * juego);
static int buscarMatriz(FILE * archivo, int random, int n);
static int escribirMatriz(FILE * archivo, char ** matriz, int n);
static char ** creaMatrizCuadrada(int n);
static void liberarMatrizCuadrada(char ** matriz, int n);
void imprimirTablero(matriz_t tablero);
int cargarJuego(tipoJuego * juego);
void leer_movimiento(movimiento_t * mov, tipoJuego * juego);
static int leerArchivo(char * nombreArchivo, tipoJuego * juego);
void printError(int error);
int guardarJuego(char * nombreArchivo, tipoJuego * juego);

int main(void){
	printf("Bienvenido al juego de botones y tijeras, ingrese una opcion: \n");
	randomize();
	int opcion,salir=0,error=SIN_ERROR;
	tipoJuego juego;
	jugador jugadores[JUGADORES]={0};
	do{
		opcion=menu();
			switch(opcion){
				case JVSJ:
				case JVSCPU:
					juego.tablero.dim= validar_dim();
					error = matrizDsdArchivo(&juego);
					if (error)
						printError(error);
					else
						if(opcion==JVSCPU)
							jugadores[JUGADORES-1].esPC=1; // tomo a la pc como el ultimo jugador, en este caso el 2.
						jugar(&juego, jugadores);
					break;
				case CARGAR:
					error = cargarJuego(&juego);
					if (error)
						printError(error);
					else
						if(juego.modoJuego==1)
							jugadores[JUGADORES-1].esPC=1;
						jugar(&juego, jugadores);
					break;
				case SALIR:
					salir = 1;
					break;
			}
	}while(!salir);

	return 0;
	
}

int jugar(tipoJuego * juego, jugador jugadores[JUGADORES]){
	int ganador=0;
	int flag=0;
	juego->turno=randInt(1,JUGADORES); //hago que empiece uno random.
	do{
        printf("Botones J1: %d, J2: %d\n", jugadores[0].botones_cortados, jugadores[1].botones_cortados);
        imprimirTablero(juego->tablero);
        printf("Turno jugador %d\n", juego->turno);
        switch(jugadores[turno-1].esPC){
            case NOPC:{
                leermovimiento(&mov, &juego);                                  
                jugadores[turno-1].botones_cortados = realizarcorte();
            }
            case ESPC:{
                FUNCION MOV MAQUINA
            }
        }
        if(!hayMovimientosValidos(juego->tablero.posicion)){
            printf("Gano el jugador %d\n", juego->turno);
            liberarMatrizCuadrada(juego->tablero.dimension,juego->tablero.dimension);
            flag = validar_volvermenu();
        }
        else
            turno = (turno%JUGADORES)+1;
    }while (!ganador && !flag);
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
        printf("Opcion: ");
        scanf("%c%c", &respuesta, &ultimo_caracter);
        if (ultimo_caracter != '\n')
        {
            LIMPIAR_BUFFER;
        }
    } while (ultimo_caracter != '\n' || respuesta < '1' || respuesta > '4');

    return respuesta-'0';
}

int validar_dim(){
    size_t dim = 0;
    char ultimo_caracter;
    do{
        ultimo_caracter = '\0';
        printf("Ingrese la dimensión del tablero (mínimo 5 y máximo 30)\n");
        printf("Dimension: ");
        scanf("%ld%c", &dim, &ultimo_caracter);
    }
    while(ultimo_caracter != '\n' || dim < 5 || dim > 30);
    return dim;
}  

int matrizDsdArchivo(tipoJuego * juego){
    int error=SIN_ERROR;
    int i, c;
    char nombreArchivo[10], whitespace=0;
    sprintf(nombreArchivo, "./%dx%d", juego->tablero.dim,juego->tablero.dim );
    char ** aux=NULL;
    FILE * archivo;
    archivo = fopen(nombreArchivo, "r");
    if (archivo!=NULL) {
        fscanf(archivo, "%d%c", &c, &whitespace);
        if (whitespace != '\n') {
            error = E_ARCHIVO_MATRICES;
        } 
        else {
            error = buscarMatriz(archivo,randInt(1,c), juego->tablero.dim);
            if (!error) {
                aux = creaMatrizCuadrada(juego->tablero.dim);
                if (aux == NULL)
                    error = E_MEM_DIN;
                else {
                    error = escribirMatriz(archivo, aux, juego->tablero.dim);
                    if (error) {
                        error = E_ARCHIVO_MATRICES;
                        liberarMatrizCuadrada(aux, juego->tablero.dim);
                    }
                    else{
                    	juego->tablero.posicion=aux;
                    }
                }
            }

        }
        fclose(archivo);
    }
    else
        error=E_ABRIR_ARCHIVO;
    return error;
}

static int buscarMatriz(FILE * archivo, int random, int n){
    int error=SIN_ERROR, flag=0, lineas=0;
    char c;
    while(random!=1 && !error){
        c=fgetc(archivo);
        if (isalpha(c)) {
            flag++;
        }
        else if(c=='\n' && flag==n) {
            flag = 0;
            lineas++;
        }
        else if(lineas==n && c=='-' && (c=fgetc(archivo))=='\n'){
            random--;
        }
        else
            error=E_ARCHIVO_MATRICES;
    }
    return error;
}

static int escribirMatriz(FILE * archivo, char ** matriz, int n){
    int i,j;
    char c;
    int error=SIN_ERROR;
    for(i=0; i<n && !error; i++){
        for(j=0; j<n && !error; j++){
            c=fgetc(archivo);
            if(isalpha(c))
                matriz[i][j]=c;
            else
                error=E_ARCHIVO_MATRICES;
        }
        c=fgetc(archivo);
        if(c!='\n' && c!=EOF)
            error=E_ARCHIVO_MATRICES;
    }
    return error;
}


static char ** creaMatrizCuadrada(int n){
    int i,flag,j;
    char ** aux=NULL;
    aux=malloc(sizeof(*aux)*n);
    if(aux!=NULL){
        for (i=0, flag=1; i<n && flag; i++){
            aux[i]=malloc(sizeof(**aux)*n);
            if(aux[i]==NULL){
                flag=0;
                for(j=i-1;j>=0; j--)
                    free(aux[j]);
                free(aux);
            }
        }
    }
    return aux;
}

static void liberarMatrizCuadrada(char ** matriz, int n){
    int i;
    for(i=0; i<n; i++)
        free(matriz[i]);
    free(matriz);
    return;
}

void imprimirTablero(matriz_t tablero){
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
            i++;
    }

    printf("\n\n");
    return;
}

void leer_movimiento(movimiento_t * mov, tipoJuego * juego)
{
    char ultimo_caracter, comando[MAXLARGOARCHIVO+MAXNPALABRA+1];
    int i, k, flag_error=SIN_ERROR, n=0;
    printf("Ingrese el comando:\n");
    do
    {
    	printf("Comando: ");
        ultimo_caracter = 0;
        fgets(vector, MAXLARGOARCHIVO+MAXNPALABRA+1, stdin);
        if(getchar()=)
        n = sscanf(vector,"[%d,%d] [%d,%d]%c", mov->origen.x, mov->origen.y, mov->destino.x, mov->destino.y, &ultimo_caracter);
        if(n == 5 && ultimo_caracter == '\n')
            error = validarMov(mov, juego);
        else if(strncmp(vector, "savegame ", 9));
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
                  printf("¿Está seguro que quiere salir (Y/N)? ");
                  fgets(vector, 3, stdin);
                  if(vector[0]=='Y' && vector[1]=='\n')
                  {
                     printf("¿Desea guardar la partida antes de salir (Y/N)? ");
                     fgets(vector, 3, stdin);
                     if(vector[0]=='Y' && vector[1]=='\n')
                     {
                        printf("Ingrese el nombre del archivo: ");
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
                               flag_error = guardarJuego(nombre_archivo, juego);
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
            
    }while (error);
}



int cargarJuego(tipoJuego * juego){
    char c=0;
    int i=0;
    int error=SIN_ERROR;
    char * nombreArchivo=NULL;
    char * aux=NULL;
    printf("Ingrese el nombre del archivo a cargar: ");
    while((c=getchar())!='\n' && !error){
        if (i%BLOQUE==0){
            aux = realloc(aux, sizeof(char)*(i+BLOQUE));
            if (aux==NULL)
                error=E_MEM_DIN;
            else
                nombreArchivo=aux;
         }
        nombreArchivo[i]=c;
        i++;
    }
    if(!error){
        nombreArchivo[i]=0;
        nombreArchivo=realloc(nombreArchivo, sizeof(char)*i);
        error = leerArchivo(nombreArchivo, juego);
    }
    return error;
}

static int leerArchivo(char * nombreArchivo, tipoJuego * juego){
    char c;
    int error=SIN_ERROR, flag=0,i,j;
    FILE * archivo;
    archivo=fopen(nombreArchivo, "rb");
    if (archivo!=NULL) {
        juego->tablero = creaMatrizCuadrada(juego->dim);
        if (juego->tablero != NULL) {
            c = fgetc(archivo);
            if (c == '0' || c == '1') {
                juego->modoJuego = c;
                c = fgetc(archivo);
                if (c == '1' || c == '2') {
                    juego->turno = c;
                    for (i = 0; i < juego->dim && !error; i++) {
                        for (j = 0; j < juego->dim && !error; j++) {
                            c = fgetc(archivo);
                            if (isalpha(c))
                                juego->tablero[i][j] = c;
                            else{
                                error=E_ARCHIVO_MAL;
                                }
                            }
                    }

                }
                else
                    error=E_ARCHIVO_MAL;
            }
            else
                error=E_ARCHIVO_MAL;
        } else
            error = E_MEM_DIN;
    }
    else
        error=E_ABRIR_ARCHIVO;
    if(error && error!=E_MEM_DIN)
        liberarMatrizCuadrada(juego->tablero, juego->dim);
    return error;
}

void printError(int error){
    char * s_errores[]={"No hay error\n", "Error\n", "Error en la memoria dinamica\n", "Error, el archivo de matrices esta mal hecho\n",
                        "Error, el archivo esta corrupto o no existe\n", "Error en el nombre de archivo \n",
                        "Error al crear/sobrescribir el archivo \n", "El archivo esta corrupto o mal escrito\n",
                        "Elija otro nombre \n"};
    printf("%s\n", s_errores[error]);
    return;
}

int guardarJuego(char * nombreArchivo, tipoJuego * juego){
    int i,j;
    FILE * archivo;
    int error=SIN_ERROR;
    archivo = fopen(nombreArchivo, "wb");
    if (archivo!=NULL){
        fprintf(archivo,"%d%d", juego->modoJuego, juego->turno);
        for(i=0;i<juego->dim;i++){
            for(j=0;j<juego->dim;j++)
                fputc(juego->tablero[i][j], archivo);
        }
    }
    else
        error=E_CREAR_ARCHIVO;
    if (!error)
        fclose(archivo);
    return error;
}