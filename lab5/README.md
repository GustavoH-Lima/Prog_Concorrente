compile o programa control.c com
"gcc -o nome control.c -Wall"

Execute-o com "./nome nthreads" com nthreads o número de threads desejada

o arquivo teste testa se o programa termina para 6 a 10 threads, para usá-lo sem modificações, compile com "$gcc -o lab -Wall", deve-se, também, ter um arquivo gabarito chamado "gab", para fazê-lo, execute o programa uma vez com a saída direcionada ao arquivo com "./lab n_threads >> gab"