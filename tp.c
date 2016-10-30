#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "random.h"
#include <ctype.h>

#define limpiaBuffer while(getchar()!='\n');
#define MAXMATRICESPORARCHIVO 5
enum errores {SIN_ERROR=0, ERROR, E_MEM_DIN,E_ARCHIVO_MATRICES,E_ABRIR_ARCHIVO, E_NOMBRE_ARCHIVO, E_CREAR_ARCHIVO};
enum retornos {FALSO=0, VERDADERO};
typedef enum errores error;

//aclaro que algunas cosas de final de archivo las tube que validar con \n y EOF porque con el editor de texto que tenia
//parece que automaticamente me agregaba un \n, no se si es por el editor o por linux, pero por las dudas valide las 2

static int escribirArchivo(char * , char ** , int , int , int );
static int existeArchivo( char * );
int guardarJuego(char ** , int , int , int );
static int sobrescribir();
static int buscarMatriz(FILE*, int, int);
static int escribirMatriz(FILE*, char **, int);
static void liberarMatrizCuadrada(char **, int);
static char ** creaMatrizCuadrada(int);
char **matrizDsdArchivo(int);
void printError(int);

int main(void){
    int i,j;
    char ** m;
    char n=5;
    m = matrizDsdArchivo(n);
    if (m!=NULL) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                printf("%c", m[i][j]);
            }
            putchar('\n');
        }
    }
    return SIN_ERROR;
}


int guardarJuego(char ** tablero, int tipo_juego, int turno, int n){
    int error=SIN_ERROR;
    int flag=0;
    char whitespace=0;
    char nombreArchivo[11];
    do{
        printf("Nombre del archivo?: ");
        scanf("%10s%c", nombreArchivo, &whitespace);
        if(whitespace=='\n'){
            if (existeArchivo(nombreArchivo) && sobrescribir()){
                    error = escribirArchivo(nombreArchivo, tablero,tipo_juego, turno, n);
                }
            else {
                error = escribirArchivo(nombreArchivo, tablero,tipo_juego, turno, n);
            }
        }
        else{
            error = E_NOMBRE_ARCHIVO;
            printf(error);
            limpiaBuffer
            error=SIN_ERROR;
        }
    } while (!error);
    if(error)
        printError(error);
    return error;
}



static int escribirArchivo(char * nombreArchivo, char ** tablero, int tipo_juego, int turno, int n){
    int i,j;
    FILE * archivo;
    int error=SIN_ERROR;
    archivo = fopen(nombreArchivo, "wb");
    if (archivo!=NULL){
        fputc(tipo_juego, archivo);
        fputc(turno, archivo);
        for(i=0;i<n;i++){
            for(j=0;j<n;j++)
                fputc(tablero[i][j], archivo);
        }
    }
    else
        error=E_CREAR_ARCHIVO;
    if (error)
        printError(error);
    fclose(archivo);
    return error;

}

static int sobrescribir(){
    char respuesta;
    do{
    limpiaBuffer
    printf("Hay un archivo con el mismo nombre, desea sobrescribirlo?(S/N) \n");
    scanf("%c", &respuesta);
    }
    while( (respuesta!='s' || (respuesta!='n')));
    return 'n'-respuesta;
}

static int existeArchivo(char * nombreArchivo){
    int error = VERDADERO;
    FILE * aux;
    aux = fopen(nombreArchivo, "r");
    if (aux == NULL)
        error=FALSO;
    return error;
}

void printError(int error){
    char * s_errores[]={"No hay error\n", "Error\n", "Error en la memoria dinamica\n", "Error, el archivo de matrices esta mal hecho\n",
                        "Error, el archivo esta corrupto o no existe\n", "Error en el nombre de archivo \n",
                        "Error al crear/sobrescribir el archivo \n"};
    printf("%s\n", s_errores[error]);
    return;
}


char ** matrizDsdArchivo(int n){
    error error=SIN_ERROR;
    int i, c;
    char nombreArchivo[10], whitespace=0;
    sprintf(nombreArchivo, "%dx%d", n,n );
    char pwd[10]="./";
    char ** aux=NULL;
    FILE * archivo;
    archivo = fopen("/home/joaquin/Escritorio/TP-master/5x5", "r");
    if (archivo!=NULL) {
        fscanf(archivo, "%d%c", &c, &whitespace);
        if ((c > MAXMATRICESPORARCHIVO || c < 1) && whitespace != '\n') {
            error = E_ARCHIVO_MATRICES;
        } else {
            error = buscarMatriz(archivo, (rand() % c) + 1, n);
            if (!error) {
                aux = creaMatrizCuadrada(n);
                if (aux == NULL)
                    error = E_MEM_DIN;
                else {
                    error = escribirMatriz(archivo, aux, n);
                    if (error) {
                        error = E_ARCHIVO_MATRICES;
                        liberarMatrizCuadrada(aux, n);
                    }
                }
            }

        }
    }
    else
        error=E_ABRIR_ARCHIVO;
    if(error){
        printError(error);
        aux=NULL;
    }
    fclose(archivo);
    return aux;
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
//
// Created by joaquin on 29/10/16.
//
