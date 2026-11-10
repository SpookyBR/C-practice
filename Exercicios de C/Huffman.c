#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALFABETO 256
#define ASSINATURA "HUF1"
#define TAM_BUFFER 65536

typedef struct No {
    unsigned char caractere;
    uint64_t freq;
    struct No *esq, *dir;
} No;

typedef struct {
    No *dados[ALFABETO];
    int tamanho;
} MinHeap;

static No *novo_no(unsigned char c, uint64_t freq, No *esq, No *dir) {
    No *n = malloc(sizeof(No));
    if (!n) {
        perror("malloc");
        exit(1);
    }
    n->caractere = c;
    n->freq = freq;
    n->esq = esq;
    n->dir = dir;
    return n;
}

static int eh_folha(const No *n) {
    return n->esq == NULL && n->dir == NULL;
}

static void liberar_arvore(No *n) {
    if (!n) return;
    liberar_arvore(n->esq);
    liberar_arvore(n->dir);
    free(n);
}

static void trocar(No **a, No **b) {
    No *t = *a;
    *a = *b;
    *b = t;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai]->freq <= h->dados[i]->freq) break;
        trocar(&h->dados[pai], &h->dados[i]);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    for (;;) {
        int menor = i;
        int e = 2 * i + 1, d = 2 * i + 2;
        if (e < h->tamanho && h->dados[e]->freq < h->dados[menor]->freq) menor = e;
        if (d < h->tamanho && h->dados[d]->freq < h->dados[menor]->freq) menor = d;
        if (menor == i) break;
        trocar(&h->dados[i], &h->dados[menor]);
        i = menor;
    }
}

static void inserir(MinHeap *h, No *n) {
    h->dados[h->tamanho] = n;
    subir(h, h->tamanho);
    h->tamanho++;
}

static No *extrair_min(MinHeap *h) {
    No *min = h->dados[0];
    h->tamanho--;
    h->dados[0] = h->dados[h->tamanho];
    descer(h, 0);
    return min;
}

static No *construir_arvore(const uint64_t freq[ALFABETO]) {
    MinHeap h;
    h.tamanho = 0;

    for (int i = 0; i < ALFABETO; i++)
        if (freq[i] > 0)
            inserir(&h, novo_no((unsigned char)i, freq[i], NULL, NULL));

    if (h.tamanho == 0) return NULL;

    while (h.tamanho > 1) {
        No *a = extrair_min(&h);
        No *b = extrair_min(&h);
        inserir(&h, novo_no(0, a->freq + b->freq, a, b));
    }
    return extrair_min(&h);
}

static void gerar_codigos(const No *n, char *buffer, int prof,
                          char *codigos[ALFABETO]) {
    if (!n) return;

    if (eh_folha(n)) {
        if (prof == 0) {
            buffer[0] = '0';
            prof = 1;
        }
        buffer[prof] = '\0';
        codigos[n->caractere] = strdup(buffer);
        if (!codigos[n->caractere]) {
            perror("strdup");
            exit(1);
        }
        return;
    }

    buffer[prof] = '0';
    gerar_codigos(n->esq, buffer, prof + 1, codigos);
    buffer[prof] = '1';
    gerar_codigos(n->dir, buffer, prof + 1, codigos);
}

static void escrever_u64(FILE *f, uint64_t v) {
    for (int i = 0; i < 8; i++)
        fputc((int)((v >> (8 * i)) & 0xFF), f);
}

static int ler_u64(FILE *f, uint64_t *v) {
    uint64_t r = 0;
    for (int i = 0; i < 8; i++) {
        int c = fgetc(f);
        if (c == EOF) return 0;
        r |= (uint64_t)c << (8 * i);
    }
    *v = r;
    return 1;
}

static int compactar(const char *entrada, const char *saida) {
    FILE *in = fopen(entrada, "rb");
    if (!in) {
        fprintf(stderr, "Erro ao abrir '%s': ", entrada);
        perror(NULL);
        return 1;
    }

    uint64_t freq[ALFABETO] = {0};
    uint64_t total = 0;
    unsigned char buf[TAM_BUFFER];
    size_t n;

    while ((n = fread(buf, 1, sizeof buf, in)) > 0) {
        for (size_t i = 0; i < n; i++) freq[buf[i]]++;
        total += n;
    }
    if (ferror(in)) {
        fprintf(stderr, "Erro ao ler '%s'.\n", entrada);
        fclose(in);
        return 1;
    }

    No *raiz = construir_arvore(freq);
    char *codigos[ALFABETO] = {0};
    char tmp[ALFABETO + 1];
    gerar_codigos(raiz, tmp, 0, codigos);

    FILE *out = fopen(saida, "wb");
    if (!out) {
        fprintf(stderr, "Erro ao criar '%s': ", saida);
        perror(NULL);
        fclose(in);
        liberar_arvore(raiz);
        return 1;
    }

    fwrite(ASSINATURA, 1, 4, out);
    escrever_u64(out, total);

    int distintos = 0;
    for (int i = 0; i < ALFABETO; i++)
        if (freq[i] > 0) distintos++;
    fputc(distintos & 0xFF, out);
    fputc((distintos >> 8) & 0xFF, out);

    for (int i = 0; i < ALFABETO; i++) {
        if (freq[i] > 0) {
            fputc(i, out);
            escrever_u64(out, freq[i]);
        }
    }

    rewind(in);
    unsigned char byte = 0;
    int nbits = 0;

    while ((n = fread(buf, 1, sizeof buf, in)) > 0) {
        for (size_t i = 0; i < n; i++) {
            for (const char *c = codigos[buf[i]]; *c; c++) {
                byte = (unsigned char)((byte << 1) | (*c == '1'));
                if (++nbits == 8) {
                    fputc(byte, out);
                    byte = 0;
                    nbits = 0;
                }
            }
        }
    }
    if (nbits > 0)
        fputc((unsigned char)(byte << (8 - nbits)), out);

    long tam_saida = ftell(out);
    int erro = ferror(in) || ferror(out);
    if (fclose(out) != 0) erro = 1;
    fclose(in);

    for (int i = 0; i < ALFABETO; i++) free(codigos[i]);
    liberar_arvore(raiz);

    if (erro) {
        fprintf(stderr, "Erro durante a compactacao.\n");
        return 1;
    }

    printf("Arquivo compactado: %s\n", saida);
    printf("Tamanho original:   %llu bytes\n", (unsigned long long)total);
    printf("Tamanho compactado: %ld bytes\n", tam_saida);
    if (total > 0)
        printf("Taxa de compressao: %.2f%%\n",
               100.0 * (1.0 - (double)tam_saida / (double)total));
    return 0;
}

static int descompactar(const char *entrada, const char *saida) {
    FILE *in = fopen(entrada, "rb");
    if (!in) {
        fprintf(stderr, "Erro ao abrir '%s': ", entrada);
        perror(NULL);
        return 1;
    }

    
    char assin[4];
    if (fread(assin, 1, 4, in) != 4 || memcmp(assin, ASSINATURA, 4) != 0) {
        fprintf(stderr, "'%s' nao e um arquivo compactado valido.\n", entrada);
        fclose(in);
        return 1;
    }

    uint64_t total;
    if (!ler_u64(in, &total)) goto corrompido;

    int b0 = fgetc(in), b1 = fgetc(in);
    if (b0 == EOF || b1 == EOF) goto corrompido;
    int distintos = b0 | (b1 << 8);
    if (distintos > ALFABETO) goto corrompido;

    uint64_t freq[ALFABETO] = {0};
    uint64_t soma = 0;
    for (int i = 0; i < distintos; i++) {
        int simbolo = fgetc(in);
        uint64_t f;
        if (simbolo == EOF || !ler_u64(in, &f)) goto corrompido;
        freq[simbolo] = f;
        soma += f;
    }
    if (soma != total) goto corrompido;

    No *raiz = construir_arvore(freq);

    FILE *out = fopen(saida, "wb");
    if (!out) {
        fprintf(stderr, "Erro ao criar '%s': ", saida);
        perror(NULL);
        fclose(in);
        liberar_arvore(raiz);
        return 1;
    }

    uint64_t escritos = 0;
    const No *atual = raiz;
    int truncado = 0;

    while (escritos < total) {
        int c = fgetc(in);
        if (c == EOF) {
            truncado = 1;
            break;
        }
        for (int bit = 7; bit >= 0 && escritos < total; bit--) {
            if (!eh_folha(raiz))
                atual = ((c >> bit) & 1) ? atual->dir : atual->esq;
            if (eh_folha(atual)) {
                fputc(atual->caractere, out);
                escritos++;
                atual = raiz;
            }
        }
    }

    int erro = ferror(out);
    if (fclose(out) != 0) erro = 1;
    fclose(in);
    liberar_arvore(raiz);

    if (truncado) {
        fprintf(stderr, "Arquivo compactado esta truncado.\n");
        return 1;
    }
    if (erro) {
        fprintf(stderr, "Erro ao escrever '%s'.\n", saida);
        return 1;
    }

    printf("Arquivo restaurado: %s (%llu bytes)\n", saida,
           (unsigned long long)total);
    return 0;

corrompido:
    fprintf(stderr, "Arquivo compactado corrompido ou incompleto.\n");
    fclose(in);
    return 1;
}

static void uso(const char *prog) {
    fprintf(stderr,
            "Uso:\n"
            "  %s c <arquivo_original> <arquivo_compactado>   (compactar)\n"
            "  %s d <arquivo_compactado> <arquivo_restaurado> (descompactar)\n",
            prog, prog);
}

int main(int argc, char *argv[]) {
    if (argc != 4 || strlen(argv[1]) != 1) {
        uso(argv[0]);
        return 1;
    }

    if (argv[1][0] == 'c') return compactar(argv[2], argv[3]);
    if (argv[1][0] == 'd') return descompactar(argv[2], argv[3]);

    uso(argv[0]);
    return 1;
}