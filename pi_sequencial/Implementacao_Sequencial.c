#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INTERVALOS 5000

double vieteSerie(){ 
    double numerador = sqrt(2), corrente = 1, pi;
    for(int i = 0; i < INTERVALOS; i++) {
        corrente *= numerador/2;
        numerador = sqrt(2 + numerador);
    }
    pi = 2/corrente;
    return pi;
}

double leibnizSerie(){
    double s = 4, pi = 0;
    for(int i = 0; i < INTERVALOS; i++){
        pi += s / (2*i + 1);
        s *= -1;
    }
    return pi;
}

double nilakanthaSerie(){   
    double s = 4, pi = 3;
    for(double i = 1; i <= INTERVALOS; i++){
        pi += s / ((2*i) * (2*i+1) * (2*i+2));
        s *= -1;
    }
    return pi;
}

int main() {
    double pi;
    clock_t ti, tf;
    double tempo_gasto;
    
    printf("\nCalculo aproximado do valor de PI:\n\n");

    ti = clock();
    pi = vieteSerie();
    tf = clock();
    tempo_gasto = ((double) (tf - ti))  / CLOCKS_PER_SEC * 1000;
    printf("\nValor aproximado de PI por Viete = %.32f\n", pi); 
    printf("Tempo gasto: %fms\n", tempo_gasto);

    ti = clock();
    pi = leibnizSerie();
    tf = clock();
    tempo_gasto = ((double) (tf - ti)) / CLOCKS_PER_SEC * 1000;
    printf("\nValor aproximado de PI por Leibniz = %.32f\n", pi); 
    printf("Tempo gasto: %fms\n", tempo_gasto);

    ti = clock();
    pi = nilakanthaSerie();
    tf = clock();
    tempo_gasto = ((double) (tf - ti)) / CLOCKS_PER_SEC * 1000;
    printf("\nValor aproximado de PI por Nilakanatha = %.32f\n", pi); 
    printf("Tempo gasto: %fms\n", tempo_gasto);
}