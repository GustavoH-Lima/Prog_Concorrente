Laboratório 6 de programação concorrente

### Instruções de execução

O arquivo do lab é "plock.c", para executá-lo, digite "$ gcc -o nome list_int.c timer.h -Wall"

Se quiser usar o arquivo de testes, dê permissão à ele e execute-o como "$./testes.sh plock.c n_testes nthreads" onde "n_testes" é o número de testes que serão executados com cada número de threads e "nthreads" é o número de threads que serão testadas, o programa testará de 1 à nthreads.


### Análise da contemplação dos requisitos

Vamos analisar o cumprimento dos requisitos nos dois casos:

Leituras: Caso uma thread queira iniciar uma leitura, ela primeiro verificará se tem alguem querendo escrever, ou se tem alguem já escrevendo, caso uma das duas esteja acontecendo, ela se bloqueia e continuará se bloqueando até essas condições não serem mais verdade. Assim que ela consegue permissão para ler, ela sinaliza a chegada de mais um leitor, fazendo o incremento da variável "leitores" e faz a operação. Caso essa thread saia da leitura, ela irá decrementar a variável "leitura" sinalizando que parou de ler, e caso alguem esteja esperando para ler, e ela for a última a sair do arquivo, ela libera os arquivos pra escrita. Assim, as threads nunca irão ler se tem alguem escrevendo e liberarão a escrita assim que alguém pedir e todas saírem da leitura.

Escritas: Caso uma thread queira escrever, ela primeiro sinaliza que quer escrever aumentando a variável "quer escrever" e checa se há leitores, caso positivo, ela se bloqueia e espera todos saírem. Quando não há leitores lendo, ou porque todos saíram, ou porque não existia nenhum, ela verifica se já há alguém escrevendo, caso positivo, ela espera numa fila para escrever. caso contrário, ela sinaliza que está escrevendo incrementando a variável "escritores" e decrementa que "quer_escrever" pois ela já conseguiu a permissão e vai fazer a operação. Para sair da escrita, ela checa se há mais gente querendo ler, caso positivo, ela libera somente o próximo da fila, em caso negativo,  ela libera todas as threads para voltarem a fazer leitura.

Desse modo, todos os requisitos foram atendidos, as threads de escrita possuem prioridade, enquanto o padrão consumidor escritor foi atendido.