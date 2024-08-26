Compile o "gera.c" com "$gcc -o nome gera.c -Wall"

execute-o com "$./nome n arquivo" onde "n" é o tamanho do vetor e "arquivo" é o nome do arquivo binário de destino das informações, essas são :

tamanho do vetor;
vetor1;
vetor2;
produto interno;

Compile o "lab2.c" com "gcc -o nome2 lab2.c -Wall"

execute-o com "$./nome2 T arquivo" onde "T" é o número de threads desejado e "arquivo" é o arquivo binário gerado pelo programa "gera".

Execute o tester como "./tester.sh ./nome ./nome2 N" onde N é o numero de execuções desejada.

O resultado é avaliado com a variação relativa, se ela for menor do que 0.0001, o programa retorna como correto, caso contrário, retorna como erro.