#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
typedef struct 
{
    int linhas;
    int colunas;
    int comum;
} args_M;

float *sequencial(float *m1, float *m2,args_M arg) //Linhas da primeira, colunas da segunda
{
    float *resultado;
    int linhas_m1 = arg.linhas, colunas_m2 = arg.colunas,tam;
    tam = linhas_m1 * colunas_m2;
    resultado = (float *) malloc (tam * sizeof(float));
    if(!resultado)
    {
        puts("Não foi possivel alocar memória");
        exit(1);
    }
    float soma;
    for(int i=0;i<linhas_m1;i++)
    {
        for(int j=0;j<colunas_m2;j++)
        {
            soma = 0;
            for (int k=0; k<arg.comum;k++) // Comum = colunas de m1
            {
                soma+= m1[i*arg.comum+ k]*m2[k*colunas_m2 + j];
            }
            resultado[i*colunas_m2 + j] = soma;
        }
    }
    return resultado;
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
    matriz = (float *) malloc(tam * sizeof(float));
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
}

int main(int argc,char*argv[])
{
    float *matriz1, *matriz2,*resultado;
    int linha_m1,coluna_m1,linha_m2,coluna_m2;
    args_M arg; // Pega os Argumentos necessários
    struct timeval t1,t2;
    double ms;
    
    gettimeofday(&t1,NULL);  //Tomada de tempo
    if(argc != 4)
    {
        printf("Escreva: %s <arquivo Binario Entrada 1> <Arquivo Binario Entrada 2> <Arquivo Binario Saida>\n",argv[0]);
        exit(4);
    }
    matriz1=le_matriz(argv[1],&linha_m1,&coluna_m1);
    matriz2=le_matriz(argv[2],&linha_m2,&coluna_m2);
    if(coluna_m1 != linha_m2)
    {
        printf("Erro na dimensão das matrizes, número de linhas da primeira diferente do número de colunas da segunda!\n");
        return 3;
    }
    arg.linhas = linha_m1;
    arg.comum = coluna_m1;
    arg.colunas = coluna_m2;
    gettimeofday(&t2,NULL); //Tomada de tempo
    ms = (t2.tv_usec - t1.tv_usec)/1000;
    ms += (t2.tv_sec - t1.tv_sec)*1000;
    printf("Tempo de execução de Entrada: %lf ms\n",ms);//Imprime tempo
    
    gettimeofday(&t1,NULL);  //Tomada de tempo

    resultado = sequencial(matriz1,matriz2,arg);
    
    gettimeofday(&t2,NULL); //Tomada de tempo
    ms = (t2.tv_usec - t1.tv_usec)/1000;
    ms += (t2.tv_sec - t1.tv_sec)*1000;
    printf("Tempo de execução de Processamento: %lf ms\n",ms);//Imprime tempo

    gettimeofday(&t1,NULL);  //Tomada de tempo
    
    escreve_arquivo(argv[3],resultado,linha_m1,coluna_m2);

    gettimeofday(&t2,NULL); //Tomada de tempo
    ms = (t2.tv_usec - t1.tv_usec)/1000;
    ms += (t2.tv_sec - t1.tv_sec)*1000;
    printf("Tempo de execução de Finalização: %lf ms\n",ms);//Imprime tempo
    return 0;
}