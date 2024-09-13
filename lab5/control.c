/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define nvezes 20 //Número de vezes que a variavel soma vai ser impressa
#define multiplo 10 //Multiplo em que a variável vai ser impressa

long int soma = 0; //variavel compartilhada entre as threads
int bloqueado=0; //Variável de controle;
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
pthread_cond_t operadores;
pthread_cond_t imprime;
pthread_cond_t primeiro;
//funcao executada pelas threads


void *ExecutaTarefa (void *arg) {
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++) {     
     //--entrada na SC
     pthread_mutex_lock(&mutex);
     
     if(soma<multiplo*(nvezes) && soma%multiplo == 0) //Se for para imprimir o número
     {
      if(bloqueado == 0) // A primeira thread que chegar e ver essa condição, irá sinalizar para imprimir o número e esperar, ela sempre será desbloqueada por lança a thread que a desbloqueia antes de se bloquear
      { //Essa primeira thread também é especial pois como ela será a primeira a sair, então somente ela pode incrementar a soma para que as outras threads saiam do loop
        bloqueado++; //Fala para os outros somente esperarem ele alterar a soma
        pthread_cond_signal(&imprime); //Libera a impressão
        pthread_cond_wait(&primeiro,&mutex); //Se bloqueia
        pthread_cond_broadcast(&operadores); //Assim que liberado, libera todas as threads que estão esperando para verificar as condições
        bloqueado = 0;
      }
      else
      {
        while(soma%multiplo==0) //Threads que caírem aqui estarão esperando a primeira a modificar a variável soma
        {
          pthread_cond_wait(&operadores,&mutex);
        }
      }
     }
     //--SC (seção critica)
      soma++; //incrementa a variavel compartilhada 
     //--saida da SC
     pthread_mutex_unlock(&mutex);
  }
  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
  printf("Extra : esta executando...\n");

  int i=0;
  pthread_mutex_lock(&mutex);
  do
  {
    printf("soma = %ld\n",soma);
    pthread_cond_signal(&primeiro); // Libera o primeiro cara para modificar a soma
    pthread_cond_wait(&imprime,&mutex); // Se bloqueia logo em seguida
    i++;
  }
  while(i<nvezes);
  pthread_cond_signal(&primeiro); // Libera o primeiro uma última vez
  pthread_mutex_unlock(&mutex);

  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)

   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);
   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   //--inicilaiza o mutex (lock de exclusao mutua)
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&operadores,NULL);
   pthread_cond_init(&imprime,NULL);
   pthread_cond_init(&primeiro,NULL);
   //--cria as threads
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   //--cria thread de log
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   //--finaliza o mutex
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&imprime);
   pthread_cond_destroy(&operadores);
   pthread_cond_destroy(&primeiro);
   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}
