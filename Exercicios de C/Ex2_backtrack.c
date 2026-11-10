#include <stdio.h>

void geraPlacar(char s[], int i, int golsA, int golsB, int m, int n) {
    if (golsA == m && golsB == n) {
        s[i] = '\0';
        printf("%s\n", s);
        return;
    }

    if (golsA < m) {
        s[i] = 'A';
        geraPlacar(s, i + 1, golsA + 1, golsB, m, n);
    }

    if (golsB < n) {
        s[i] = 'B';
        geraPlacar(s, i + 1, golsA, golsB + 1, m, n);
    }
}

int main() {
    int m = 5, n = 1; 
    char s[m + n + 1];    
    
    geraPlacar(s, 0, 0, 0, m, n);
    return 0;
}