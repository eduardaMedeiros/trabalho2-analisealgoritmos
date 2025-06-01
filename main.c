#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXV 100                // Número máximo de vértices
#define INF 1000000000          // Valor "infinito" para inicialização de distâncias

/*=============================================
   1. CAMINHO MÍNIMO - Dijkstra adaptado
=============================================*/

// Estrutura para representar uma aresta (vértice de destino e peso)
typedef struct {
    int v;
    int peso;
} Aresta;

// Estrutura para representar um vértice e sua lista de adjacentes
typedef struct {
    Aresta adj[MAXV];   // Lista de adjacência
    int grau;           // Grau do vértice (número de vizinhos)
} Vertice;

// Função para Dijkstra considerando pesos nos vértices
void dijkstra_com_peso_vertices(int n, Vertice grafo[], int peso_v[], int origem) {
    int dist[MAXV];             // Vetor de distâncias mínimas
    int visitado[MAXV] = {0};   // Vetor de vértice visitado ou não

    // Inicializa todas as distâncias como infinito
    for (int i = 0; i < n; i++) dist[i] = INF;
    
    // A distância até a origem é o peso do próprio vértice de origem
    dist[origem] = peso_v[origem];

    // Laço principal: para cada vértice
    for (int i = 0; i < n; i++) {
        int u = -1;

        // Encontra o vértice não visitado com a menor distância
        for (int j = 0; j < n; j++) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) u = j;
        }

        if (dist[u] == INF) break;   // Não há mais vértices alcançáveis

        visitado[u] = 1;             // Marca como visitado

        // Relaxa as arestas de u
        for (int j = 0; j < grafo[u].grau; j++) {
            int v = grafo[u].adj[j].v;       // Vértice vizinho
            int w = grafo[u].adj[j].peso;    // Peso da aresta

            // Relaxamento considerando também o peso do vértice de destino
            if (dist[u] + w + peso_v[v] < dist[v]) {
                dist[v] = dist[u] + w + peso_v[v];
            }
        }
    }

    // Imprime as distâncias mínimas calculadas
    printf("Caminho mínimo com peso de vértices:\n");
    for (int i = 0; i < n; i++) {
        printf("Distância até %d: %d\n", i + 1, dist[i]);
    }
    printf("\n");
}

/*=============================================
   2. ÁRVORE GERADORA MÍNIMA - Kruskal
=============================================*/

// Estrutura para representar uma aresta (para AGM)
typedef struct {
    int u, v;       // Vértices
    int peso;       // Peso da aresta
} ArestaAGM;

// Função de comparação para qsort (ordenar as arestas pelo peso)
int cmp_arestas(const void *a, const void *b) {
    return ((ArestaAGM *)a)->peso - ((ArestaAGM *)b)->peso;
}

// Função para encontrar o representante (com compressão de caminho)
int find(int pai[], int x) {
    if (pai[x] != x)
        pai[x] = find(pai, pai[x]);
    return pai[x];
}

// Função para unir dois conjuntos
void unite(int pai[], int x, int y) {
    pai[find(pai, x)] = find(pai, y);
}

// Implementação de Kruskal
void kruskal(int n, ArestaAGM arestas[], int m) {
    qsort(arestas, m, sizeof(ArestaAGM), cmp_arestas); // Ordena as arestas pelo peso

    int pai[MAXV];
    for (int i = 0; i < n; i++) pai[i] = i;    // Inicializa cada vértice como seu próprio representante

    int peso_total = 0;    // Peso total da AGM

    printf("Arestas da AGM:\n");
    for (int i = 0; i < m; i++) {
        // Se os vértices não estão no mesmo conjunto, inclui a aresta na AGM
        if (find(pai, arestas[i].u) != find(pai, arestas[i].v)) {
            unite(pai, arestas[i].u, arestas[i].v);    // Une os conjuntos
            peso_total += arestas[i].peso;             // Soma o peso
            printf("(%d - %d): %d\n", arestas[i].u + 1, arestas[i].v + 1, arestas[i].peso);
        }
    }

    printf("Peso total da AGM: %d\n\n", peso_total);
}

/*=============================================
   3. ORDENAÇÃO TOPOLOGICA - Algoritmo de Kahn
=============================================*/

// Função para executar a ordenação topológica usando o algoritmo de Kahn
void ordenacao_topologica(int n, Vertice grafo[]) {
    int grau_entrada[MAXV] = {0};     // Vetor para armazenar o grau de entrada de cada vértice
    int fila[MAXV];                   // Fila para os vértices com grau de entrada 0
    int ini = 0, fim = 0;             // Índices da fila
    int ordem_topologica[MAXV];      // Resultado da ordenação
    int k = 0;                        // Índice da ordenação final

    // Calcula o grau de entrada de cada vértice
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < grafo[u].grau; i++) {
            int v = grafo[u].adj[i].v;
            grau_entrada[v]++;
        }
    }

    // Insere os vértices com grau de entrada 0 na fila
    for (int i = 0; i < n; i++) {
        if (grau_entrada[i] == 0) {
            fila[fim++] = i;
        }
    }

    // Processa a fila
    while (ini < fim) {
        int u = fila[ini++];
        ordem_topologica[k++] = u;

        // Para cada vizinho, reduz o grau de entrada
        for (int i = 0; i < grafo[u].grau; i++) {
            int v = grafo[u].adj[i].v;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) {
                fila[fim++] = v;
            }
        }
    }

    // Verifica se foi possível fazer ordenação (grafo sem ciclos)
    if (k != n) {
        printf("O grafo possui ciclo. Não é possível ordenar topologicamente.\n\n");
    } else {
        printf("Ordenação Topológica:\n");
        for (int i = 0; i < k; i++) {
            printf("%d ", ordem_topologica[i] + 1);  // +1 para exibir a partir de 1
        }
        printf("\n\n");
    }
}

/*=============================================
   MAIN - Testes para cada implementação
=============================================*/

int main() {
    /*---------------------------
      Teste 1: Caminho Mínimo
    ---------------------------*/
    Vertice grafo_cm[MAXV];
    for (int i = 0; i < MAXV; i++) grafo_cm[i].grau = 0;

    int n_cm = 4;                  // Número de vértices
    int peso_v[] = {1, 2, 3, 4};   // Peso de cada vértice

    // Adiciona as arestas ao grafo
    grafo_cm[0].adj[grafo_cm[0].grau++] = (Aresta){1, 1};
    grafo_cm[1].adj[grafo_cm[1].grau++] = (Aresta){0, 1};

    grafo_cm[1].adj[grafo_cm[1].grau++] = (Aresta){2, 2};
    grafo_cm[2].adj[grafo_cm[2].grau++] = (Aresta){1, 2};

    grafo_cm[2].adj[grafo_cm[2].grau++] = (Aresta){3, 3};
    grafo_cm[3].adj[grafo_cm[3].grau++] = (Aresta){2, 3};

    grafo_cm[0].adj[grafo_cm[0].grau++] = (Aresta){3, 10};
    grafo_cm[3].adj[grafo_cm[3].grau++] = (Aresta){0, 10};

    dijkstra_com_peso_vertices(n_cm, grafo_cm, peso_v, 0);

    /*---------------------------
      Teste 2: Árvore Geradora Mínima
    ---------------------------*/
    ArestaAGM arestas[] = {
        {0, 1, 1},
        {0, 2, 3},
        {1, 2, 1},
        {1, 3, 4},
        {2, 3, 2}
    };
    int n_agm = 4, m_agm = 5;   // Número de vértices e arestas

    kruskal(n_agm, arestas, m_agm);

    /*---------------------------
      Teste 3: Ordenação Topológica
    ---------------------------*/

    Vertice grafo_ot[MAXV];
    for (int i = 0; i < MAXV; i++) grafo_ot[i].grau = 0;

    int n_ot = 6; // Exemplo com 6 vértices
    grafo_ot[5].adj[grafo_ot[5].grau++] = (Aresta){2, 0};
    grafo_ot[5].adj[grafo_ot[5].grau++] = (Aresta){0, 0};
    grafo_ot[4].adj[grafo_ot[4].grau++] = (Aresta){0, 0};
    grafo_ot[4].adj[grafo_ot[4].grau++] = (Aresta){1, 0};
    grafo_ot[2].adj[grafo_ot[2].grau++] = (Aresta){3, 0};
    grafo_ot[3].adj[grafo_ot[3].grau++] = (Aresta){1, 0};

    ordenacao_topologica(n_ot, grafo_ot);
  
    return 0;
}
