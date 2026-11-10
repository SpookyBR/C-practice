#include <stdio.h>

void gerar_sequencias(int v[], int s[], int tamanho_v, int tamanho_s, int index) {
    if (index == tamanho_s) {
        for (int i = 0; i < tamanho_s; i++) {
            printf("%d ", s[i]);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < tamanho_v; i++) {
        s[index] = v[i];
        gerar_sequencias(v, s, tamanho_v, tamanho_s, index + 1);
    }
}

int main() {
    int v[3] = {1, 2, 3};
    int s[3];
    
    printf("--- Sequencias (v=3, s=3) ---\n");
    gerar_sequencias(v, s, 3, 3, 0);
    
    return 0;
}