#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// struct conjunto contem o valor de um no e o ponteiro para o proximo no
typedef struct conjunto {
	int numero;
	struct conjunto *prox;
}conjunto;

// uniao matematica entre conjuntos (A + B - (A.B))
bool uniao(conjunto *inicio1, conjunto *inicio2, conjunto **resultado);

// retorna um conjunto de elementos que existem em ambos os conjuntos A e B
bool interseccao(conjunto *inicio1, conjunto *inicio2, conjunto **resultado);

// retorna true se o int valor existe no conjunto, e false se nao existe
bool existe(conjunto *lista, int valor);

// se todos os elementos do conjunto B estao presentes no conjunto A, retornara true
bool contem(conjunto *inicio1, conjunto *inicio2);

//imprime todos os elementos de um conjunto
void imprime(conjunto *lista);

// verifica se dois conjuntos sao identicos
bool igual(conjunto *inicio1, conjunto *inicio2);

// cria um no com um valor e insere no conjunto
void inserir(conjunto **lista, int valor);

// retorna a quantidade de elementos em um conjunto
int tamanhoConjunto(conjunto *lista);

// retira o valor do parametro de um conjunto
void retiraConjunto(conjunto **lista, int valor);

// free em todos os elementos do conjunto
void limpa_conjunto(conjunto *lista);
