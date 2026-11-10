#include <stdio.h>
#include <math.h>

int contar_digitos(long long n) {
    int cont = 0;
    while (n > 0) {
        cont++;
        n /= 10;
    }
    return cont;
}

long long karatsuba(long long x, long long y) {
    if (x < 10 || y < 10) {
        return x * y;
    }

    int n = contar_digitos(x);
    int n2 = contar_digitos(y);
    int tamanho_max = (n > n2) ? n : n2;

    int m = tamanho_max / 2;

    long long multiplicador = (long long)pow(10, m);
)
    long long x1 = x / multiplicador; // Metade esquerda de x
    long long x0 = x % multiplicador; // Metade direita de x
    long long y1 = y / multiplicador; // Metade esquerda de y
    long long y0 = y % multiplicador; // Metade direita de y

    long long z2 = karatsuba(x1, y1);                 // z2 = x1 * y1
    long long z0 = karatsuba(x0, y0);                 // z0 = x0 * y0
    long long z1 = karatsuba(x1 + x0, y1 + y0) - z2 - z0; // z1 = (x1+x0)*(y1+y0) - z2 - z0

    return z2 * (long long)pow(10, 2 * m) + z1 * multiplicador + z0;
}

int main() {
    long long num1 = 12345;
    long long num2 = 67890;

    printf("--- Teste do Algoritmo de Karatsuba ---\n");
    printf("Numero 1: %lld\n", num1);
    printf("Numero 2: %lld\n\n", num2);

    long long resultado_karatsuba = karatsuba(num1, num2);
    long long resultado_normal = num1 * num2;

    printf("Resultado Karatsuba: %lld\n", resultado_karatsuba);
    printf("Resultado Esperado:  %lld\n", resultado_normal);

    if (resultado_karatsuba == resultado_normal) {
        printf("\nSucesso! O algoritmo multiplicou corretamente.\n");
    } else {
        printf("\nErro na multiplicacao!\n");
    }

    return 0;
}