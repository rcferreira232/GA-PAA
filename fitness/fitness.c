#include "fitness.h"

int calculate_fitness(GraphData *graph, int *chromosome) {
    int fitness = 0;

    for (int i = 0; i < graph->num_vertices - 1; i++) {
        int from = chromosome[i];
        int to = chromosome[i + 1];
        fitness += graph->distance_matrix[from][to];
    }

    fitness += graph->distance_matrix[chromosome[graph->num_vertices - 1]][chromosome[0]];

    return fitness;
}