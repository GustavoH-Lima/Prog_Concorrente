#!/bin/bash

total_tests=10000  # Número total de testes
progress=0
total=100  # Número total de "blocos" na barra de progresso (ajuste conforme necessário)
n_threads=10  # Número de threads que você está utilizando para o teste
echo -n "Inicio dos testes:"
date | awk '{printf $5}'
echo ""
for j in $(seq 1 $n_threads);
do
    echo "Início do teste com $j threads"
    for i in $(seq 0 $total_tests);
    do
        >resp
        ./lab $j >>resp
        diff resp gab >>check
        if [ -s check ]; then #Se check tiver algo escrito, deu errado
            exit 1 
        fi
        modulo=$((i % 100))  # Atualiza a barra de progresso a cada 100 iterações
        if [ $modulo -eq 0 ]; then
            # Calcula a porcentagem
            percent=$(( i * 100 / total_tests ))
            # Cria a barra de progresso
            bar=$(printf "%-${total}s" "#" | tr ' ' '#')
            bar_display=${bar:0:$(( percent / (100 / total) ))}
            # Exibe a barra de progresso
            printf "\r[%s] %d%% concluído" "$bar_display" "$percent"
        fi
    done
    echo -e -n "\nFim do teste com $j threads "
    date | awk '{printf $5}'
    echo ""
done

echo "Todos os testes concluídos."
