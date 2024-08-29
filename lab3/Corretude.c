#include <stdio.h>
#include <stdlib.h>
typedef struct 
{
    int linhas;
    int colunas;
    int comum;
} args_M;

int sequencial(float *m1, float *m2,args_M arg,char *nome) //Linhas da primeira, colunas da segunda
{
    FILE *resultado;
    int linhas_m1 = arg.linhas, colunas_m2 = arg.colunas;
    resultado = fopen(nome,"wb+");
    if(!resultado)
    {
        puts("Não foi possivel abrir o arquivo");
        return 1;
    }
    fwrite(&linhas_m1,sizeof(int),1,resultado);
    fwrite(&colunas_m2,sizeof(int),1,resultado);
    float soma;
    for(int i=0;i<colunas_m2;i++)
    {
        for(int j=0;j<linhas_m1;j++)
        {
            soma = 0;
            for (int k=0; k<arg.comum;k++) // Comum = colunas de m1
            {
                soma+= m1[i*arg.comum+ k]*m2[k*colunas_m2 + j];
            }
            fwrite(&soma,sizeof(float),1,resultado); 
        }
    }
    fclose(resultado);
    return 0;
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
    matriz = (float *) malloc(tam * sizeof(float*));
    if(!matriz)
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

int main(int argc,char*argv[])
{
    float *matriz1, *matriz2;
    int linha_m1,coluna_m1,linha_m2,coluna_m2;
    args_M arg; // Pega os Argumentos necessários
    if(argc != 4)
    {
        printf("Escreva: %s <arquivo Binario Entrada 1> <Arquivo Binario Entrada 2> <Arquivo Binario Saida>\n",argv[0]);
        exit(4);
    }
    matriz1=le_matriz(argv[1],&linha_m1,&coluna_m1);
    matriz2=le_matriz(argv[2],&linha_m2,&coluna_m2);
    arg.linhas = linha_m1;
    arg.comum = coluna_m1;
    arg.colunas = coluna_m2;
    return sequencial(matriz1,matriz2,arg,argv[3]);
}