// Projeto Fast Pizzaria 

//Integrantes:
// Miguel Bastos Dias Barbosa - 10736603

//Referencias:

//Uso de IA:
Main: https://claude.ai/share/2f8cb8cf-9153-49f4-9fc0-0296c5848c9b

#include <stdio.h>
#include <stdlib.h>

#define INFINITO 1000000000

void rota_mais_rapida(int n, int **w, int T[], int R[]) {
    int *E = (int *)malloc((n + 1) * sizeof(int));

    int restantes = n;
    int e, v;
    
    for (e = 1; e <= n; e++) {
        E[e] = 1;
        T[e] = INFINITO;
        R[e] = -1;
    }
    
    T[1] = 0;
    
    while(restantes > 0) {
        v = -1;
        for (e = 1; e <= n; e++) {
            if(E[e] && (v == -1 || T[e] < T[v])) {
                v = e;
            }
        }
        E[v] = 0;
        restantes--;
        
        if (T[v] == INFINITO) {
            break;
        }
        
        for (e = 1; e <= n; e++) {
            if(E[e] && w[v][e] > 0) {
                if (T[e] > T[v] + w[v][e]) {
                    T[e] = T[v] + w[v][e];
                    R[e] = v;
                }
            }
        }
    }
    free(E);
}

int main(int argc, char *argv[]) {
    char *nome = (argc > 1) ? argv[1] : "pizzaria.txt";
    FILE *f = fopen(nome, "r");
    
    if (f == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", nome);
        return 1;
    }
    
    int destino; // Vertice de chegada
    int n; // Total de vertices
    int a, b, t; // Variaveis para calcular a rota
    int i; // Contador de loops
    int tam; // tamanho de passos
    
    int **w; // matriz de adjacencia
    int *T, *R; // O tempo e a rota
    int *rota; // mostrar o caminho na saida
    
    if (fscanf(f, "%d", &destino) != 1 || fscanf(f, "%d", &n) != 1 || n < 1) {
        printf("erro: formato de arquivo invalido\n");
        fclose(f);
        return 1;
    }
    
    //Aloca memoria pra w
    w = (int **)calloc(n + 1, sizeof(int *));
    for (i = 0; i <= n; i++) {
        w[i] = (int *)calloc(n + 1, sizeof(int));
    }
    
    //Verifica se tem 0
    while (fscanf(f, "%d", &a) == 1 && a != 0) {
        
        if (fscanf(f, "%d %d", &b, &t) != 2) {
            break;
        }
        
        if (a >= 1 && a <= n && b >= 1 && b <= n && t > 0 ) {
            if (w[a][b] == 0 || t < w[a][b]) {
                w[a][b] = t;
            }
        }
    }
    fclose(f);
    
    //Verifica se a esquina consta no mapa
    if (destino < 1 || destino > n) {
        printf("erro: esquina de entrega %d nao existe no mapa \n", destino);
        return 1;
    }
    
    //Aloca memoria a T e R
    T = (int *)malloc((n + 1) * sizeof(int));
    R = (int *)malloc((n + 1) * sizeof(int));
    
    rota_mais_rapida(n, w, T, R);
    
    if (T[destino] == INFINITO) {
        printf("nao tem rota da esquina #1 ate a esquina #%d\n", destino);
    } else {
        rota = (int *)malloc((n + 1) * sizeof(int));
        tam = 0;
        
        for (i = destino; i != -1; i = R[i]) {
            rota[tam++] = i;
        }
        
        printf("rota até a esquina #%d:", destino);
        for (i = tam - 1; i >= 0; i--) {
            printf(" %d", rota[i]);
        }
        
        printf("\n");
        printf("tempo calculado para rota = %d min.\n", T[destino]);
        
        free(rota);
    }
    
    for (i = 0; i <= n; i++) {
        free(w[i]);
    }
    free(w);
    free(T);
    free(R);
    return 0;
}