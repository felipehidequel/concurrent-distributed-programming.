#include "time.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double pi_monte_carlo(long long int lancamentos);
double pi_monte_carlo_parallel(long long int lancamentos, int thread_count);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf(
        "Usage: ./monte_carlo <número de lançamentos> <número de threads>\n");
    exit(1);
  }
  double pi;
  int n, thread_count;

  n = strtol(argv[1], NULL, 10);
  thread_count = strtol(argv[2], NULL, 10);

  pi = pi_monte_carlo(n);
  printf("Pi: %f\n", pi);
  pi = 0.0;
  pi = pi_monte_carlo_parallel(n, thread_count);
  printf("Pi: %f\n", pi);

  return 0;
}

double pi_monte_carlo_parallel(long long int lancamentos, int thread_count) {
  long long int qtd_no_circulo = 0;
  double x, y, distancia_quadrada;
#pragma omp parallel default(none)                                             \
    shared(lancamentos, qtd_no_circulo) private(x, y, distancia_quadrada)
  {
    unsigned rank = omp_get_thread_num() * time(NULL);
#pragma omp for reduction(+ : qtd_no_circulo)
    for (int lancamento = 0; lancamento < lancamentos; lancamento++) {
      x = (double)rand_r(&rank) / RAND_MAX * 2 - 1;
      y = (double)rand_r(&rank) / RAND_MAX * 2 - 1;
      distancia_quadrada = x * x + y * y;
      if (distancia_quadrada <= 1)
        qtd_no_circulo++;
    }
  }

  return 4 * qtd_no_circulo / ((double)lancamentos);
}

double pi_monte_carlo(long long int lancamentos) {
  int qtd_no_circulo = 0;
  double x, y, distancia_quadrada;
  for (int lancamento = 0; lancamento < lancamentos; lancamento++) {
    x = (double)rand() / RAND_MAX * 2 - 1;
    y = (double)rand() / RAND_MAX * 2 - 1;
    distancia_quadrada = x * x + y * y;
    if (distancia_quadrada <= 1)
      qtd_no_circulo++;
  }
  return 4 * qtd_no_circulo / ((double)lancamentos);
}