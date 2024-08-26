#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define erro 0.000001

typedef struct
{
    int inicio;
    int fatia;
}args;

float *vetor1,*vetor2; //Variaveis globais pra não acessar a memória duas vezes

void* conc_dot(void* arg)
{
    double *sum = malloc(sizeof(double)); //Malloc pra salvar na pilha o endereço
    if (!sum) {
        perror("Erro ao alocar memória");
        pthread_exit(NULL);
    }
    *sum = 0;
    args argm = *(args*) arg;
    int inicio = argm.inicio;
    for(int i=0;i<argm.fatia;i++)
    {
        *sum+= (vetor1[inicio + i] * vetor2[inicio + i]);
    }
    free(arg);
    pthread_exit((void*) sum);
}

int verifica(double result,double gabarito)
{
    double var = (gabarito - result)/gabarito;
    printf("Variação relativa: %lf\n",var); // Se quiser printar o resultado
    if(var < erro && var > -erro) 
    {
        puts("Resultado bom!");
        return 0;
    }
    else 
    {
        puts("Resultado ruim");
        return 8;
    }
}

int main(int argc,char*argv[])
{
    int n_threads,tam,fatia,resto;
    FILE *arq;
    size_t ret;
    double result;
    if(argc != 3)
    {
        puts("Número de argumentos incorreto!");
        exit(1);
    }
    n_threads = atoi(argv[1]);
    if(n_threads < 1)
    {
        puts("Número de threads inválido");
        exit(2);
    }
    arq = fopen(argv[2],"rb");
    if(!arq)
    {
        puts("-- Erro de abertura de arquivo");
        exit(3);
    }

    //Pegando o tamanho dos vetores
    ret=fread(&tam,sizeof(int),1,arq);
    //Inicializando os vetores
    float vec1[tam],vec2[tam];
    ret = fread(vec1,sizeof(float),tam,arq);
    if(ret < tam)
    {
        puts("Erro de escrita no arquivo");
        exit(4);
    }
    ret = fread(vec2,sizeof(float),tam,arq);
    if(ret < tam)
    {
        puts("Erro de escrita no arquivo");
        exit(4);
    }
    //Pegando o resultado
    ret = fread(&result,sizeof(double),1,arq);
    fclose(arq); //Fechando o arquivo;

    vetor1 = vec1;
    vetor2 = vec2;

    if(tam > n_threads)
    {
        fatia = tam/n_threads;
        resto = tam%n_threads;
    }
    else
    {
        fatia = 1;
        n_threads = tam;
    }

    pthread_t tid_sistema[n_threads];
    args *arg;
    int inicio=0;
    //Lançando as threads
    for(int i=0; i<n_threads;i++)
    {
        arg = (args*) malloc(sizeof(args));
        if(!arg)
        {
            puts(" --Erro alocação dos argumentos");
            exit(5);
        }
        arg->inicio = inicio;
        if(i<resto)
        {
            arg->fatia = fatia+1;
            inicio+=fatia+1;
        }
        else
        {
            arg->fatia = fatia;
            inicio+=fatia;
        }
        if(pthread_create(&tid_sistema[i],NULL,conc_dot,(void*) arg))
        {
            puts("--ERRO: pthread_create()");
            exit(6);
        }
    }

    double soma=0,*parcela;
    //Esperando todas terminarem
    for(int i=0;i<n_threads;i++)
    {
        if(pthread_join(tid_sistema[i],(void **) &parcela))
        {
            puts("--ERRO: pthread_join()");
            exit(7);
        }
        soma+=*parcela;
    }
    free(parcela);
    //Calculo variação relativa
    return verifica(soma,result);
}