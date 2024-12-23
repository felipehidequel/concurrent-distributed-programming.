import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Dados médios fornecidos
data = {
    "p": np.array([1, 2, 4, 8]),  # Número de processos
    "N": np.array([640000000, 1280000000, 5120000000, 10240000000]),  # Número de trapézios
    "T": np.array([
        [1.436927, 3.079102, 2.010693, 4.154027],  # p = 1
        [0.762086, 1.657926, 1.088632, 2.333555],  # p = 2
        [0.413428, 0.895481, 0.583419, 1.303487],  # p = 4
        [0.438046, 0.893335, 0.605147, 1.244554],  # p = 8
    ])
}

# Flatten dos dados para usar na regressão
p_values = np.repeat(data["p"], len(data["N"]))  # Repetir os valores de p
N_values = np.tile(data["N"], len(data["p"]))  # Repetir os valores de N
T_values = data["T"].flatten()  # Achatar a matriz T

# Definição da função modelo para ajuste
def execution_time_model(inputs, a, b):
    n, p = inputs
    return a * (n / p) + b * np.log2(p)

# Ajuste usando mínimos quadrados
params, covariance = curve_fit(execution_time_model, (N_values, p_values), T_values)
a, b = params

# Gerar tempos previstos com os parâmetros ajustados
T_predicted = execution_time_model((N_values, p_values), a, b)

# Cálculo do erro médio quadrático (RMSE)
rmse = np.sqrt(np.mean((T_values - T_predicted) ** 2))

# Impressão dos resultados
print("Parâmetros ajustados:")
print(f"a = {a:.6f}, b = {b:.6f}")
print(f"Erro médio quadrático (RMSE): {rmse:.6f}")

# Gráfico para comparar os tempos reais e previstos
plt.figure(figsize=(10, 6))
for i, p in enumerate(data["p"]):
    plt.plot(data["N"], data["T"][i], 'o-', label=f"Real (p={p})")
    plt.plot(data["N"], execution_time_model((data["N"], np.full(len(data["N"]), p)), a, b), '--', label=f"Previsto (p={p})")

plt.xlabel("N (número de trapézios)")
plt.ylabel("Tempo de execução (s)")
plt.title("Comparação entre tempos reais e previstos")
plt.legend()
plt.grid()
plt.show()