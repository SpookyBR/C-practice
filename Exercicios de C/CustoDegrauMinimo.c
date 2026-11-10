#include <stdio.h>

int min(int a, int b) {
    return (a < b) ? a : b;
}

int custoMinimo(int custo[], int n) {
    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return custo[0];
    }

    int dp[1000];
    dp[0] = custo[0];
    dp[1] = custo[1];

    for (int i = 2; i < n; i++) {
        dp[i] = custo[i] + min(dp[i - 1], dp[i - 2]);
    }

    return min(dp[n - 1], dp[n - 2]);
}
