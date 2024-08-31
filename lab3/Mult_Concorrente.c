#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
typedef struct
{
    float *m1;
    float *m2;
    float *resultado;
    int fatia;
    int inicio;
    int colunas_m2;
    int comum; // Coluna de m1 e linhas de m2
}argumentos;

void *computa_linha(void* args)
{
    argumentos arg = *(argumentos*)args;

    float soma;
    for(int i=arg.inicio;i<arg.inicio+arg.fatia;i++) //Indo do inicio até inicio + fatia, ou seja, faz a sua parte
    {
        for(int j=0;j<arg.colunas_m2;j++)
        {
            soma = 0;
            for (int k=0; k<arg.comum;k++) // Comum = colunas de m1
            {
                soma+= arg.m1[i*arg.comum+ k]*arg.m2[k*arg.colunas_m2 + j];
            }
            arg.resultado[i*arg.colunas_m2 + j] = soma;
        }
    }
    free(args);
    pthread_exit(NULL);
}

float *le_matriz(char* nome, int *linhas , int *colunas)
{
    FILE *arq;
    size_t ret;
    int tam;
    float* matriz;
    arq = fopen(nome,"rb");
    if(!arq)
    {
        puts("Erro de abertura do arquivo");
        exit(1);
    }
    ret=fread(linhas,sizeof(int),1,arq);
    ret=fread(colunas,sizeof(int),1,arq);
    tam = *linhas * *colunas;
    matriz = (float *) malloc(tam * sizeof(float)); //Malloc para usar a matriz do tipo ponteiro
    if(!matriz) //Teste do malloc
    {
        puts("Erro de malloc");
        exit(2);
    }
    ret = fread(matriz,sizeof(float),tam,arq);
    if(ret<tam)
    {
        puts("--Erro de leitura do arquivo");
        exit(3);
    }
    fclose(arq);
    return matriz;
}

void escreve_arquivo(char *nome,float*matriz,int linhas_m1,int colunas_m2)
{
    size_t ret;
    FILE *arq = fopen(nome,"wb+");
    if(!arq)
    {
        printf("--Erro de abertura do arquivo %s\n",nome);
        exit(1);
    }
    fwrite(&linhas_m1,sizeof(int),1,arq);
    fwrite(&colunas_m2,sizeof(int),1,arq);

    int tam = linhas_m1 * colunas_m2;
    ret=fwrite(matriz,sizeof(float),tam,arq);
    if(ret < tam)
    {
        printf("Erro de escrita no arquivo %s\n",nome);
        exit(2);
    }
    fclose(arq);
}

int main(int argc, char *argv[])
{
    //Inicialização
    if(argc != 5)
    {
        printf("Entrada deve ser: %s <Arquivo_Binario_de_entrada_1> <Arquivo_Binario_de_entrada_2> <Arquivo_de_Saida> <Quantidade de threads>\n",argv[0]);
        return 1;
    }
    
    float *matriz1,*matriz2;
    int linhas_m1,linhas_m2,colunas_m1,colunas_m2;
    struct timeval t1,t2;
    long ms;

//Leitura da entrada
    gettimeofday(&t1,NULL);  //Tomada de tempo
    matriz1 = le_matriz(argv[1],&linhas_m1,&colunas_m1);
    matriz2 = le_matriz(argv[2],&linhas_m2,&colunas_m2);
    if(colunas_m1 != linhas_m2)
    {
        printf("Erro na dimensão das matrizes, número de linhas da primeira diferente do número de colunas da segunda!\n");
        return 3;
    }

    gettimeofday(&t2,NULL); //Tomada de tempo
    ms =(long) (t2.tv_usec - t1.tv_usec)/1000;
    ms += (t2.tv_sec - t1.tv_sec)*1000;

    printf("Entrada: %ld ms\n",ms);//Imprime tempo
    //printf(" %ld",ms);//Imprime tempo
    //Processamento (Tudo que envolve a concorrência)
    gettimeofday(&t1,NULL);  //Tomada de tempo
    argumentos *arg;
    int n_threads,fatia,resto,tam,inicio=0;
    float *resultado;
    n_threads = atoi(argv[4]);
    if(n_threads < 1)
    {
        printf("Número de threads inválido\n");
        return 2;
    }
    pthread_t tid_sistema[n_threads];
    tam = linhas_m1*colunas_m2;

    //Irei separar pelo número em comum, número de colunas da primeira / Número de linhas da segunda
    if(n_threads >= linhas_m1)
    {
        n_threads = linhas_m1; // Para não dar erro na hora de lançar as threads
        fatia = 1;
        resto = 0;
    }
    else
    {
        fatia= linhas_m1/n_threads;
        resto = linhas_m1%n_threads;
    }
    resultado = (float *) malloc(tam*sizeof(float)); // Vetor para armazenar o resultado
    if(!resultado)
    {
        printf("--ERRO não foi possível alocar memória para matriz\n");
        return 3;
    }

    for(int i=0;i<n_threads;i++)
    {
        arg = (argumentos*) malloc (sizeof(argumentos));
        if(!arg)
        {
            printf("Não foi possível alocar os argumentos\n");
            return 4;
        }
        
        arg->m1 = matriz1;
        arg->m2 = matriz2;
        arg->resultado = resultado;
        arg->inicio= inicio;
        arg->fatia = fatia;
        if(i<resto)
        {
            arg->fatia++;
        }
        inicio+=arg->fatia;
        arg->colunas_m2 = colunas_m2;
        arg->comum = colunas_m1;
        //Lançar a thread
        if(pthread_create(&tid_sistema[i],NULL,computa_linha,(void*)arg))
        {
            printf("--ERRO pthread_create()\n");
            return 5;
        }
    }
    for(int i=0;i<n_threads;i++)
    {
        if(pthread_join(tid_sistema[i],NULL))
        {
            printf("--ERRO pthread_join()\n");
            return 6;
        }
    }

    gettimeofday(&t2,NULL); //Tomada de tempo
    ms = (long)(t2.tv_usec - t1.tv_usec)/1000;
    ms += (t2.tv_sec - t1.tv_sec)*1000;
    //printf(" %ld",ms);//Imprime tempo
    printf("Processamento: %ld ms\n",ms);//Imprime tempo
    // Finalização (Escrita no Arquivo)
    gettimeofday(&t1,NULL);  //Tomada de tempo

    escreve_arquivo(argv[3],resultado,linhas_m1,colunas_m2);
    
    gettimeofday(&t2,NULL); //Tomada de tempo
    ms =(long) (t2.tv_usec - t1.tv_usec)/1000;
    ms += (t2.tv_sec - t1.tv_sec)*1000;
    printf("Finalização:%ld ms\n",ms);//Imprime tempo
    //printf(" %ld %d %d\n",ms,n_threads,linhas_m1);//Imprime tempo
    return 0;
}