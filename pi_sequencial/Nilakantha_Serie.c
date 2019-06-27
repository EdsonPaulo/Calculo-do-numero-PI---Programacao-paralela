#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTERVALOS 5000

int main() {    
    clock_t ti, tf;
    double tempo_gasto;
    
    ti = clock(); // tempo de inicio

    double s = 4, pi = 3;
    for(double i = 1; i <= INTERVALOS; i++){
        pi += s / ((2*i) * (2*i+1) * (2*i+2));
        s *= -1;
    }

    tf = clock();  // tempo de fim
    tempo_gasto = ((double) (tf - ti)) / CLOCKS_PER_SEC * 1000; // tempos total em milissegundos    
    printf("\nValor aproximado de PI por Nilakantha = %lf\n", pi); 
    printf("Tempo gasto: %lf ms\n", tempo_gasto);

}
