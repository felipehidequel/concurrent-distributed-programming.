#!/bin/bash

declare -a array=("10240000000")

declare -a p=("1" "2" "4")

output_file="execution_times.txt"
>$output_file

for N in "${p[@]}"; do
    # for P in "${p[@]}"; do
        # echo "testing with n=$N, p=$P" | tee -a $output_file
        echo "testing with n=10240000000, p=$N "| tee -a $output_file
        for i in {1..5}; do
            mpirun --use-hwthread-cpus --map-by core -np $N ./main.o 10240000000 >>$output_file
            wait
        done
        echo "" >>$output_file
    # done
done

echo "Testes concluídos! Resultados salvos em $output_file."
