#ifndef FILE_READER_H
#define FILE_READER_H

typedef struct {
    int num_vertices;
    int num_edges;
    int **distance_matrix; 
} GraphData;

GraphData* read_file();
void free_graph_data();

#endif