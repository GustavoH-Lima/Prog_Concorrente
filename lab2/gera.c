#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max 10000
int n;
double dot(float vec1[],float vec2[]) // Função que faz o produto interno dos vetores
{
    double result = 0;

    for(int i = 0;i<n;i++)
    {
        result+=vec1[i]*vec2[i];
    }
    return result;
}

void inicializa(float *vec)//Função para inicializar os vetores com números entre -999 e 999
{
    float num;
    for(int i=0;i<n;i++)
    {
        num = ((rand()%(2*max))-max)/100.;
        vec[i]=num;
    }
}

int main(int argc, char*argv[])
{
    double result;
    FILE *arq;
    size_t ret;
    srand(time(NULL));
    if(argc != 3)
    {
        puts("Numero de argumentos incorretos, preciso de tamanho dos vetores e arquivo de saída!");
        exit(1);
    }
    n=atoi(argv[1]);
    if(n<1)
    {
        puts("Tamanho de vetor inválido");
        exit(2);
    }
    float vec1[n],vec2[n];
    inicializa(vec1);
    inicializa(vec2);
    result = dot(vec1,vec2);
    // for(int i=0;i<n;i++)
    // {
    //     printf("%.2f %.2f\n",vec1[i],vec2[i]);
    // }
    // printf("%f\n",result); Printar os vetores e o resultado do produto interno

    arq = fopen(argv[2],"wb");
    if(!arq)
    {
        fprintf(stderr,"Erro de abertura de arquivo\n");
        return 1;
    }
    ret = fwrite(&n,sizeof(int),1,arq);
    ret = fwrite(vec1,sizeof(float),n,arq);
    if(ret<n)
    {
        fprintf(stderr,"Erro de escrita no arquivo!\n");
        return 2;
    }
    ret = fwrite(vec2,sizeof(float),n,arq);
    if(ret<n)
    {
        fprintf(stderr,"Erro de escrita no arquivo!\n");
        return 2;
    }
    ret = fwrite(&result,sizeof(double),1,arq);
    fclose(arq);
    return 0;
}