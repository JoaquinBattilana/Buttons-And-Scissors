#include "random.h"
#include <stdio.h>
#include <stdlib.h>

void crearTablero(int n, int cantColores)
{
    int offSet;
    putchar('-');
    printf("\n");
    int i,j;
    for(i=0,j=0; i<n && j<n;)
    {
        offSet = randInt(0,cantColores-1);
        printf("%c", 'A' + offSet);
        if(++j == n)
        {
            i++;
            j=0;
            printf("\n");
        }
    }
    return;
}

int main(void)
{
 randomize();
 int n;
 int cantColores;
 printf("\n\ndimension: ");
 scanf("%d", &n);
 printf("\ncantidad de colrcillos: ");
 scanf("%d", &cantColores);
 printf("\n\n");
 int i=0;
 for(i=0; i<5; i++)
 	crearTablero(n, cantColores);
 return 0;
}
