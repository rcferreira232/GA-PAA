# Objetivo do Trabalho

Implementar (Valor 1,0) na Linguagem C a Metaheurística estudada no trabalho T1 para solucionar o Problema do Caixeiro Viajante Simétrico (PCVS), que é um dos mais conhecidos e estudados na área de Otimização Combinatória. Seja um grafo simples completo não-orientado com n vértices e m arestas, onde o custo dij de cada aresta (i, j) representa a distância da cidade i para a cidade j (1 ≤ i, j ≤ n). Considere ainda que dij = dji. Ou seja, a distância da cidade i para a cidade j é a mesma da cidade j para a i. Assim, o objetivo do PCVS é partir de um dos vértices (chamado de Origem), passar por todos os demais n-1 vértices, sem repeti-los, e retornar para o vértice Origem, fazendo isto com a menor distância (custo) possível. Repare que o vértice Origem é repetido somente duas vezes, na saída e na chegada, e que os demais vértices não permitem repetição.

## Importante

O algoritmo usado para solucionar o PCVS deve ser a Metaheurística estudada no trabalho T1, ou seja, o Algoritmo Genético (GA). O algoritmo deve ser implementado em Linguagem C.

O grupo tem que preparar um texto apresentando detalhadamente a estratégia que o algoritmo utilizou para resolver o problema (adicionando um exemplo da execução à escrita da explicação da estratégia) e como executar o algoritmo;

O algoritmo tem que ter como entrada um arquivo .txt que contém na primeira linha o número de vértices e o número de arestas do grafo (separados por espaço). As demais linhas armazenam a matriz distância D, onde o elemento dij armazena a distância da cidade i para a cidade j (1 ≤ i, j ≤ n). Veja o exemplo a seguir do arquivo .txt que representa o grafo G abaixo:

```
4 6
0 7 1 3
7 0 5 8
1 5 0 6
3 8 6 0
```

Ou seja - O grafo G tem 4 vértices e 6 arestas. A distância da cidade 1 para a cidade 2 é 7, da cidade 1 para a cidade 3 é 1, da cidade 1 para a cidade 4 é 3, da cidade 2 para a cidade 3 é 5, da cidade 2 para a cidade 4 é 8, e da cidade 3 para a cidade 4 é 6. Note que a distância da cidade 2 para a cidade 1 é 7, da cidade 3 para a cidade 1 é 1, da cidade 4 para a cidade 1 é 3, da cidade 3 para a cidade 2 é 5, da cidade 4 para a cidade 2 é 8, e da cidade 4 para a cidade 3 é 6. O resultado do algoritmo para o grafo G deve ser a melhor solução encontrada, ou seja, a melhor rota (sequência de vértices) e o custo total da rota.

Observação: O vértice Origem é o vértice 1.
