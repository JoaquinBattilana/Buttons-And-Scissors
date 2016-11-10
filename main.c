#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random.h"
#include <ctype.h>
#include "buttonsAndScissorsBack.h"

enum opciones {JVSJ=1, JVSCPU, CARGAR, SALIR};
enum ia {NOPC=0, PC};
enum errores {SIN_ERROR=0, ERROR, E_MEM_DIN,E_ARCHIVO_MATRICES,E_ABRIR_ARCHIVO, E_NOMBRE_ARCHIVO,
    E_CREAR_ARCHIVO, E_ARCHIVO_MAL,NO_SOBRESCRIBIR,COMANDO_INVALIDO,DISTINTO_COLOR,DIR_INVALIDA,ENTRE_BOTONES,VEC_NULL,MUY_LARGO,MISMO_BOT,FUERA_MATRIZ_1,FUERA_MATRIZ_2,NO_PRINT};

#define SAVEGAME 10
#define JUGADORES 2
#define BLOQUE 10
#define LIMPIAR_BUFFER() while(getchar()!='\n')

typedef struct{
    int botones_cortados;
    int esPC;
}jugador;

typedef struct tipoJuego{
    matriz_t tablero;
    int modoJuego;
    int turno;
} tipoJuego;


int menu();
int validar_dim();
int matrizDsdArchivo(tipoJuego * juego);
static int buscarMatriz(FILE * archivo, int random, int n);
static int escribirMatriz(FILE * archivo, char ** matriz, int n);
static char ** creaMatrizCuadrada(size_t n);
static void liberarMatrizCuadrada(matriz_t tablero);
void imprimirTablero(matriz_t tablero);
int cargarJuego(tipoJuego * juego);
int guardar_nombre(char * s);
int leer_movimiento(movimiento_t * mov, tipoJuego * juego);
static int leerArchivo(char * nombreArchivo, tipoJuego * juego);
void printError(int error);
int guardarJuego(char * nombreArchivo, tipoJuego * juego);
int validarMovimiento(movimiento_t * mov, matriz_t tablero);
int validar_volvermenu(void);
int jugar(tipoJuego * juego, jugador jugadores[JUGADORES]);

int main(void){
    printf("Bienvenido al juego de botones y tijeras, ingrese una opcion: \n");
    randomize();
    int opcion,salir=0,error=SIN_ERROR;
    tipoJuego juego;
    jugador jugadores[JUGADORES] = {};
    do{
        opcion=menu();
        switch(opcion){
            case JVSJ:
            case JVSCPU:
                juego.tablero.n= validar_dim();
                error = matrizDsdArchivo(&juego);
                juego.turno=(randInt(1,JUGADORES)); //hago que empiece uno random.
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

int jugar(tipoJuego * juego, jugador jugadores[JUGADORES]){
    int flag=0;
    punto_t direccion;
    movimiento_t mov;
    do{
        printf("Botones J1: %d, J2: %d\n", jugadores[0].botones_cortados, jugadores[1].botones_cortados);
        imprimirTablero(juego->tablero);
        printf("Turno jugador %d\n", juego->turno);
        switch(jugadores[juego->turno-1].esPC)
        {
            case NOPC:
                flag = leer_movimiento(&mov, juego);
                if(!flag) {
                    calcularDireccion(mov, &direccion);
                    jugadores[juego->turno-1].botones_cortados += realizarCorte(&(juego->tablero), mov, direccion);
                }
                break;
            case PC:
                jugadores[juego->turno-1].botones_cortados += realizarCortePc(&(juego->tablero));
                break;
        }
        if(!flag && !hayMovimientosValidos(juego->tablero))
        {
            printf("Gano el jugador %d\n", juego->turno);
            liberarMatrizCuadrada(juego->tablero);
            flag = validar_volvermenu();
        }
        else
            juego->turno = (juego->turno)%JUGADORES+1;
    }while (!flag);

    return flag; //lo puse para que no joda mas, REVISAR ESTO
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
            LIMPIAR_BUFFER();
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

int validar_volvermenu(void)
{
    int flag = 1;
    char ultimo_caracter;
    printf("¿Desea volver al menú o salir (M\\S)?\n");
    do{
        ultimo_caracter = '\0';
        if (getchar() == 'M' && (ultimo_caracter = getchar()) == '\n'){
            flag = 0;
        }
        else if (getchar() == 'S' && (ultimo_caracter = getchar()) == '\n'){
            flag = 0;
        }
        else{
            printf("Formato invalido");
        }
        if (ultimo_caracter != '\n')
        {
            LIMPIAR_BUFFER();
        }
    }
    while(flag);
    return !flag;
}

int matrizDsdArchivo(tipoJuego * juego){
    int error=SIN_ERROR;
    int c;
    char nombreArchivo[50], whitespace=0; //cambiar el por 10 en nombrearchivo
    sprintf(nombreArchivo, "/home/ubuntu/ClionProjects/TPE/%dx%d", juego->tablero.n,juego->tablero.n );//Cambie el path para probar en clion
    char ** aux=NULL;
    FILE * archivo;
    archivo = fopen(nombreArchivo, "r");
    if (archivo!=NULL) {
        fscanf(archivo, "%d%c", &c, &whitespace);
        if (whitespace != '\n') {
            error = E_ARCHIVO_MATRICES;
        }
        else {
            error = buscarMatriz(archivo,randInt(1,c), juego->tablero.n);
            if (!error) {
                aux = creaMatrizCuadrada(juego->tablero.n);
                if (aux == NULL)
                    error = E_MEM_DIN;
                else {
                    error = escribirMatriz(archivo, aux, juego->tablero.n);
                    if (error) {
                        error = E_ARCHIVO_MATRICES;
                        matriz_t matriz = {aux, juego->tablero.n};
                        liberarMatrizCuadrada(matriz);
                    }
                    else{
                        juego->tablero.v=aux;
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
        else if(lineas==n && c=='-' && ((c=fgetc(archivo))=='\n' || c==EOF)){
            random--;
            lineas = 0;
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


static char ** creaMatrizCuadrada(size_t n){
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

static void liberarMatrizCuadrada(matriz_t tablero){
    int i;
    for(i=0; i<tablero.n; i++)
        free(tablero.v[i]);
    free(tablero.v);
    return;
}

void imprimirTablero(matriz_t tablero) {
    printf("\n   ");
    int i, j;
    for(i = 0; i < tablero.n; i++) //referencia numerica para las columnas
        printf(" %d", i%10);
    i=0;
    printf("\n");
    while(i < tablero.n)
    {
        printf("\n");
        for(j = 0; j < tablero.n; j++)
        {
            if(j==0)
                i>9? printf("%d ", i) : printf("%d  ", i); //ref numerica para filas
            printf(" %c", tablero.v[i][j]);
        }
        if(j == tablero.n)
            i++;
    }

    printf("\n\n");
    return;
}

int guardar_nombre(char * s) {
    s = NULL;
    char * aux = NULL;
    int i, c, error = SIN_ERROR;
    for (i = 0; (c = getchar()) != '\n' && i < FILENAME_MAX + 1 && !error; i++) {
        if (i % BLOQUE == 0) {
            aux = realloc(s, (i + BLOQUE) * sizeof(char));
            if (aux == NULL) {
                error = E_MEM_DIN;
                free(s);
            }
            else
                s = aux;
        }
        s[i] = c;
    }
    if (!error && s == NULL)
    {
        error = VEC_NULL;
    }
    else
    {
        realloc(s, ++i);
        s[i] = '\0';
    }
    if(!error && i == FILENAME_MAX + 1 && c != '\n')
    {
        LIMPIAR_BUFFER();
        error = MUY_LARGO;
        free(s);
    }
    return  error;
}

int leer_movimiento(movimiento_t * mov, tipoJuego * juego)
{
    char flag_caracter, caracter, flag_salir = 0, comando[FILENAME_MAX+SAVEGAME+1];
    int flag_error, n;
    do
    {
        flag_error = NO_PRINT;
        n = 0;
        comando[FILENAME_MAX+SAVEGAME-1] = -1;
        printf("Ingreses el comando: ");
        fgets(comando, FILENAME_MAX+SAVEGAME+1, stdin);
        if(comando[FILENAME_MAX+SAVEGAME-1] != '\n' && comando[FILENAME_MAX+SAVEGAME-1] != -1)
            flag_error = COMANDO_INVALIDO;
        else{
            switch (comando[0]) {
                case '[':
                    n = sscanf(comando, "[%2d,%2d] [%2d,%2d]%c", &mov->origen.x, &mov->origen.y, &mov->destino.x,
                               &mov->destino.y,
                               &flag_caracter);
                    if (flag_caracter == '\n' && n == 5)
                        flag_error = validarMovimiento(mov, juego->tablero);
                    else {
                        flag_error = COMANDO_INVALIDO;
                    }
                    break;
                case 's':
                    n = sscanf(comando, "savegame%c", &flag_caracter);
                    if (n == 1 && flag_caracter == ' ') {
                        comando[strlen(comando)-1] = '\0';
                        flag_error = guardarJuego(comando + 9, juego);
                        if (flag_error == SIN_ERROR) {
                            flag_error = NO_PRINT;
                            printf("Se ha guardado exitosamente la partida\n");
                        }
                    }
                    else {
                        flag_error = COMANDO_INVALIDO;
                    }
                    break;
                case 'q':
                    n = sscanf(comando, "quit%c", &flag_caracter);
                    if (flag_caracter == '\n' && n == 1) {
                        printf("¿Está seguro que quiere salir (Y/N)? ");
                        if ((caracter = getchar()) == 'Y' && (flag_caracter = getchar()) == '\n') {
                            printf("¿Desea guardar la partida antes de salir (Y/N)? ");
                            if ((caracter = getchar()) == 'Y' && (flag_caracter = getchar()) == '\n') {
                                printf("Ingrese el nombre del archivo: ");
                                fgets(comando, FILENAME_MAX + 1, stdin);
                                comando[strlen(comando)-1] = '\0';
                                flag_error = guardarJuego(comando, juego);
                                if (flag_error == SIN_ERROR) {
                                    flag_error = NO_PRINT;
                                    printf("Se ha guardado exitosamente la partida\n");
                                }
                                flag_salir = 1;
                            } else if (caracter == 'N' && flag_caracter == '\n')
                                flag_salir = 1;                                            //sale
                            else {
                                flag_error = COMANDO_INVALIDO;
                            }
                        } else if (caracter == 'N' && flag_caracter == '\n')
                            flag_salir = 0;
                        else {
                            flag_error = COMANDO_INVALIDO;
                        }
                    }
                    break;
                default:
                    flag_error = COMANDO_INVALIDO;
            }
        }
        if(flag_error == FUERA_MATRIZ_1)
            printf("No existe la posición [%d,%d]\n", mov->origen.x, mov->origen.y);
        else if(flag_error == FUERA_MATRIZ_2)
            printf("No existe la posición [%d,%d]\n", mov->destino.x, mov->destino.y);
        else if(flag_error != NO_PRINT)
            printError(flag_error);
    }while (flag_error && !flag_salir);
    return flag_salir;
}


int validarMovimiento(movimiento_t * mov, matriz_t tablero)
{
    double pendiente;
    int flag = 0;
    if(mov->destino.x >= tablero.n || mov->destino.y >= tablero.n || mov->destino.x < 0 || mov->destino.y < 0) //fuera de matriz
        flag = FUERA_MATRIZ_2;
    else if (mov->origen.x >= tablero.n || mov->origen.y >= tablero.n || mov->origen.x < 0 || mov->origen.y < 0) //fuera de matriz
        flag = FUERA_MATRIZ_1;
    else if (mov->origen.x == mov->destino.x && mov->origen.y == mov->destino.y) //mismo punto
        flag = MISMO_BOT;
    else if (tablero.v[mov->origen.x][mov->origen.y] !=    tablero.v[mov->destino.x][mov->destino.y])
    flag = DISTINTO_COLOR;
    else if (mov->origen.x-mov->destino.x != 0)
    {
        pendiente = ((double)mov->destino.y - mov->origen.y)/(mov->destino.x - mov->origen.x);
        if (pendiente != 0 && pendiente != 1 && pendiente != -1)
        flag = DIR_INVALIDA;
    }
    else
    {
        punto_t direccion;
        calcularDireccion(*mov, &direccion);
        if(hayBtnsEntreMedio(tablero, *mov, direccion))
            flag = ENTRE_BOTONES;
    }

    return flag;
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
    int error=SIN_ERROR,i,j;
    FILE * archivo;
    archivo=fopen(nombreArchivo, "rb");
    c = fgetc(archivo);
    if (c == '0' || c == '1') {
        juego->modoJuego = c -'0';
        c = fgetc(archivo);
        if (c == '1' || c == '2') {
            juego->turno = c - '0';
            c = fgetc(archivo);
            if (c > '4' && c < '31') {
                juego->tablero.n = c - '0';
                juego->tablero.v = creaMatrizCuadrada(juego->tablero.n);
                if (juego->tablero.v != NULL) {
                    for (i = 0; i < juego->tablero.n && !error; i++) {
                        for (j = 0; j < juego->tablero.n && !error; j++) {
                            c = fgetc(archivo);
                            if (isalpha(c) || c == VACIO)
                                juego->tablero.v[i][j] = c;
                            else {
                                error = E_ARCHIVO_MAL;
                                liberarMatrizCuadrada(juego->tablero);
                            }
                        }
                    }
                } else
                    error = E_MEM_DIN;
            } else
                error = E_ARCHIVO_MAL;
        } else
            error = E_ARCHIVO_MAL;
    } else
        error = E_ARCHIVO_MAL;
    return error;
}

void printError(int error){
    char * s_errores[]={"No hay error\n", "Error\n", "Error en la memoria dinamica\n", "Error, el archivo de matrices esta mal hecho\n",
                        "Error, el archivo esta corrupto o no existe\n", "Error en el nombre de archivo \n",
                        "Error al crear/sobrescribir el archivo \n", "El archivo esta corrupto o mal escrito\n",
                        "Elija otro nombre\n", "Comando invalido\n", "El corte no tiene una única variedad de botones\n",
                        "Dirección invalida\n", "El corte no tiene una única variedad de botones\n", "El nombre del archivo no puede ser nulo\n",
                        "El nombre del archivo es muy largo\n", "El corte esta compuesto por solo un boton\n"};
        printf("%s", s_errores[error]);
    return;
}

int guardarJuego(char * nombreArchivo, tipoJuego * juego){
    int i,j;
    FILE * archivo;
    int error=SIN_ERROR;
    archivo = fopen(nombreArchivo, "wb");
    if (archivo!=NULL){
        fprintf(archivo,"%d%d%d", juego->modoJuego, juego->turno, juego->tablero.n);
        for(i=0;i<juego->tablero.n;i++){
            for(j=0;j<juego->tablero.n;j++)
                fputc(juego->tablero.v[i][j], archivo);
        }
    }
    else
        error=E_CREAR_ARCHIVO;
    if (!error)
        fclose(archivo);
    return error;
}