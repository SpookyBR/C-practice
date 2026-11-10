#include <stdio.h>
#include <string.h>

void lcs(char *X, char *Y, int m, int n) {
    int L[m + 1][n + 1];

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                L[i][j] = 0;
                
            } else if (X[i - 1] == Y[j - 1]) {
                L[i][j] = L[i - 1][j - 1] + 1;
                
            } else {
                L[i][j] = (L[i - 1][j] > L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1];
            }
        }
    }

    int tamanho_lcs = L[m][n];
    char lcs_str[tamanho_lcs + 1];
    lcs_str[tamanho_lcs] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs_str[tamanho_lcs - 1] = X[i - 1];
            i--;
            j--;
            tamanho_lcs--;
        } else if (L[i - 1][j] > L[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("Comprimento da LCS: %d\n", L[m][n]);
    printf("Subsequência: %s\n", lcs_str);
}

int main() {
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";
    
    int m = strlen(X);
    int n = strlen(Y);
    
    lcs(X, Y, m, n);
    
    return 0;
}