#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "random.h"
#include <ctype.h>

#define MAXMATRICESPORARCHIVO 5
enum errores {SIN_ERROR=0, ERROR, E_MEM_DIN,E_ARCHIVO_MATRICES,E_ABRIR_ARCHIVO};
typedef enum errores error;

//aclaro que algunas cosas de final de archivo las tube que validar con \n y EOF porque con el editor de texto que tenia
//parece que automaticamente me agregaba un \n, no se si es por el editor o por linux, pero por las dudas valide las 2

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

void printError(int error){
    char * s_errores[]={"No hay error\n", "Error\n", "Error en la memoria dinamica\n", "Error, el archivo de matrices esta mal hecho\n", "Error, el archivo esta corrupto o no existe\n"};
    printf("%s\n", s_errores[error]);
    return;
}


char ** matrizDsdArchivo(int n){
    error error=SIN_ERROR;
    int i, c, whitespace=0;
    char nombreArchivo[10];
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

