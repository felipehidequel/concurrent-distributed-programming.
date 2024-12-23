#!/bin/bash

# declare -a array=("10240000" "20480000" "40960000" "81920000J")

declare -a p=("1" "2" "4" "8" "16" "32")

output_file="execution_times.txt"
>$output_file

for P in "${p[@]}"; do
    echo "testing with n=$N, p=$P" | tee -a $output_file
    for i in {1..5}; do
        mpirun --use-hwthread-cpus --map-by core -np $P ./speed.o >>$output_file
        wait
    done
    echo "" >>$output_file
done

echo "Testes concluídos! Resultados salvos em $output_file."
