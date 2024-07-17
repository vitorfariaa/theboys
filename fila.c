#include "fila.h"

bool enfileirar(fila **f, int dados){
    fila *novo = (fila*)malloc(sizeof(fila)); // maloc para um novo no na lista encadeada
	novo->dados = dados;
	novo->prox = NULL;

	if(eh_vazia(*f)){ // se a fila estiver vazia coloca na primeira posicao
		*f = novo;
		return true;
	}

	fila *aux = *f;

	while (aux->prox != NULL) // se nao, anda pela fila com variavel aux p achar posicao certa
		aux = aux->prox;

	aux->prox = novo;
	return true;
}

int desenfileirar(fila **f){
    if (eh_vazia(*f))
		return 0;
	
	int retorno = (*f)->dados;

	fila *aux = (*f)->prox; // remove e limpa primeira posicao da fila
	free(*f);

	*f = aux;

	return retorno; // retorna o conteudo do no removido
}

bool eh_vazia(fila *f){
	return (f == NULL);
}

void limpa_fila(fila *f){
	while (f != NULL){
		fila *aux = f->prox;
		free(f); // anda pela fila e limpa cada no
		f = aux;
	}
}

int tam_fila(fila *f){
	fila *aux = f;
	int tamanho = 0;

	while (aux != NULL){ // anda pela fila com variavel aux e conta quantas posicoes andou
		tamanho++;
		aux = aux->prox;
	}
	return tamanho;
}

void imprime_fila(fila *f){
	fila *aux = f;
	while (aux != NULL){
		printf("%d ", aux->dados); // imprime conteudo de cada posicao da fila
		aux = aux->prox;
	}
}
