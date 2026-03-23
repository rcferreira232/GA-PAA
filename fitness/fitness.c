// Danilo, Fernanda e Rafael


#include "fitness.h"

int calcular_aptidao(GraphData *grafo, int *cromossomo) {
    int aptidao = 0;

    for (int i = 0; i < grafo->num_vertices - 1; i++) {
        int de = cromossomo[i];
        int para = cromossomo[i + 1];
        aptidao += grafo->distance_matrix[de][para];
    }

    aptidao += grafo->distance_matrix[cromossomo[grafo->num_vertices - 1]][cromossomo[0]];

    return aptidao;
}