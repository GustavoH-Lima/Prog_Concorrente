#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define nthreads 3
#define N 10
#define bool int

sem_t entrada1,entrada2,saida1,saida2; //1,0,1,0
char buffer_1[N];
char buffer_2[2*N]; //Buffer 2 com o dobro do tamanho caso tenha sempre adição
int vezes=0;
bool pega_chunk(FILE *arq, char *buffer)
{
    char carac;
    for(int i = 0 ;i < N;i++)
    {
        carac = fgetc(arq);
        buffer[i] = carac;
        if(carac == EOF) // Se terminar de ler o arquivo, diz para acabar.
        {
            return 0;
        }
    }
    return 1;
}

bool processa(char *buffer_local,int *contador,char *buffer_dobro_local)
{
    int postos=0;
    int i;
    char carac;
    for(i=0; i<N;i++)
    {
        carac = buffer_local[i];
        if(carac == EOF)
        {
            buffer_dobro_local[2*N - 1] = carac; // Põe na última posição o EOF para terminar a última thread
            buffer_dobro_local[i+postos] = '\0'; //Coloca para imprimir
            return 0;
        }
        buffer_dobro_local[i+postos]=carac; //Copiando caracter a caracter
        (*contador)++; //Acrescentando o contador
        if((*contador) == (2*vezes+1) && vezes <= 10) //Adicionar o \n ao buffer a cada numero pedido
        {
            *contador = 0;
            vezes++;
            postos++;
            buffer_dobro_local[i+postos] = '\n';
        }
        else if(vezes >10 && ((*contador) == 10)) // Quando já foram impressos 10 '\n' começa a inserir a cada 10
        {
            *contador = 0;
            postos++;
            buffer_dobro_local[i+postos] = '\n';
        }
    }
    buffer_dobro_local[i+postos] = '\0'; //Sinaliza que a string terminou
    return 1;
}

void *t1(void *arg) //Sempre que lê e coloca no buffer, adiciona um post em entrada 2
{
    FILE *arq;
    char *arquivo = (char*) arg;
    char buffer_local[N];
    int sai; // Variavel para ver se vai sair do loop;
    if((arq = fopen(arquivo,"r")) == NULL)
    {
        puts("Não foi possível abrir o arquivo");
        exit(1);
    }
    do //Lê o arquivo
    {
        sai = pega_chunk(arq,buffer_local);
        sem_wait(&entrada1); //Começa com 1
        strcpy(buffer_1,buffer_local); //Se puder, coloca no buffer
        sem_post(&entrada2);
    }
    while(sai);
    fclose(arq);
    pthread_exit(NULL);
}

void *t2(void *arg) //Sempre que modifica e coloca no buffer, adiciona um post ao saida 1 e entrada1 ,sempre que lê vai decrementar o semaforo 1
{
    char buffer_local[N];
    char buffer_local_dobrada[2*N];
    int contador = 0;
    bool continua;
    do
    {
        sem_wait(&entrada2); //começa com 0
        //le
        
        strcpy(buffer_local,buffer_1); //Le buffer_1

        sem_post(&entrada1); //permite que o buffer seja usado de novo para escrita

        continua = processa(buffer_local,&contador,buffer_local_dobrada); //processa()

        sem_wait(&saida1); //começa com 1

        strcpy(buffer_2,buffer_local_dobrada); //escreve em buffer 2
        
        if(buffer_local_dobrada[2*N-1] == EOF) // Para resolver minha condição de parada das threads
        {
            buffer_2[2*N-1] = EOF; // Por algum motivo, o strcpy não estava copiando o EOF depois do \0, creio que por ser depois do \0, então eu forço isso com esse comando
        }

        sem_post(&saida2); // Libera pra printar na tela
    }
    while(continua);
    pthread_exit(NULL);
}

void *t3(void *arg)
{
    char buffer_local[N];
    bool continua = 1;
    do
    {
        // puts("Estou me bloqueando");
        sem_wait(&saida2); //começa com 0
        // puts("Estou me desbloqueando");
        if(buffer_2[2*N-1] == EOF) continua = 0;
        strcpy(buffer_local,buffer_2); //Pega o buffer

        sem_post(&saida1); //permite a escrever no buffer2 dnv 

        printf("%s",buffer_local); // Escreve na tela
    }
    while(continua);
    // puts("Thread 3 terminou");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t tid[nthreads];
    if(argc < 2)
    {
        puts("Entrada: ./<Nome do programa> <Arquivo de entrada>");
        exit(1);
    }
    
    sem_init(&entrada1,0,1);
    sem_init(&entrada2,0,0);
    sem_init(&saida1,0,1);
    sem_init(&saida2,0,0);


    if(pthread_create(&tid[0],NULL,t1,(void *) argv[1]))
    {
        puts("Erro ao criar thread 1!");
        exit(1);
    }

    if(pthread_create(&tid[1],NULL,t2,NULL))
    {
        puts("Erro ao criar a thread 2!");
        exit(1);
    }

    if(pthread_create(&tid[2],NULL,t3,NULL))
    {
        puts("Erro ao criar a thread 3!");
        exit(1);
    }

    for (int t=0; t<nthreads; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 
    sem_destroy(&entrada1);
    sem_destroy(&entrada2);
    sem_destroy(&saida1);
    sem_destroy(&saida2);
    return 0;
}
