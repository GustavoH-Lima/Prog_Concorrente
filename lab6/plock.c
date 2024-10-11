#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"
#include <time.h>

#define QTDE_OPS 10000000 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;
//lock de exclusao mutua
pthread_mutex_t mutex;
pthread_cond_t f_leitura;
pthread_cond_t f_escrita;
pthread_cond_t f_escrita_interna;

// Variaveis de controle
int quer_escrever=0;
int leitores=0;
int escritores=0;

void entra_escrita()
{
    pthread_mutex_lock(&mutex);
    quer_escrever++;
    if(leitores != 0) //Se tiverem leitores, se bloqueia ver se da pra tirar, não dá pois se não tiverem leitores e tiver escritores, vai continuar e possivelmente dará erro
    {
         // printf("Há %d leitores lendo, irei me bloquear!\n",leitores);
        pthread_cond_wait(&f_escrita,&mutex);
    }
    if(escritores != 0) //Se tiverem escritores, se bloqueia em outra fila
    {
         // printf("Há um escritor, irei me bloquear\n");
        pthread_cond_wait(&f_escrita_interna,&mutex);
      //   printf("Estava na fila de escritores, restam %d\n",quer_escrever);
    }
    escritores++;
    quer_escrever--;
    pthread_mutex_unlock(&mutex);
}

void sai_escrita()
{
    pthread_mutex_lock(&mutex);
    escritores--;
    if(quer_escrever == 0) //Se ninguem mais quiser escrever, desbloqueia pra leitura
    {
         printf("Fui o último a escrever, irei liberar todas as leituras escritores = %d \n",escritores);
        pthread_cond_broadcast(&f_leitura);
    }
    else //Caso contrário, libera o próximo a escrever
    {
        pthread_cond_signal(&f_escrita_interna);
    }
   pthread_mutex_unlock(&mutex); // Libera o lock e continua a vida
}

void entra_leitura()
{
   pthread_mutex_lock(&mutex); // Pego o lock
   while(quer_escrever || escritores) //Caso alguém queira escrever ou esteja escrevendo, se bloqueia
   {
      printf("Há %d escritores querendo escrever e %d escrevendo, irei me bloquear!\n",quer_escrever,escritores);
      pthread_cond_wait(&f_leitura,&mutex);
   }
   leitores++;
   pthread_mutex_unlock(&mutex);
}

void sai_leitura()
{
   pthread_mutex_lock(&mutex);
   leitores--;
   if(quer_escrever != 0 && leitores == 0) // Se alguem quiser escrever e for o ultimo leitor, libera a escrita e segue a vida
   {
      printf("Fui o último a ler, liberando todas as escrituras numero de leitores = %d\n",leitores);
      pthread_cond_broadcast(&f_escrita);
   }
   pthread_mutex_unlock(&mutex);
}

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 
   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<98) 
      {
	      entra_leitura();   
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
         sai_leitura(); 
	      read++;
      } 
      else if(98<=op && op<99) 
      {
	      entra_escrita();     
         // puts("in2");
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
         // puts("out2");
	      sai_escrita();     
	      in++;
      }
      else if(99<=op)
      {
	      entra_escrita();     
         // puts("in3");
         // printf("leitores = %d\nquer escrever = %d\nescritores=%d\n",leitores,quer_escrever,escritores);
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	      // puts("out3");
         sai_escrita();     
	      out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%.3lf%% out=%.3lf%% read=%.3lf%%\n", id, in*100.*nthreads/QTDE_OPS, out*100.*nthreads/QTDE_OPS, read*100.*nthreads/QTDE_OPS);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   srand(time(NULL));
   // verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);
   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   
   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //inicializa a variavel mutex
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&f_escrita,NULL);
   pthread_cond_init(&f_escrita_interna,NULL);
   pthread_cond_init(&f_leitura,NULL);
   //tomada de tempo inicial
   GET_TIME(ini);
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o mutex
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&f_escrita);
   pthread_cond_destroy(&f_escrita_interna);
   pthread_cond_destroy(&f_leitura);
   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}  /* main */