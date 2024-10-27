#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int ehPrimo(long long int n) {
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
return 1;
}
int main(int argc,char* argv[])
{
    long long int total = 0;
    for (long long j = 1;j<=(long long) atoll(argv[1]);j++)
    {
        total += ehPrimo(j);
    }
    printf("O total de números primos é: %lld",total);
    return 0;
}