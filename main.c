#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "IO/file_reader.h"
#include "fitness/fitness.h"
#include "operators/selection.h"
#include "operators/crossover.h"
#include "operators/mutation.h"

static void copiar_cromossomo(int *destino, int *origem, int n) {
    memcpy(destino, origem, n * sizeof(int));
}

static void inicializar_populacao(int **populacao, int tamanho_populacao, int n) {
    for (int i = 0; i < tamanho_populacao; i++) {
        for (int j = 0; j < n; j++) {
            populacao[i][j] = j;
        }
        for (int j = n - 1; j > 0; j--) {
            int k = rand() % (j + 1);
            int tmp = populacao[i][j];
            populacao[i][j] = populacao[i][k];
            populacao[i][k] = tmp;
        }
    }
}

static void avaliar_populacao(GraphData *grafo, int **populacao, int tamanho_populacao, int *aptidao) {
    for (int i = 0; i < tamanho_populacao; i++) {
        aptidao[i] = calcular_aptidao(grafo, populacao[i]);
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    GraphData *grafo = read_file();
    if (!grafo) {
        return 1;
    }

    int n = grafo->num_vertices;
    if (n <= 1) {
        printf("Grafo inválido ou trivial (n <= 1).\n");
        free_graph_data();
        return 1;
    }

    int tamanho_populacao = 100;
    int max_geracoes = 1000;
    double taxa_crossover = 0.8;
    double taxa_mutacao = 0.12;
    int tamanho_torneio = 5;
    int elitismo = 1;

    int **populacao = malloc(tamanho_populacao * sizeof(int *));
    int **nova_populacao = malloc(tamanho_populacao * sizeof(int *));
    int *aptidao = malloc(tamanho_populacao * sizeof(int));
    int *nova_aptidao = malloc(tamanho_populacao * sizeof(int));

    for (int i = 0; i < tamanho_populacao; i++) {
        populacao[i] = malloc(n * sizeof(int));
        nova_populacao[i] = malloc(n * sizeof(int));
    }

    inicializar_populacao(populacao, tamanho_populacao, n);
    avaliar_populacao(grafo, populacao, tamanho_populacao, aptidao);

    int indice_melhor = 0;
    for (int i = 1; i < tamanho_populacao; i++) {
        if (aptidao[i] < aptidao[indice_melhor]) {
            indice_melhor = i;
        }
    }

    int *melhor_solucao = malloc(n * sizeof(int));
    int melhor_custo = aptidao[indice_melhor];
    copiar_cromossomo(melhor_solucao, populacao[indice_melhor], n);

    for (int ger = 0; ger < max_geracoes; ger++) {
        if (elitismo) {
            copiar_cromossomo(nova_populacao[0], populacao[indice_melhor], n);
            nova_aptidao[0] = aptidao[indice_melhor];
        }

        int inicio = elitismo ? 1 : 0;
        for (int i = inicio; i < tamanho_populacao; i++) {
            int pai1 = selecao_torneio(aptidao, tamanho_populacao, tamanho_torneio);
            int pai2 = selecao_torneio(aptidao, tamanho_populacao, tamanho_torneio);
            while (pai2 == pai1) {
                pai2 = selecao_torneio(aptidao, tamanho_populacao, tamanho_torneio);
            }

            if ((rand() / (double)RAND_MAX) < taxa_crossover) {
                crossover_orientado(populacao[pai1], populacao[pai2], nova_populacao[i], n);
            } else {
                copiar_cromossomo(nova_populacao[i], populacao[pai1], n);
            }

            mutacao_troca(nova_populacao[i], n, taxa_mutacao);
            nova_aptidao[i] = calcular_aptidao(grafo, nova_populacao[i]);

            if (nova_aptidao[i] < melhor_custo) {
                melhor_custo = nova_aptidao[i];
                copiar_cromossomo(melhor_solucao, nova_populacao[i], n);
            }
        }

        for (int i = 0; i < tamanho_populacao; i++) {
            copiar_cromossomo(populacao[i], nova_populacao[i], n);
            aptidao[i] = nova_aptidao[i];
        }

        int atual_melhor = 0;
        for (int i = 1; i < tamanho_populacao; i++) {
            if (aptidao[i] < aptidao[atual_melhor]) {
                atual_melhor = i;
            }
        }
        indice_melhor = atual_melhor;
    }

    // Exibir grafo como matriz de distâncias
    printf("\nGrafo (matriz de distancias):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", grafo->distance_matrix[i][j]);
        }
        printf("\n");
    }

    // Exibir caminho com setas
    printf("\nMelhor rota (com setas):\n");
    for (int i = 0; i < n; i++) {
        printf("%d -> ", melhor_solucao[i] + 1);
    }
    printf("%d\n", melhor_solucao[0] + 1);
    printf("Custo total: %d\n", melhor_custo);

    // Salvar em JSON para front-end
    FILE *saida_json = fopen("graph_output.json", "w");
    if (saida_json) {
        fprintf(saida_json, "{\n");
        fprintf(saida_json, "  \"n\": %d,\n", n);
        fprintf(saida_json, "  \"matriz\": [\n");
        for (int i = 0; i < n; i++) {
            fprintf(saida_json, "    [");
            for (int j = 0; j < n; j++) {
                fprintf(saida_json, "%d", grafo->distance_matrix[i][j]);
                if (j < n - 1) fprintf(saida_json, ", ");
            }
            fprintf(saida_json, "]%s\n", (i < n - 1) ? "," : "");
        }
        fprintf(saida_json, "  ],\n");
        fprintf(saida_json, "  \"melhor_rota\": [");
        for (int i = 0; i < n; i++) {
            fprintf(saida_json, "%d", melhor_solucao[i] + 1);
            if (i < n - 1) fprintf(saida_json, ", ");
        }
        fprintf(saida_json, "],\n");
        fprintf(saida_json, "  \"custo\": %d\n", melhor_custo);
        fprintf(saida_json, "}\n");
        fclose(saida_json);
        printf("Arquivo graph_output.json gravado para visualizacao front-end.\n");
    } else {
        printf("Erro ao gravar graph_output.json.\n");
    }

    for (int i = 0; i < tamanho_populacao; i++) {
        free(populacao[i]);
        free(nova_populacao[i]);
    }
    free(populacao);
    free(nova_populacao);
    free(aptidao);
    free(nova_aptidao);
    free(melhor_solucao);

    free_graph_data();

    return 0;
}
