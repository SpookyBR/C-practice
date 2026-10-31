#include <stdio.h>

void geraBinario(int s[], int i, int n) {
    if (i == n) {
        for (int j = 0; j < n; j++) {
            printf("%d", s[j]);
        }
        printf("\n");
        return;
    }

    s[i] = 0;
    geraBinario(s, i + 1, n);

    s[i] = 1;
    geraBinario(s, i + 1, n);
}

int main() {
    int n = 4;
    int s[4]; // Vetor para armazenar a solução
    
    printf("Gerando binarios de tamanho %d:\n", n);
    geraBinario(s, 0, n);
    
    return 0;
}