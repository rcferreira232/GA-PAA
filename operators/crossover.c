#include <stdlib.h>
#include "crossover.h"

void crossover_orientado(int *pai1, int *pai2, int *filho, int n) {
    int *aux = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        filho[i] = -1;
        aux[i] = 0;
    }

    int corte1 = rand() % n;
    int corte2 = rand() % n;
    if (corte1 > corte2) {
        int t = corte1;
        corte1 = corte2;
        corte2 = t;
    }

    for (int i = corte1; i <= corte2; i++) {
        filho[i] = pai1[i];
        aux[pai1[i]] = 1;
    }

    int posicao_escrita = (corte2 + 1) % n;
    for (int k = 0; k < n; k++) {
        int candidato = pai2[(corte2 + 1 + k) % n];
        if (!aux[candidato]) {
            filho[posicao_escrita] = candidato;
            posicao_escrita = (posicao_escrita + 1) % n;
        }
    }

    free(aux);
}
