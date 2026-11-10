#include <stdio.h>

void gerar_permutacoes(int v[], int s[], int usado[], int tamanho, int index) {
    if (index == tamanho) {
        for (int i = 0; i < tamanho; i++) {
            printf("%d ", s[i]);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        if (!usado[i]) {
            usado[i] = 1; 
            s[index] = v[i];
            
            gerar_permutacoes(v, s, usado, tamanho, index + 1);
            
            usado[i] = 0;
        }
    }
}

int main() {
    int v[3] = {1, 2, 3};
    int s[3];
    int usado[3] = {0, 0, 0};
    
    printf("--- Permutacao (v=3, s=3) ---\n");
    gerar_permutacoes(v, s, usado, 3, 0);
    
    return 0;
}