#ifndef _random_h
 
#define _random_h
 
// pone una semilla para empezar a crear numeros random
void randomize(void);
 
// devuelve un numero pseudo aleatorio entre [0,1)
double randNormalize(void);
// devuelve un numero pseudo aleatorio entre izq y der (entero)
int randInt(int, int);
 
// devuelve un numero pseudo aleatorio entre izq y der (real)
double randReal(double, double);
 
#endif
