// Danilo, Fernanda e Rafael

#include <stdlib.h>
#include "mutation.h"

void mutacao_troca(int *cromossomo, int n, double taxa_mutacao) {
    for (int i = 0; i < n; i++) {
        double r = rand() / (double)RAND_MAX;
        if (r < taxa_mutacao) {
            int j = rand() % n;
            int tmp = cromossomo[i];
            cromossomo[i] = cromossomo[j];
            cromossomo[j] = tmp;
        }
    }
}
