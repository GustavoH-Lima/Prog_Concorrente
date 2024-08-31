log="LOG"
>"$log"


for i in $(seq 1 4);
do
    mul=$((500*i))
    #Gerar os vetores de teste
    ./gera $mul $mul m1
    ./gera $mul $mul m2
    #echo "Tempos sequenciais para matriz de tamanho $mul x $mul" >>"$log"
    ./mult_seq m1 m2 saida.out >> "$log" #Gabarito
    for j in $(seq 0 3);
    do
        n_threads=$(( 1 << $j ))
        echo "" >>"$log"
        echo "Tempos de execução para $n_threads threads e matriz dimensão $mul" >> "$log"
        for k in $(seq 1 5);
        do
            echo "">>"$log"
            ./conc m1 m2 saidac.out $n_threads >> "$log"
            diff saida.out saidac.out >> "$log"
        done
    done
    echo"">>"$log"
    rm -f m1
    rm -f m2
    rm -f saidac.out
    rm -f saida.out
done
echo "Teste terminado!"