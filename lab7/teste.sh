gcc -o lab lab.c -Wall
for k in $(seq 0 1000);
do
    modulo=$((k % 100))
    if [ $modulo -eq 0 ]; then
        percent=$((k * 100 / 1000))
        echo "$percent% completo"
    fi
    >log
    for i in $(seq 0 11);
    do
        j=entradas/arquivo$i
        ./lab $j >>log
    done
    rm log
done