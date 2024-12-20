#!/bin/bash

EXECUTABLE=$1

declare -a NS=("16000000" "32000000" "64000000" "128000000")
# declare -a PS=("1" "2" "4")
declare -a PS=("1" "2" "4" "8")

# Definir o arquivo de saída onde os tempos de execução serão armazenados
OUTPUT_FILE="execution_times.txt"

# Limpar o arquivo de saída antes de começar
>$OUTPUT_FILE

# Loop sobre os diferentes valores de N e P
for N in "${NS[@]}"; do
    for P in "${PS[@]}"; do
        echo "Testing with n=$N, p=$P..." | tee -a $OUTPUT_FILE
        for i in {1..5}; do
            # Passar "g" para gerar lista aleatória, seguido do tamanho global (N)
            mpiexec --use-hwthread-cpus -n $P $EXECUTABLE g $N >>$OUTPUT_FILE ; wait
        done
        echo "" >>$OUTPUT_FILE # Adicionar uma linha em branco após os testes para clareza
    done
done

# Mensagem de conclusão
echo "Testes concluídos! Resultados salvos em $OUTPUT_FILE."
