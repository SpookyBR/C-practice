public class ArvoreRubroNegra {
    private static final boolean VERMELHO = true;
    private static final boolean PRETO = false;

    private class No {
        int chave;
        boolean cor;
        No esq, dir, pai;

        No(int chave) {
            this.chave = chave;
            this.cor = VERMELHO;
        }
    }

    private No raiz;

    private void rotacionarEsquerda(No p) {
        No z = p.dir;
        p.dir = z.esq;
        if (z.esq != null) z.esq.pai = p;
        z.pai = p.pai;
        if (p.pai == null) raiz = z;
        else if (p == p.pai.esq) p.pai.esq = z;
        else p.pai.dir = z;
        z.esq = p;
        p.pai = z;
    }

    private void rotacionarDireita(No p) {
        No u = p.esq;
        p.esq = u.dir;
        if (u.dir != null) u.dir.pai = p;
        u.pai = p.pai;
        if (p.pai == null) raiz = u;
        else if (p == p.pai.dir) p.pai.dir = u;
        else p.pai.esq = u;
        u.dir = p;
        p.pai = u;
    }
    
    public void inserir(int chave) {
        No novo = new No(chave);
        No q = null;
        No p = raiz;

        while (p != null) {
            q = p;
            if (novo.chave < p.chave) p = p.esq;
            else if (novo.chave > p.chave) p = p.dir;
            else return;
        }

        novo.pai = q;
        if (q == null) {
            raiz = novo;
        } else if (novo.chave < q.chave) {
            q.esq = novo;
        } else {
            q.dir = novo;
        }

        corrigirInsercao(novo);
    }

    private void corrigirInsercao(No k) {
        No tio;
        while (k.pai != null && k.pai.cor == VERMELHO) {
            if (k.pai == k.pai.pai.esq) {
                tio = k.pai.pai.dir;
                if (tio != null && tio.cor == VERMELHO) {
                    k.pai.cor = PRETO;
                    tio.cor = PRETO;
                    k.pai.pai.cor = VERMELHO;
                    k = k.pai.pai;
                } else {
                    if (k == k.pai.dir) {
                        k = k.pai;
                        rotacionarEsquerda(k);
                    }
                    k.pai.cor = PRETO;
                    k.pai.pai.cor = VERMELHO;
                    rotacionarDireita(k.pai.pai);
                }
            } else {
                tio = k.pai.pai.esq;
                if (tio != null && tio.cor == VERMELHO) {
                    k.pai.cor = PRETO;
                    tio.cor = PRETO;
                    k.pai.pai.cor = VERMELHO;
                    k = k.pai.pai;
                } else {
                    if (k == k.pai.esq) {
                        k = k.pai;
                        rotacionarDireita(k);
                    }
                    k.pai.cor = PRETO;
                    k.pai.pai.cor = VERMELHO;
                    rotacionarEsquerda(k.pai.pai);
                }
            }
        }
        raiz.cor = PRETO;
    }
}