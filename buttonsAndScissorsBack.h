
int calcularMovPcEnDir(matriz_t tablero, punto_t pos, punto_t dir, char boton, int (*cond)(int), size_t dim, movimiento_t * mov_vec);

void limitesSubMatriz(punto_t p1, punto_t p2, int * minFil, int * maxFil, int * minCol, int * maxCol);

int esMovimientoValido(matriz_t tablero, punto_t pos, movimiento_t mov, punto_t dir, char boton);

movimiento_t * sobreescribir(movimiento_t * mov_vec, movimiento_t mov, size_t * dim);

movimiento_t * agregar(movimiento_t * mov_vec, movimiento_t mov, size_t * dim);

int realizarCorte(matriz_t * tablero, movimiento_t mov, punto_t dir);

void calcularDireccion(movimiento_t mov, punto_t * direccion);

int buscarBoton(matriz_t tablero, punto_t pos, char boton);

void calcularMovPc(matriz_t tablero, movimiento_t * mov);

int hayMovimientosValidos(matriz_t tablero);

int realizarCortePc(matriz_t * tablero);

void imprimirTablero(matriz_t tablero); //ES FRONT END, CORREGIR JE

int condMaxMov(int cantBotones);

int condMinMov(int cantBotones);