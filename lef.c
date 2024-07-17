#include "lef.h"

// inicializa a lef de eventos com NULL
evento* cria_lef (){
    evento *lef;

    lef = NULL;

    return lef;
}

// limpa cada elemento da lef
void destroi_lef (evento *lef) {
	while (lef != NULL){
		evento *aux = lef->prox;
		free(lef);
		lef = aux;
	}
}

int insere_lef (evento **lef, int tipo, int tempo, int dado_1, int dado_2){
   
	evento *novoEvento = (evento *)malloc(sizeof(evento));
	if (!novoEvento){
		return 0; //retorna 0 se a alocacao falhou
	}

    novoEvento->tipo = tipo;
	novoEvento->tempo = tempo;
	novoEvento->dado_1 = dado_1;
	novoEvento->dado_2 = dado_2;
	novoEvento->prox = NULL;

// secao para encontrar posicao na lef a partir do tempo
	
	evento *aux = *lef;

	if (aux == NULL || aux->tempo > tempo){ // checar se o novo evento pertence a primeira posicao
		novoEvento->prox = *lef;
		*lef = novoEvento;
		return 1;
	}

	while (aux->prox != NULL && aux->prox->tempo <= tempo){ // se nao procura a posicao de acordo com a variavel tempo
		aux = aux->prox;
	}
	novoEvento->prox = aux->prox;
	aux->prox = novoEvento;

    return 1;
}

int remove_lef (evento **lef){
    if (*lef == NULL)
		return 0;

    evento *aux = *lef;
	*lef = (*lef)->prox; // remove primeira posicao da lef (prioridade de acordo com a variavel tempo)
	free(aux);
	return 1;
}
