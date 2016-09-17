#include <stdio.h>
#include <stdlib.h>


typedef struct no {
    struct no *anterior;
    int valor;
} no;

typedef no pilha;
pilha *empilha(pilha *p, int valor);
pilha *cria_pilha(int topo);

pilha *cria_pilha(int topo) {
    pilha *p = (pilha*)malloc(sizeof(pilha));
    p->anterior = NULL;
    p->valor = topo;
    return p;
}

pilha *empilha(pilha *p, int valor) {
    no *novoNo = (no*)malloc(sizeof(no));
    novoNo->anterior = p;
    novoNo->valor = valor;
    return novoNo;
}

pilha *desempilha(pilha *p) {
    no *novoTopo = p->anterior;
    free(p);
    return novoTopo;
}

pilha *esvazia_pilha(pilha *p) {
    no *topo = p;
    no *desempilhado;
    while(topo != NULL) {
        desempilhado = topo;
        topo = topo->anterior;
        printf("Desempilhando %d\n", desempilhado->valor);
        free(desempilhado);
    }
    return topo;
}

void imprime_pilha(pilha *p) {
    printf("chegou no imprimir pilha\n");
    no *noAtual = p;
    int i = 0;
    while(noAtual != NULL) {
        printf("Pilha[%d]: %d\n", i, noAtual->valor);
        noAtual = noAtual->anterior;
        i++;
    }
    printf("vai verificar i\n");
    if(i == 0) {
        printf("Pilha esta vazia\n");
    }
}

