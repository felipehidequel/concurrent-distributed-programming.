#!/bin/bash

EXECUTABLE=$1

declare -a PS=("1" "2" "4" "8" "16" "32" "64")

# Definir o arquivo de saída onde os tempos de execução serão armazenados
OUTPUT_FILE="execution_times.txt"

# Limpar o arquivo de saída antes de começar
>$OUTPUT_FILE

# Loop sobre os diferentes valores de N e P
for P in "${PS[@]}"; do
    echo "Testing with p=$P..." | tee -a $OUTPUT_FILE
    for i in {1..5}; do
        # Passar "g" para gerar lista aleatória, seguido do tamanho global (N)
        mpiexec --use-hwthread-cpus --map-by core -n $P $EXECUTABLE g $N >>$OUTPUT_FILE
        wait
    done
    echo "" >>$OUTPUT_FILE # Adicionar uma linha em branco após os testes para clareza
done

# Mensagem de conclusão
echo "Testes concluídos! Resultados salvos em $OUTPUT_FILE."
