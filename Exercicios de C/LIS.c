#include <stdio.h>
#include <stdlib.h>

static long chamadas = 0;

int lis_end_rec(int a[], int i) {
    chamadas++;
    int melhor = 1;

    for (int j = 0; j < i; j++) {
        if (a[j] < a[i]) {
            int t = 1 + lis_end_rec(a, j);
            if (t > melhor) melhor = t;
        }
    }
    return melhor;
}

int lis_recursiva(int a[], int n) {
    int melhor = 0;
    for (int i = 0; i < n; i++) {
        int t = lis_end_rec(a, i);
        if (t > melhor) melhor = t;
    }
    return melhor;
}

int lis_end_memo(int a[], int i, int memo[]) {
    chamadas++;
    if (memo[i] != 0) return memo[i];

    int melhor = 1;
    for (int j = 0; j < i; j++) {
        if (a[j] < a[i]) {
            int t = 1 + lis_end_memo(a, j, memo);
            if (t > melhor) melhor = t;
        }
    }
    memo[i] = melhor;
    return melhor;
}

int lis_memoization(int a[], int n) {
    int *memo = calloc(n, sizeof(int));
    if (memo == NULL) return -1;

    int melhor = 0;
    for (int i = 0; i < n; i++) {
        int t = lis_end_memo(a, i, memo);
        if (t > melhor) melhor = t;
    }

    free(memo);
    return melhor;
}

int lis_tabulation(int a[], int n) {
    int *dp = malloc(n * sizeof(int));
    if (dp == NULL) return -1;

    int melhor = 0;
    for (int i = 0; i < n; i++) {
        dp[i] = 1;
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i])
                dp[i] = dp[j] + 1;
        }
        if (dp[i] > melhor) melhor = dp[i];
    }

    free(dp);
    return melhor;
}

int main(void) {
    int a[] = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    int n = sizeof(a) / sizeof(a[0]);

    printf("Vetor: ");
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n\n");

    chamadas = 0;
    int r1 = lis_recursiva(a, n);
    printf("Recursiva     : LIS = %d  (%ld chamadas)\n", r1, chamadas);

    chamadas = 0;
    int r2 = lis_memoization(a, n);
    printf("Memoization   : LIS = %d  (%ld chamadas)\n", r2, chamadas);

    int r3 = lis_tabulation(a, n);
    printf("Tabulation    : LIS = %d  (sem recursão)\n", r3);

    return 0;
}