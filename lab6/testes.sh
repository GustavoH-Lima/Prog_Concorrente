gcc -o lab $1 list_int.c timer.h -Wall
total_tests=$2 # Número total de testes
progress=0
total=100  # Número total de "blocos" na barra de progresso (ajuste conforme necessário)
n_threads=$3  # Número de threads que você está utilizando para o teste
echo -n "Inicio dos testes: "
date | awk '{printf $5}'
echo ""
p1=$((total_tests/100))
for j in $(seq 1 $n_threads);
do
    echo "Início do teste com $j threads"
    for i in $(seq 0 $total_tests);
    do
        >log
        ./lab $j >>log
        if [ $? != 0 ]; then #Se for diferente de 0, deu errado.
            exit 1 
        fi
        modulo=$((i % p1))  # Atualiza a barra de progresso a cada 100 iterações
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
    rm log
done
rm lab
echo "Todos os testes concluídos."
