#include "buttonsAndScissorsBack.h"
#include "buttonsAndScissorsFront.h"

static int buscarMatriz(FILE * archivo, int random, size_t n);
static int escribirMatriz(FILE * archivo, char ** matriz, size_t n);
static char ** creaMatrizCuadrada(size_t n);
static void liberarMatrizCuadrada(matriz_t tablero);
static int leerArchivo(char * nombreArchivo, tipoJuego * juego);

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

    return flag;
}


int menu()
{
    int flag=1;
    int respuesta;
    do
    {
        printf("Menu principal:\n");
        printf("1-Juego de dos jugadores\n");
        printf("2-Juego contra computadora\n");
        printf("3-Recuperar un juego grabado\n");
        printf("4-Terminar\n");
        printf("Opcion: ");
        respuesta=getchar();
        if (respuesta > '0' && respuesta <'5') {
            if (getchar() == '\n')
                flag = 0;
            else
                LIMPIAR_BUFFER();
        }
        else if (respuesta!='\n' && getchar() != '\n')
            LIMPIAR_BUFFER();
    } while (flag);

    return respuesta-'0';
}

size_t validar_dim(){
    size_t dim = 0;
    char ultimo_caracter;
    do{
        ultimo_caracter = '\0';
        printf("Ingrese la dimension del tablero (minimo 5 y maximo 30): ");
        scanf("%ld%c", &dim, &ultimo_caracter);
    }
    while(ultimo_caracter != '\n' || dim < 5 || dim > 30);
    return dim;
}

int validar_volvermenu(void)
{
    int flag = 1;
    int ultimo_caracter;
    printf("¿Desea volver al menu o salir (M\\S)?: ");
    do{
        ultimo_caracter = '\0';
        if (toupper(getchar()) == 'M' && (ultimo_caracter = getchar()) == '\n'){
            flag = 0;
        }
        else if (toupper(getchar()) == 'S' && (ultimo_caracter = getchar()) == '\n'){
            flag = 0;
        }
        else{
            printf("Formato invalido.");
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
    char nombreArchivo[10], whitespace=0;
    sprintf(nombreArchivo, "./%ldx%ld", juego->tablero.n,juego->tablero.n );
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

static int buscarMatriz(FILE * archivo, int random, size_t n){
    int error=SIN_ERROR, flag=0, lineas=0;
    int c;
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

static int escribirMatriz(FILE * archivo, char ** matriz, size_t n){
    int i,j;
    int c;
    int error=SIN_ERROR;
    for(i=0; i<n && !error; i++){
        for(j=0; j<n && !error; j++){
            c=fgetc(archivo);
            if(isalpha(c))
                matriz[i][j]=(char)c;
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
                i>9? printf("%d ", i) : printf("%d  ", i); //referencia numerica para filas
            printf(" %c", tablero.v[i][j]);
        }
        if(j == tablero.n)
            i++;
    }

    printf("\n\n");
    return;
}

int leer_movimiento(movimiento_t * mov, tipoJuego * juego)
{
    int flag_caracter, caracter;
    char flag_salir = 0, comando[FILENAME_MAX+SAVEGAME+1];
    int flag_error, n;
    do
    {
        flag_error = NO_PRINT;
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
                            putchar('\n');
                            printf("Se ha guardado exitosamente la partida.\n");
                            putchar('\n');
                        }
                    }
                    else {
                        flag_error = COMANDO_INVALIDO;
                    }
                    break;
                case 'q':
                    n = sscanf(comando, "quit%c", &flag_caracter);
                    if (flag_caracter == '\n' && n == 1) {
                        printf("¿Esta seguro que quiere salir (Y/N)? ");
                        if (toupper((caracter = getchar())) == 'Y' && (flag_caracter = getchar()) == '\n') {
                            printf("¿Desea guardar la partida antes de salir (Y/N)? ");
                            if (toupper((caracter = getchar())) == 'Y' && (flag_caracter = getchar()) == '\n') {
                                printf("Ingrese el nombre del archivo: ");
                                fgets(comando, FILENAME_MAX + 1, stdin);
                                comando[strlen(comando)-1] = '\0';
                                flag_error = guardarJuego(comando, juego);
                                if (flag_error == SIN_ERROR) {
                                    flag_error = NO_PRINT;
                                    printf("Se ha guardado exitosamente la partida\n");
                                }
                                flag_salir = 1;
                            } else if (toupper(caracter) == 'N' && flag_caracter == '\n')
                                flag_salir = 1;
                            else {
                                LIMPIAR_BUFFER()
                                    flag_error = COMANDO_INVALIDO;
                            }
                        } else if (toupper(caracter) == 'N' && flag_caracter == '\n')
                            flag_salir = 0;
                        else {
                            LIMPIAR_BUFFER()
                                flag_error = COMANDO_INVALIDO;
                        }
                    }
                    break;
                default:
                    flag_error = COMANDO_INVALIDO;
            }
        }
        if(flag_error == FUERA_MATRIZ_1)
            printf("No existe la posicion [%d,%d].\n", mov->origen.x, mov->origen.y);
        else if(flag_error == FUERA_MATRIZ_2)
            printf("No existe la posicion [%d,%d].\n", mov->destino.x, mov->destino.y);
        else if(flag_error != SIN_ERROR && flag_error != NO_PRINT)
            printError(flag_error);
    }while (flag_error && !flag_salir);
    return flag_salir;
}




int cargarJuego(tipoJuego * juego){
    int c=0;
    int i=0;
    int error=E_ABRIR_ARCHIVO;
    char * nombreArchivo=NULL;
    char * aux=NULL;
    printf("Ingrese el nombre del archivo a cargar: ");
    while((c=getchar())!='\n' && error!=E_MEM_DIN ){
        if (i%BLOQUE==0){
            aux = realloc(aux, sizeof(char)*(i+BLOQUE));
            if (aux==NULL)
                error=E_MEM_DIN;
            else {
                error=SIN_ERROR;
                nombreArchivo = aux;
            }
        }
        nombreArchivo[i]=(char)c;
        i++;
    }
    if(!error && i<FILENAME_MAX){
        nombreArchivo[i]=0;
        nombreArchivo=realloc(nombreArchivo, sizeof(char)*i);
        error = leerArchivo(nombreArchivo, juego);
        free(nombreArchivo);
    }
    else
        error=E_ABRIR_ARCHIVO;
    return error;
}

static int leerArchivo(char * nombreArchivo, tipoJuego * juego){
    int c;
    char o;
    int error=SIN_ERROR,i,j;
    FILE * archivo;
    archivo=fopen(nombreArchivo, "rb");
    if (archivo!=NULL) {
        fread(&c, sizeof(int), 1, archivo);
        if (c == 0 || c == 1) {
            juego->modoJuego = c;
            fread(&c, sizeof(int), 1, archivo);
            if (c == 1 || c == 2) {
                juego->turno = c;
                fread(&c, sizeof(int), 1, archivo);
                if (c > 4 && c < 31) {
                    juego->tablero.n = (size_t) c;
                    juego->tablero.v = creaMatrizCuadrada(juego->tablero.n);
                    if (juego->tablero.v != NULL) {
                        for (i = 0; i < juego->tablero.n && !error; i++) {
                            for (j = 0; j < juego->tablero.n && !error; j++) {
                                fread(&o, sizeof(char), 1, archivo);
                                if (isalpha(o))
                                    juego->tablero.v[i][j] = o;
                                else if(o=='0')
                                    juego->tablero.v[i][j] = VACIO;
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
    }
    else
        error=E_ABRIR_ARCHIVO;
    return error;
}

void printError(int error){
    char * s_errores[]={"No hay error.\n", "Error.\n", "Error en la memoria dinamica.\n", "Error, el archivo de matrices esta mal hecho.\n",
                        "Error, el archivo esta corrupto o no existe.\n", "Error al crear/sobrescribir el archivo. \n", "El archivo esta corrupto o mal escrito.\n",
                        "Elija otro nombre.\n", "Comando invalido.\n", "El corte no tiene una unica variedad de botones.\n",
                        "Direccion invalida.\n", "El corte no tiene una unica variedad de botones.\n", "El nombre del archivo no puede ser nulo.\n",
                        "El nombre del archivo es muy largo.\n", "El corte esta compuesto por solo un boton.\n"};
    putchar('\n');
    printf("%s", s_errores[error]);
    putchar('\n');
    return;
}

int guardarJuego(char * nombreArchivo, tipoJuego * juego){
    int i,j;
    char c='0';
    FILE * archivo;
    int error=SIN_ERROR;
    archivo = fopen(nombreArchivo, "wb");
    if (archivo!=NULL){
        fwrite(&(juego->modoJuego), sizeof(int), 1, archivo);
        fwrite(&(juego->turno), sizeof(int), 1, archivo);
        fwrite(&(juego->tablero.n), sizeof(int), 1, archivo);
        for(i=0;i<juego->tablero.n;i++){
            for(j=0; j<juego->tablero.n;j++){
                if(juego->tablero.v[i][j]==VACIO)
                    fwrite(&c, sizeof(char), 1, archivo);
                else
                    fwrite(&juego->tablero.v[i][j], sizeof(char), 1, archivo);

            }
        }
    }
    else
        error=E_CREAR_ARCHIVO;
    if (!error)
        fclose(archivo);
    return error;
}
