#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define INTERVALOS 5000

int main() 
{      
        clock_t ti, tf;
        double tempo_gasto;
        //FILE *file = fopen("leibniz.xls","w");

        ti = clock(); // tempo de inicio

        double s = 4, pi = 0;
        for(int i = 0; i < INTERVALOS; i++)
	{
                pi += s / (2*i + 1);
                s *= -1;
                //fprintf(file, "%d\t%f\n", i, pi);
        }
        //fclose(file);
        tf = clock();  // tempo de fim
        tempo_gasto = ((double) (tf - ti)) / CLOCKS_PER_SEC * 1000; // tempos total em milissegundos
        printf("\nValor aproximado de PI por Leibniz = %lf\n", pi); 
        printf("Tempo gasto: %lf ms\n", tempo_gasto);
}
