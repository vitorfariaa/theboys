#include <stdio.h>
#include <stdlib.h>

// struct evento, com tipo, tempo p ordenacao, dois dados (máximo requisitado pelos eventos), e proximo evento
typedef struct evento {
	int tipo;
	int tempo;
	int dado_1;
	int dado_2;
	struct evento *prox;
} evento;

// inicializa uma lef
evento* cria_lef ();

// limpa todos os elementos da lef com funcao free
void destroi_lef (evento *lef);

// insere um novo elemento na lef, de acordo com a variavel tempo
int insere_lef (evento **lef, int tipo, int tempo, int dado_1, int dado_2);

// remove um elemento da lef, sempre pegando da primeira posicao
int remove_lef(evento **lef);
