#include <stdio.h>
#include "IO/file_reader.h"

int main() {    
    GraphData *graph = read_file();

    if (graph) {
        printf("Número de vértices: %d\n", graph->num_vertices);
        printf("Número de arestas: %d\n", graph->num_edges);
        printf("Matriz de distâncias:\n");
        for (int i = 0; i < graph->num_vertices; i++) {
            for (int j = 0; j < graph->num_vertices; j++) {
                printf("%d ", graph->distance_matrix[i][j]);
            }
            printf("\n");
        }
    }

    free_graph_data();

    return 0;
}