#include <stdio.h>
#include <stdlib.h>

int main() {
  int ranks[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // Ranks disponíveis (0 a 7)
  int size = 8;                            // Número total de ranks
  int salto = 1;                           // Distância inicial entre os ranks
  int rank, parceiro;

  // Loop principal, que continua até que o salto seja maior ou igual ao número
  // de ranks
  while (salto < size) {
    parceiro = rank ^ salto; // Determina o parceiro de comunicação usando XOR

    if (parceiro < size) {
      if (rank < parceiro) {
        printf("Rank %d recebe o valor do parceiro %d\n", rank, parceiro);
        printf("Rank %d soma a sua soma_local com o valor recebido do "
               "parceiro %d\n",
               rank, parceiro);
      } else {
        printf("Rank %d envia o valor para o parceiro %d\n", rank, parceiro);
      }
    }

    salto <<= 1;
    printf("Salto agora é %d\n", salto);
  }

  return 0;
}
