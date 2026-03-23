// Danilo, Fernanda e Rafael

#include <stdlib.h>
#include "selection.h"

int selecao_torneio(int *aptidao, int tamanho_populacao, int tamanho_torneio) {
    int melhor = -1;
    for (int i = 0; i < tamanho_torneio; i++) {
        int idx = rand() % tamanho_populacao;
        if (melhor == -1 || aptidao[idx] < aptidao[melhor]) {
            melhor = idx;
        }
    }
    return melhor;
}
