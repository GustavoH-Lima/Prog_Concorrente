Compile o código geraMatrizBinario.c com 

"$gcc -o nome geraMatrizBinario.c -Wall"

e Execute com 

"$./nome Nlinhas Ncolunas Saida" onde Nlinhas é o número de linhas, Ncolunas o número de colunas e Saida o onde será impressa a saída

Já o Código Mult_Concorrente.c deve ser executado com

"$gcc -o nome2 Mult_Concorrente.c -Wall"

e Executado com

"$./nome2 M1 M2 NThreads Saida" onde M1 e M2 são as matrizes que serão multiplicadas, NThreads é o número de threads usada pelo programa e Saida é o arquivo onde a saída sera impressa

Se quiser usar o "teste.sh" sem mudar nada no código, nome deve ser "gera" e nome2 deve ser "conc" e executar o código de Corretude com "$gcc -o mult_seq Corretude.c -Wall"
