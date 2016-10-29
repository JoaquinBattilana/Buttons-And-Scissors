#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "random.h"
 
#define MAXMATRICESPORARCHIVO 5
enum errores {SIN_ERROR=0, ERROR, E_MEM_DIN,E_ARCHIVO_MATRICES,E_ABRIR_ARCHIVO};
typedef enum errores error;
 
static int buscarMatriz(FILE*, int n);
static int escribirMatriz(FILE*, char **, int);
static void liberaMatrizCuadrada(char **, int);
static char ** creaMatrizCuadrada(int);
char **matrizDsdArchivo(int);
void printError(int);
 
int main(void){
    int i,j;
    char ** m;
    char n=5;
    randomize();
    m = matrizDsdArchivo(n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%c", m[i][j]);
        }
        putchar('\n');
    }
    return SIN_ERROR;
}

void printError(int error){
    char * s_errores[]{"No hay error\n", "Error\n", "Error en la memoria dinamica\n", "Error, el archivo de matrices esta mal hecho\n", "Error, el archivo esta corrupto o no existe\n"};
    printf("%s\n", s_errores[error]);
    return;
    }
    
 
char ** matrizDsdArchivo(int n){
    error error=SIN_ERROR;
    int random, i, eof, c, validacion;
    char nombreArchivo[10];
    sprintf(nombreArchivo, "%dx%d", n,n );
    char pwd[10]="./";
    char ** aux=NULL;
    FILE * archivo;
    archivo = fopen(strcat(pwd, nombreArchivo), "r");
    if (archivo!=NULL){
        aux = creaMatrizCuadrada(n);
            if (aux==NULL)
                error=E_MEM_DIM;
            else{
                validacion = fscanf(archivo, "%d\n", &c);
                if ((c>MAXMATRICESPORARCHIVO || c<1 || !isdigit(c) && validacion == 2){
                    error = E_ARCHIVO_MATRICES;
                }
                else{
                    random=randInt(1,c);
                    eof = buscarMatriz(archivo, random);
                    if (eof)
                        error = E_ARCHIVO_MATRICES;
                    else{
                        eof = escribirMatriz(archivo, aux, n);
                        if(eof)
                            error = E_ARCHIVO_MATRICES;
                        }
 
                    }
                }
    }
    else
        error=E_ARCHIVO;
    if(error)
        printError(error);
    fclose(archivo);
    return aux;
}
 
static int escribirMatriz(FILE * archivo, char ** matriz, int n){
    int i,j,c;
    int error=SIN_ERROR;
    for(i=0; i<n && !error; i++){
        for(j=0; j<n; j++){
            c=fgetc(archivo);
            matriz[i][j]=c;
        }
        c=fgetc(archivo);
        if(c!='\n')
            error=ERROR;
    }
    return error;
}
 
static int buscarMatriz(FILE * archivo, int n){
    int flag=0, c;
    while(n!=1 && !(flag=feof(archivo))){
        c=fgetc(archivo);
        if (c=='-' && (c=fgetc(archivo))=='\n')
            n--;
        }
    return flag;
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
    
