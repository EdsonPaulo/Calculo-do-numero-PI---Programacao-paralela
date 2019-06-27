#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INTERVALOS 5000

int main() { 
    clock_t ti, tf;
    double tempo_gasto;

    ti = clock(); // tempo de inicio

    double numerador = sqrt(2), corrente = 1, pi;
    for(int i = 0; i < INTERVALOS; i++) {
        corrente *= numerador/2;
        numerador = sqrt(2 + numerador);
    }
    pi = 2/corrente;

    tf = clock();  // tempo de fim
    tempo_gasto = ((double) (tf - ti)) / CLOCKS_PER_SEC * 1000; // tempos total em milissegundos
    printf("\nValor aproximado de PI por Viete = %lf\n", pi); 
    printf("Tempo gasto: %lf ms\n", tempo_gasto);

    
}
