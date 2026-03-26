#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maxN 4
#define tamanho_populacao 30
#define geracoes 200
#define taxa_mut 0.1

int n, m;
int dist[maxN][maxN];

typedef struct 
{
    int rota[maxN];
    int custo;
} Individuo;

Individuo populacao[tamanho_populacao];
Individuo novaPop[tamanho_populacao];

// -------------------- FUNÇÃO CUSTO --------------------
int calcularCusto(int rota[]) 
{
    int custo = 0;

    for (int i = 0; i < n - 1; i++)
    {
        custo += dist[rota[i]][rota[i+1]];
    }

    // retorno para origem
    custo += dist[rota[n-1]][rota[0]];

    return custo;
}

// -------------------- GERAR INDIVÍDUO --------------------
void gerarIndividuo(Individuo *ind) 
{
    
    ind->rota[0] = 0; // fixa cidade 1 no início

    for (int i = 1; i < n; i++)
    {
        ind->rota[i] = i;

    }

    // embaralhar apenas de 1 até n-1
    for (int i = n-1; i > 1; i--) 
    {
        int j = 1 + rand() % i; // nunca mexe na posição 0
        int tmp = ind->rota[i];
        ind->rota[i] = ind->rota[j];
        ind->rota[j] = tmp;
    }

    ind->custo = calcularCusto(ind->rota);
}

// -------------------- SELEÇÃO (TORNEIO) --------------------
Individuo torneio() 
{
    int a = rand() % tamanho_populacao;
    int b = rand() % tamanho_populacao;

    if (populacao[a].custo < populacao[b].custo)
    {
        return populacao[a];
    }
    else
    {
        return populacao[b];
    }
}

// -------------------- CROSSOVER (OX) --------------------
void crossover(Individuo p1, Individuo p2, Individuo *filho) 
{
    int inicio = 1 + rand() % (n-1);
    int fim = 1 + rand() % (n-1);

    if (inicio > fim) 
    {
        int tmp = inicio;
        inicio = fim;
        fim = tmp;
    }

    int usado[maxN] = {0};

    filho->rota[0] = 0;
    usado[0] = 1;

    // copia trecho do pai1
    for (int i = inicio; i <= fim; i++) 
    {
        filho->rota[i] = p1.rota[i];
        usado[filho->rota[i]] = 1;
    }

    int j = 1;

    for (int i = 0; i < n; i++) 
    {
        if (!usado[p2.rota[i]]) 
        {
            while (j >= inicio && j <= fim)
            {
                j++;
            }

            filho->rota[j] = p2.rota[i];
            j++;
        }
    }

    filho->custo = calcularCusto(filho->rota);
}

// -------------------- MUTAÇÃO --------------------
void mutacao(Individuo *ind) 
{
    double r = (double) rand() / RAND_MAX;

    if (r < taxa_mut) 
    {
        int i = 1 + rand() % (n-1);
        int j = 1 + rand() % (n-1);

        // garantir que sejam diferentes
        while (i == j) 
        {
            j = 1 + rand() % (n-1);
        }

        // troca (swap)
        int tmp = ind->rota[i];
        ind->rota[i] = ind->rota[j];
        ind->rota[j] = tmp;

        ind->custo = calcularCusto(ind->rota);
    }
}

// -------------------- LEITURA DO ARQUIVO --------------------
void lerArquivo(char *nome) 
{
    FILE *f = fopen(nome, "r");

    if (!f) 
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    fscanf(f, "%d %d", &n, &m);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(f, "%d", &dist[i][j]);
        }
    }

    fclose(f);
}

// -------------------- IMPRESSÃO --------------------
void imprimirRota(Individuo melhor) 
{
    printf("Melhor rota:\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d ", melhor.rota[i] + 1);
    }

    printf("1\n"); // retorno
}

// -------------------- MAIN --------------------
int main() {
    srand(time(NULL));

    lerArquivo("entrada1.txt");

    // população inicial
    for (int i = 0; i < tamanho_populacao; i++)
    {
        gerarIndividuo(&populacao[i]);
    }

    // evolução
    for (int g = 0; g < geracoes; g++) 
    {
        for (int i = 0; i < tamanho_populacao; i++) 
        {
            Individuo p1 = torneio();
            Individuo p2 = torneio();

            crossover(p1, p2, &novaPop[i]);
            mutacao(&novaPop[i]);
        }

        // substitui população
        for (int i = 0; i < tamanho_populacao; i++)
        {
            populacao[i] = novaPop[i];
        }
    }

    // melhor solução
    Individuo melhor = populacao[0];
    for (int i = 1; i < tamanho_populacao; i++) 
    {
        if (populacao[i].custo < melhor.custo)
        {
            melhor = populacao[i];
        }
    }

    imprimirRota(melhor);

    printf("Custo: %d\n", melhor.custo);

    return 0;
}
