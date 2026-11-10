#include <stdio.h>

int lucroMaximo(int precos[], int n) {
    if (n <= 1) {
        return 0;
    }

    int menor_preco = precos[0];
    int lucro_maximo = 0;

    for (int i = 1; i < n; i++) {
        if (precos[i] < menor_preco) {
            menor_preco = precos[i];
        } else {
            int lucro_hoje = precos[i] - menor_preco;
                if (lucro_hoje > lucro_maximo) {
                    lucro_maximo = lucro_hoje;
                }
        }
    }
    return lucro_maximo;
}