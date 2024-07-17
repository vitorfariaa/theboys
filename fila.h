#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// struct fila possui um valor do no e um ponteiro para o proximo elemento da fila
typedef struct fila {
    int dados;
    struct fila *prox;
}fila;

// insere um novo valor na fila, sempre na ultima posicao
bool enfileirar(fila **f, int dados);

// retira um valor da fila, o da primeira posicao
int desenfileirar(fila **f);

// retorna true se a fila estiver vazia
bool eh_vazia(fila *f);

// da free em todos os elementos da fila
void limpa_fila(fila *f);

// retorna a quantidade de elementos da fila
int tam_fila(fila *f);

// imprime cada elemento da fila
void imprime_fila(fila *f);
