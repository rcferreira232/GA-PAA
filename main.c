#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 100
#define POP_SIZE 50
#define GERACOES 500
#define TAXA_MUT 0.1

int n, m;
int dist[MAXN][MAXN];

typedef struct {
    int rota[MAXN];
    int custo;
} Individuo;

Individuo populacao[POP_SIZE];
Individuo novaPop[POP_SIZE];

// -------------------- FUNÇÃO CUSTO --------------------
int calcularCusto(int rota[]) {
    int custo = 0;

    for (int i = 0; i < n - 1; i++)
        custo += dist[rota[i]][rota[i+1]];

    // retorno para origem
    custo += dist[rota[n-1]][rota[0]];

    return custo;
}

// -------------------- GERAR INDIVÍDUO --------------------
void gerarIndividuo(Individuo *ind) {
    for (int i = 0; i < n; i++)
        ind->rota[i] = i;

    // embaralhamento Fisher-Yates
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int tmp = ind->rota[i];
        ind->rota[i] = ind->rota[j];
        ind->rota[j] = tmp;
    }

    ind->custo = calcularCusto(ind->rota);
}

// -------------------- SELEÇÃO (TORNEIO) --------------------
Individuo torneio() {
    int a = rand() % POP_SIZE;
    int b = rand() % POP_SIZE;

    if (populacao[a].custo < populacao[b].custo)
        return populacao[a];
    else
        return populacao[b];
}

// -------------------- CROSSOVER (OX) --------------------
void crossover(Individuo p1, Individuo p2, Individuo *filho) {
    int inicio = rand() % n;
    int fim = rand() % n;

    if (inicio > fim) {
        int tmp = inicio;
        inicio = fim;
        fim = tmp;
    }

    int usado[MAXN] = {0};

    // copia trecho do pai1
    for (int i = inicio; i <= fim; i++) {
        filho->rota[i] = p1.rota[i];
        usado[filho->rota[i]] = 1;
    }

    // completa com pai2
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (!usado[p2.rota[i]]) {
            while (j >= inicio && j <= fim)
                j++;
            filho->rota[j++] = p2.rota[i];
        }
    }

    filho->custo = calcularCusto(filho->rota);
}

// -------------------- MUTAÇÃO --------------------
void mutacao(Individuo *ind) {
    double r = (double) rand() / RAND_MAX;

    if (r < TAXA_MUT) {
        int i = rand() % n;
        int j = rand() % n;
        while (j == i)
            j = rand() % n;

        int tmp = ind->rota[i];
        ind->rota[i] = ind->rota[j];
        ind->rota[j] = tmp;

        ind->custo = calcularCusto(ind->rota);
    }
}

// -------------------- LEITURA DO ARQUIVO --------------------
void lerArquivo(char *nome) {
    FILE *f = fopen(nome, "r");

    if (!f) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    fscanf(f, "%d %d", &n, &m);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fscanf(f, "%d", &dist[i][j]);

    fclose(f);
}

// -------------------- IMPRESSÃO CORRETA --------------------
void imprimirRota(Individuo melhor) {
    int pos;

    // encontrar onde está a cidade 1 (índice 0)
    for (int i = 0; i < n; i++) {
        if (melhor.rota[i] == 0) {
            pos = i;
            break;
        }
    }

    printf("Melhor rota:\n");

    // imprimir começando de 1
    for (int i = 0; i < n; i++) {
        int idx = (pos + i) % n;
        printf("%d ", melhor.rota[idx] + 1);
    }

    // voltar para origem
    printf("1\n");
}

// -------------------- MAIN --------------------
int main() {
    srand(time(NULL));

    lerArquivo("entrada.txt");

    // população inicial
    for (int i = 0; i < POP_SIZE; i++)
        gerarIndividuo(&populacao[i]);

    // evolução
    for (int g = 0; g < GERACOES; g++) {
        for (int i = 0; i < POP_SIZE; i++) {
            Individuo p1 = torneio();
            Individuo p2 = torneio();

            crossover(p1, p2, &novaPop[i]);
            mutacao(&novaPop[i]);
        }

        // substitui população
        for (int i = 0; i < POP_SIZE; i++)
            populacao[i] = novaPop[i];
    }

    // melhor solução
    Individuo melhor = populacao[0];
    for (int i = 1; i < POP_SIZE; i++) {
        if (populacao[i].custo < melhor.custo)
            melhor = populacao[i];
    }

    imprimirRota(melhor);

    printf("Custo: %d\n", melhor.custo);

    // Salvar em JSON para front-end
    FILE *saida_json = fopen("graph_output.json", "w");
    if (saida_json) {
        fprintf(saida_json, "{\n");
        fprintf(saida_json, "  \"n\": %d,\n", n);
        fprintf(saida_json, "  \"matriz\": [\n");
        for (int i = 0; i < n; i++) {
            fprintf(saida_json, "    [");
            for (int j = 0; j < n; j++) {
                fprintf(saida_json, "%d", dist[i][j]);
                if (j < n - 1) fprintf(saida_json, ", ");
            }
            fprintf(saida_json, "]%s\n", (i < n - 1) ? "," : "");
        }
        fprintf(saida_json, "  ],\n");
        fprintf(saida_json, "  \"melhor_rota\": [");
        // encontrar onde está a cidade 1 (índice 0)
        int pos;
        for (int i = 0; i < n; i++) {
            if (melhor.rota[i] == 0) {
                pos = i;
                break;
            }
        }
        // imprimir começando de 1
        for (int i = 0; i < n; i++) {
            int idx = (pos + i) % n;
            fprintf(saida_json, "%d", melhor.rota[idx] + 1);
            if (i < n - 1) fprintf(saida_json, ", ");
        }
        fprintf(saida_json, "],\n");
        fprintf(saida_json, "  \"custo\": %d\n", melhor.custo);
        fprintf(saida_json, "}\n");
        fclose(saida_json);
        printf("Arquivo graph_output.json gravado para visualizacao front-end.\n");
    } else {
        printf("Erro ao gravar graph_output.json.\n");
    }

    return 0;
}
