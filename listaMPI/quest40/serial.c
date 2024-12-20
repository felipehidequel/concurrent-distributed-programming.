#include <stdio.h>
#include <stdlib.h>

void read_vetor(int n, int *vetor);
void print_vetor(int n, int *vetor);

int main() {
  int *vetor;
  int *soma;
  int n;

  printf("Entre com o tamanho do vetor:\n");
  scanf("%d", &n);

  vetor = (int *)malloc(n * sizeof(int));
  soma = (int *)malloc(n * sizeof(int));

  read_vetor(n, vetor);

  int aux = 0;
  for (int i = 0; i < n; i++) {
    aux += vetor[i];
    soma[i] = aux;
  }

  printf("Vetor\n");
  print_vetor(n, vetor);
  printf("Somas parcias\n");
  print_vetor(n, soma);

  return 0;
}

void read_vetor(int n, int *vetor) {
  printf("Entre com %d elementos para o vetor:\n", n);
  int aux;
  for (int i = 0; i < n; i++) {
    scanf("%d", &aux);
    vetor[i] = aux;
  }
  printf("\n");
}

void print_vetor(int n, int *vetor) {
  for (int i = 0; i < n; i++) {
    printf("%d ", vetor[i]);
  }
  printf("\n");
}