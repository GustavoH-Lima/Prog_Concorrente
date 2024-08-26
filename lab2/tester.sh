# Nome do arquivo de saída
arquivo_saida="saida.txt"

>"$arquivo_saida"

# Verifica se os programas foram passados como argumentos
if [ $# -ne 3 ]; then
    echo "Uso: $0 <programa_criacao_binario> <programa_operacoes> <Número de execuções>" > "$arquivo_saida"
    exit 1
fi

programa_criacao=$1
programa_operacoes=$2
arquivo_binario="arquivo.bin"
n_execucoes=$3

# Loop para executar os programas várias vezes
for i in $(seq 1 $n_execucoes); do
    echo "Execução $i de $n_execucoes" >> "$arquivo_saida"
    
    # Gera dois números aleatórios entre 1000 e 200000
    param1=$((RANDOM % 99 + 1))
    param2=$((RANDOM % 199001 + 1000))
    
    echo "Parâmetros de entrada: $param2, $arquivo_binario" >> "$arquivo_saida"

    # Executa o primeiro programa para criar o arquivo binário
    ./$programa_criacao $param2 $arquivo_binario

    # Verifica se o arquivo binário foi criado
    if [ ! -f $arquivo_binario ]; then
        echo "Erro: O arquivo binário não foi criado." >> "$arquivo_saida"
        exit 2
    fi

    echo "Parâmetros de entrada: $param1, $arquivo_binario" >> "$arquivo_saida"
    # Executa o segundo programa para fazer operações com o arquivo binário
    resultado=$(./$programa_operacoes $param1 $arquivo_binario)

    retorno_programa_operacoes=$?

    # Verifica o código de retorno do segundo programa
    if [ $retorno_programa_operacoes -ne 0 ]; then
        echo "Erro: O segundo programa retornou código $retorno_programa_operacoes. Encerrando o teste." >> "$arquivo_saida"
        exit 3
    fi
    # Exibe o resultado do segundo programa
    echo "Resultado do programa de operações: $resultado" >> "$arquivo_saida"
    
    # Limpa o arquivo binário para a próxima execução
    rm -f $arquivo_binario

    echo "--------------------" >> "$arquivo_saida"
done

echo "Teste completo." >> "$arquivo_saida"
