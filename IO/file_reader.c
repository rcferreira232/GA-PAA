#include <stdio.h>
#include <stdlib.h>
#include "file_reader.h"

GraphData* graph_data = NULL;

GraphData* read_file() {    
    FILE *file = fopen("./IO/input.txt", "r");
    if (!file) {
        printf("Erro ao abrir o arquivo input.txt\n");
        return NULL;
    }

    graph_data = (GraphData*)malloc(sizeof(GraphData));
    
    fscanf(file, "%d %d", &graph_data->num_vertices, &graph_data->num_edges);
    
    // Aloca a matriz de distâncias
    graph_data->distance_matrix = (int**)malloc(graph_data->num_vertices * sizeof(int*));
    for (int i = 0; i < graph_data->num_vertices; i++) {
        graph_data->distance_matrix[i] = (int*)malloc(graph_data->num_vertices * sizeof(int));
    }

    // Lê a matriz de distâncias
    for (int i = 0; i < graph_data->num_vertices; i++) {
        for (int j = 0; j < graph_data->num_vertices; j++) {
            fscanf(file, "%d", &graph_data->distance_matrix[i][j]);
        }
    }
    
    fclose(file);
    return graph_data;
}

void free_graph_data() {
    if (graph_data) {
        for (int i = 0; i < graph_data->num_vertices; i++) {
            free(graph_data->distance_matrix[i]);
        }
        free(graph_data->distance_matrix);
        free(graph_data);
        graph_data = NULL;
    }
}