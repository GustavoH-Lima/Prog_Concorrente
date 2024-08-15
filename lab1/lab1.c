#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#define TRUE 0
#define FALSE 1
#define BOOL int 
typedef struct
{
    int *vec;
    int fatia;
    int inicio;
}argm;

void print(int *vec,int N) //Função caso seja desejado imprimir o vetor
{
    for(int i=0;i<N;i++)
    {
     printf("%d ",vec[i]);
    } 
    puts("");
}

void inicializa(int *vec,int *controle,int N) //Função que inicializa o vetor a ser operado e o gabarito
{
    int num;
    srand(time(NULL));
    for(int i=0;i<N;i++)
    {
        num = rand()%100;
        vec[i] = num;
        controle[i] = num+1;
    }
}

void *Soma(void *arg) //Função que as threads irão executar
{
    argm *args = (argm*) arg;
    for(int i=args->inicio; i < args->inicio + args->fatia;i++)
    {
        args->vec[i] = args->vec[i]+1;
    }
    pthread_exit(NULL);
}

BOOL verifica(int *vec,int*resposta,int N) //Função para verificar o resultado
{
    for(int i=0;i<N;i++)
    {
        if(vec[i] != resposta[i]) 
        {
            puts("--ERRO: números diferentes no vetor");
            return FALSE;
        }
    }
    puts("--Tudo Certo");
    return TRUE;
}

int main(int argc , char *argv[])
{
    int N,M,fatia,inicio = 0,resto;
    argm *args;
    //Alguns tratamentos de erro
    if(argc < 3 )
    {
        puts("--ERRO: Numero insuficiente de argumentos!\n");
        return 2;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    if(M < 1)
    {
        puts("--ERRO: Número inválido de threads");
        return 3;
    }
    if(N < 1)
    {
        puts("--ERRO: Número inválido de Tamanho para o vetor");
        return 4;
    }
    
    pthread_t tid_sistema[M];
    int vec[N],gabarito[N];
    //Preparando os argumentos
    if(N<=M) // Se o número de threads for maior que o tamanho do vetor, cada thread faz 1 operação
    {
        fatia = 1;
        resto = 0;
        M=N; //Diminuo o número de threads pro tamanho do vetor
    }
    else //Caso contrário, cada uma faz N/M operações, com excessão das "resto" primeiras threads
    {
        fatia = N/M;
        resto = N-(fatia*M);
    }

    inicializa(vec,gabarito,N); //Inicializo os vetores com números aleatórios de 0-100, gabarito é o vetor que vai ser utilizado depois para a verificação

   for(int i=0; i<M; i++) 
   {
        //printf("--Cria a thread %d\n", i);
        //Alocando os argumentos
        args = malloc(sizeof(argm));
        if(args == NULL)
        {
            printf("--ERRO: malloc()\n");
            return 2;
        }
        //Passando os argumentos.
        args->vec = vec;
        args->inicio = inicio;
        if(i<resto) // Se o tamanho do vetor não for divisivel pelo número de threads, N-fatia*M threads executarão 1 elemento a mais
        {
            args->fatia = fatia + 1;
            inicio += fatia+1;
        }
        else
        {
            args->fatia = fatia;
            inicio+=fatia;
        }
        //Lançando a thread
        if (pthread_create(&tid_sistema[i], NULL, Soma, (void *) args)) 
        {
            printf("--ERRO: pthread_create()\n"); 
            return 2;
        }
   }
    for (int i=0; i<M; i++) // Espero todas as threads terminarem
    {
        if (pthread_join(tid_sistema[i], NULL)) 
        {
            printf("--ERRO: pthread_join() da thread %d\n", i); 
        } 
    }

    return verifica(vec,gabarito,N); // Faço a verificação e retorno tudo certo se ok, e erro se encontrar resposta diferente do gabarito
}