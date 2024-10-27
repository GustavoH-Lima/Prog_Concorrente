#!/bin/bash

# Configuração da barra de progresso
total_tests=$((7 * 6 * 100))  # 42.000 testes (7 números, 6 threads, 1000 repetições)
current_test=0

# Função para exibir a barra de progresso
print_progress() {
    local progress=$(( ($current_test * 100) / $total_tests ))
    echo -ne "Progresso: $progress% [$current_test/$total_tests]\r"
}

# Fazendo os gabaritos
mkdir -p gabaritos
num=1
gcc -o cor corretude.c -lm
for i in $(seq 1 7); do
    num=$(($num * 10))
    nome="gabarito$i"
    ./cor $num >gabaritos/$nome
done

# Testes para uma a 6 threads
for i in $(seq 1 6); do
    num=1
    for j in $(seq 1 7); do
        num=$(($num * 10))
        nome="gabaritos/gabarito$j"
        
        for k in $(seq 1 100); do
            go run atv5.go $num $i >log
            diff log $nome >>check

            # Incrementa o contador de testes e atualiza a barra de progresso
            current_test=$((current_test + 1))
            print_progress

            if [ -s check ]; then # Se check tiver algo escrito, deu errado
                echo -e "\nErro encontrado no teste $j com $i threads na execução $k!"
                exit 1
            fi
        done
    done
done

# Limpeza
rm -r gabaritos
rm log
rm check
rm cor
echo -e "\nTodos os testes concluídos com sucesso!"
