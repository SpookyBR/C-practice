#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF (INT_MAX / 2)   /* evita overflow ao somar 1 */

static int min(int a, int b) { return a < b ? a : b; }

/* Recursivo */
int troco_rec(int T, int M[], int n) {
    if (T == 0) return 0;
    if (T < 0)  return INF;

    int menor = INF;
    for (int i = 0; i < n; i++)
        menor = min(menor, troco_rec(T - M[i], M, n) + 1);
    return menor;
}

/* Top down */
int troco_memo(int T, int M[], int n, int memo[]) {
    if (T == 0) return 0;
    if (T < 0)  return INF;
    if (memo[T] != -1) return memo[T];   /* ja calculado */

    int menor = INF;
    for (int i = 0; i < n; i++)
        menor = min(menor, troco_memo(T - M[i], M, n, memo) + 1);
    return memo[T] = menor;
}

/* Bottom up */
int troco_tab(int T, int M[], int n) {
    int *troco = malloc((T + 1) * sizeof(int));
    if (!troco) return -1;

    troco[0] = 0;
    for (int i = 1; i <= T; i++)
        troco[i] = INF;

    for (int i = 1; i <= T; i++)
        for (int j = 0; j < n; j++)
            if (i >= M[j])
                troco[i] = min(troco[i], troco[i - M[j]] + 1);

    int res = troco[T];
    free(troco);
    return res;
}

static void mostra(const char *nome, int r) {
    if (r >= INF) printf("%-10s: impossivel dar o troco\n", nome);
    else          printf("%-10s: %d moedas\n", nome, r);
}

int main(void) {
    int n, T;

    printf("Quantidade de tipos de moedas: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;

    int *M = malloc(n * sizeof(int));
    printf("Valores das moedas: ");
    for (int i = 0; i < n; i++)
        if (scanf("%d", &M[i]) != 1 || M[i] <= 0) { free(M); return 1; }

    printf("Valor do troco T: ");
    if (scanf("%d", &T) != 1 || T < 0) { free(M); return 1; }

    int *memo = malloc((T + 1) * sizeof(int));
    for (int i = 0; i <= T; i++) memo[i] = -1;

    mostra("recursivo", troco_rec(T, M, n));
    mostra("top-down", troco_memo(T, M, n, memo));
    mostra("bottom-up", troco_tab(T, M, n));

    free(memo);
    free(M);
    return 0;
}