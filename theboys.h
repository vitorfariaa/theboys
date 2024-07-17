// todas as bibliotecas necessarias para implementacao do codigo principal
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "conjuntos.h"
#include "fila.h"
#include "lef.h"

// defines do estado inicial do enunciado do trabalho
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS 50
#define N_BASES N_HEROIS / 6
#define N_MISSOES T_FIM_DO_MUNDO / 100

// constantes para os tipos da lef de eventos
#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MISSAO 7
#define FIM_MUNDO 8

// structs definidas no enunciado do trabalho
typedef struct heroi {
	int id;
	conjunto *habilidades;
	int pac;
	int velo;
	int exp;
	int base;
} heroi;

typedef struct base {
	int id;
	int lotacao;
	conjunto *presentes;
	fila *espera;
	int localX;
	int localY;
} base;

typedef struct missao {
	int id;
	conjunto *habilidades;
	int localX;
	int localY;
} missao;

typedef struct mundo {
	int Nherois;
	heroi herois[N_HEROIS];

	int Nbases;
	base bases[N_BASES];

	int Nmissoes;
	missao missoes[N_MISSOES];

	int Nhabilidades;

	int relogio;
	int xMax;
	int yMax;
} mundo;

// funcoes principais da main do projeto The Boys
void chega(int tempo, heroi *h, base *b, evento **lef);

void espera(int tempo, heroi *h, base *b, evento **lef);

void desiste(int tempo, heroi *h, base *b, evento **lef);

void avisa(int tempo, base *b, evento **lef);

void entra(int tempo, heroi *h, base *b, evento **lef);

void sai(int tempo, heroi *h, base *b, evento **lef);

void viaja(int tempo, heroi *h, base *d, mundo mu, evento **lef);

void missaoE(int tempo, missao *mi, mundo *mu, int *tentativas, int *cumpridas, evento **lef);

void fim(int tempo, mundo mu, int tentativas, int cumpridas);

void iniciaHerois(mundo *mu);

void iniciaBases(mundo *mu);

void iniciaMissoes(mundo *mu);

void eventosIniciais(mundo *mu, evento **lef);

void base_SelectionSort(base v[], int tam, int xmissao, int ymissao);

void trocaBase(base *a, base *b);

int distancia_pontos(int x1, int y1, int x2, int y2);

int sorteia(int min, int max);
