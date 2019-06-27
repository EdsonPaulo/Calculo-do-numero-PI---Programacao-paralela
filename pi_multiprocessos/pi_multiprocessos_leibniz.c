/*-------------------------------------------------------------------------------------------------------
	Autores: Edson Gregorio 
	Objectivo: Determinação do número de PI usando multiprocessos.
	Data: 12/06/2019;
	Efeito: Trabalho de Sistemas Operativos II
--------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define INTERVALOS 5000
#define TOTAL_DE_PROCESSOS 4

typedef struct 
{
    int inicio[TOTAL_DE_PROCESSOS]; //Intervalo de inicio dos processos
    int fim[TOTAL_DE_PROCESSOS];    //Intervalo de fim dos processos
    double s;
} DATA;

//Prototipo de Funcoes
//----------------------------------------------------------------------------------------------------------------------*/
double CalcularPI_Leibniz(int numDoProcesso, DATA *data);

/*---------------------------------------------------------------------------------------------------------------------------------------------
		FUNCAO PRINCIPAL
-----------------------------------------------------------------------------------------------------------------------------------------------*/
int main ()
{  
    system("clear");
    DATA data;
    clock_t ti, tf;
    double tempo_gasto;
    int Fd[2];  // Fd[0]->para leitura - Fd[1]->fpara escrita
    double pi = 0.0;
    data.s = 4;

    ti = clock(); // tempo de inicio


    if (pipe(Fd) < 0){  //cria pipe 
        fprintf(stderr, "Falha ao criar o Pipe!" ); 
        return -1; 
    }

    int numDoProcesso = -1;
    pid_t pidDoPai = getpid();
    
    for (int n = 0; n < TOTAL_DE_PROCESSOS; n++) ///Criando processos
    {
        if(getpid() == pidDoPai) //se for o processo pai
        { 
            fork(); //cria processo filho
            numDoProcesso++; //incrementa o numero do processo

            if(numDoProcesso == 0) {
                data.inicio[numDoProcesso] = 0;
                data.fim[numDoProcesso] = INTERVALOS/TOTAL_DE_PROCESSOS;
            }
            else {
                data.inicio[numDoProcesso] = data.fim[numDoProcesso-1];
                data.fim[numDoProcesso] = data.inicio[numDoProcesso] + INTERVALOS/TOTAL_DE_PROCESSOS;
            }
        }
    }
    
    if (getpid() == pidDoPai) //se for o processo pai
    { 
        double piDofilho;
        close(Fd[1]); //fecha pipe de escrita
        for (int n = 0; n < TOTAL_DE_PROCESSOS; n++) 
        {
            read(Fd[0], &piDofilho, __SIZEOF_DOUBLE__); //lê dados no pipe
            pi += piDofilho;
        }
        close(Fd[0]); //fecha pipe de leitura

        tf = clock();  // tempo de fim
        tempo_gasto = ((double) (tf - ti)) / CLOCKS_PER_SEC * 1000; // tempos total em milissegundos
        
        printf("\n\nValor estimado de pi = %1.50f\n\n" , pi);
        printf("Tempo gasto: %lf ms\n", tempo_gasto);
    }
    else //se for um processo filho
    {
        double piFilho = CalcularPI_Leibniz(numDoProcesso, &data);
       // printf("\nP%d (filho %d) - Valor de PI %f - Inicio: %d - Fim: %d\n", numDoProcesso, getpid(), piFilho, data.inicio[numDoProcesso], data.fim[numDoProcesso]);
        close(Fd[0]); //fecha pipe de leitura
        write(Fd[1], &piFilho, __SIZEOF_DOUBLE__); //escreve o valor de pi no pipe
        close(Fd[1]); //fecha pipe de escrita
    }

    return 0;
}

double CalcularPI_Leibniz(int numDoProcesso, DATA *data)
{   
    double pi = 0.0;
    for(int i = data->inicio[numDoProcesso]; i < data->fim[numDoProcesso]; i++){
        pi += data->s / (2*i + 1);
        data->s *= -1;
    }  
    return pi;
}