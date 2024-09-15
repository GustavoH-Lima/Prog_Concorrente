for j in $(seq 6 10);
do
    echo "Inicio teste com $j threads"
    for i in $(seq 1 10000);
    do
        ./lab $j
        modulo=$((i % 500))  # Aritmética no shell usando $(( ... ))
        if [ $modulo -eq 0 ]; then  # Espaços corretos ao redor do [ ]
            result=$(($i / 100))
            echo -n "$result% concluído "
            date | awk '{printf $5}'
            echo ""
        fi
    done
    echo "Fim teste com $j threads"
done
