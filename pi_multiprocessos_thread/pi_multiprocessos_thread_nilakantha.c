/*-------------------------------------------------------------------------------------------------------
	Autores: Edson Gregorio 
	Objectivo: Determinação do número de PI usando multiprocessos.
	Data: 12/06/2019;
	Efeito: Trabalho de Sistemas Operativos II
--------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define INTERVALOS 5000
#define TOTAL_DE_PROCESSOS 4

typedef struct 
{
    int inicio[TOTAL_DE_PROCESSOS]; //Intervalo de inicio dos processos
    int fim[TOTAL_DE_PROCESSOS];    //Intervalo de fim dos processos
    double s;
} DATA;

//variaveis globais;
DATA data;
double pi;
pthread_mutex_t pi_lock;

void *calcularPI (void *arg)
{
    int numDoProcesso = (*((int *) arg));
    double piAux = 0.0;
    for(int i = data.inicio[numDoProcesso]; i < data.fim[numDoProcesso]; i++){
        piAux += data.s / ((2*i) * (2*i+1) * (2*i+2));
        data.s *= -1;
    }
    /*trava acesso a esta parte do código, altera pi, e
    destrava*/
    pthread_mutex_lock (&pi_lock);
    pi = piAux;
    pthread_mutex_unlock (&pi_lock);
    return (NULL);
}

void criarThreads(int *numDoProcesso)
{
    pthread_t thread1, thread2;
    void *valorRetorno;
   /* Inicializa a variável mutex*/
    pthread_mutex_init (&pi_lock, NULL);
    /* cria e executa duas threads */
    if(pthread_create(&thread1, NULL , calcularPI, numDoProcesso) || pthread_create(&thread2,NULL, calcularPI, numDoProcesso))
    {
        fprintf (stderr, "Falha na criacao das Threads!\n");
        exit (1);
    }
    /* Join espera as threads terminarem, o retorno é armazenado em valorRetorno */
    if(pthread_join (thread1, &valorRetorno) || pthread_join (thread2, &valorRetorno))
    {
        fprintf (stderr, "Falha no termino das Threads!\n");
        exit (1);
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------
		FUNCAO PRINCIPAL
-----------------------------------------------------------------------------------------------------------------------------------------------*/
int main (void)
{  
    int Fd[2];  // Fd[0]-> leitura - Fd[1]-> escrita
    pid_t pidDoPai = getpid();
    data.s = 4.0;
    clock_t ti, tf;
    double tempo_gasto;

    ti = clock(); // tempo de inicio

    if (pipe(Fd) < 0){   //cria pipe
        fprintf(stderr, "Falha ao criar o Pipe!" ); 
        return -1; 
    }

   //Criando processos
    int numDoProcesso = -1;
    for (int n = 0; n < TOTAL_DE_PROCESSOS; n++) ///Criando processos
    {
        if(getpid() == pidDoPai){  //se for o processo pai
            fork(); //cria processo filho
            numDoProcesso++; //incrementa o numero do processo   
            if(numDoProcesso == 0) {
                data.inicio[numDoProcesso] = 1;
                data.fim[numDoProcesso] = INTERVALOS/TOTAL_DE_PROCESSOS + 1;
            }
            else {
                data.inicio[numDoProcesso] = data.fim[numDoProcesso-1];
                data.fim[numDoProcesso] = data.inicio[numDoProcesso] + INTERVALOS/TOTAL_DE_PROCESSOS;
            }        
        }
    }
    
    if (getpid() == pidDoPai) // Processo pai
    { 
        double piDofilho = 0.0, pi=3.0;
        numDoProcesso = 0;
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
    else //processo filho
    {
        criarThreads(&numDoProcesso);
    //    printf("\nP%d - PI = %f" ,numDoProcesso, pi);
        close(Fd[0]); //fecha pipe de leitura
        write(Fd[1], &pi, __SIZEOF_DOUBLE__); //escreve o valor de pi no pipe
        close(Fd[1]); //fecha pipe de escrita
    }
    return 0;
}
