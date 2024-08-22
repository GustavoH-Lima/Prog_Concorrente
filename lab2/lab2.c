#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct
{
    int inicio;
    float *vec1;
    float *vec2;
    float *fatia;
}args;

int main(int argc,char*argv[])
{
    int n_threads,tam;
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

    return 0;
}