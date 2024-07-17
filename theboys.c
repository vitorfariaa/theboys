#include "theboys.h"

int main(){

// inicializando a seed para a funcao rand, a lef e o mundo
    srand(time(0));
    evento *lef = cria_lef();
    mundo mu;

// contadores para as tentativas e erros das missoes
	int tentativas = 0;
	int cumpridas = 0;

// inicializacao do mundo e dos eventos iniciais
	eventosIniciais(&mu, &lef);

// loop no formato switch-case para controlar os eventos de acordo com a variavel "tipo" da lef
// as constantes da variavel tipo estao declaradas no arquivo theboys.h
    while (mu.relogio < T_FIM_DO_MUNDO){
        mu.relogio = lef->tempo;

        switch(lef->tipo){
            case CHEGA:
                chega(lef->tempo, &(mu.herois[lef->dado_1]), &(mu.bases[lef->dado_2]), &lef);
                break;

            case ESPERA:
                espera(lef->tempo, &(mu.herois[lef->dado_1]), &(mu.bases[lef->dado_2]), &lef);
                break;

            case DESISTE:
                desiste(lef->tempo, &(mu.herois[lef->dado_1]), &(mu.bases[lef->dado_2]), &lef);
                break;

            case AVISA:
				avisa(lef->tempo, &(mu.bases[lef->dado_2]), &lef);
                break;

            case ENTRA:
                entra(lef->tempo, &(mu.herois[lef->dado_1]), &(mu.bases[lef->dado_2]), &lef);
                break;

            case VIAJA:
                viaja(lef->tempo, &(mu.herois[lef->dado_1]), &(mu.bases[lef->dado_2]), mu, &lef);
                break;

            case SAI:
                sai(lef->tempo, &(mu.herois[lef->dado_1]), &(mu.bases[lef->dado_2]), &lef);
                break;

            case MISSAO:
                missaoE(lef->tempo, &(mu.missoes[lef->dado_1]), &mu, &tentativas, &cumpridas, &lef);
				break;

            case FIM_MUNDO:
                fim(lef->tempo, mu, tentativas, cumpridas);
				break;

			default: // caso default irá encerrar o programa caso receba algum tipo invalido da lef
				break;

        }
		remove_lef(&lef);
    }

// limpa a lef para impedir vazamento de memoria
    destroi_lef(lef);
	return 0;
}

void chega(int tempo, heroi *h, base *b, evento **lef){
	
	int numPresentes = tamanhoConjunto(b->presentes);
	h->base = b->id;
	bool espera;

	printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d)", tempo, h->id, b->id, numPresentes, b->lotacao);

	// ao heroi chegar numa base, sera analisado a situacao de lotacao
	// caso tenha vagas na base, o heroi ira aceitar entrar nela
	if ((numPresentes < b->lotacao) && eh_vazia(b->espera))
		espera = true;
	// se nao, dependera de sua paciencia ficar ou nao na fila de espera
	else
		espera = ((h->pac) > (10 * tam_fila(b->espera)));
	
	// o resultado do booleano "espera" ira ditar se o heroi ira esperar na base ou desistir
	if (espera){
		insere_lef(lef, ESPERA, tempo, h->id, b->id);
		printf(" ESPERA\n");
	}
	else{
		insere_lef(lef, DESISTE, tempo, h->id, b->id);
		printf(" DESISTE\n");
	}
}

void espera(int tempo, heroi *h, base *b, evento **lef){

	printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, h->id, b->id, tam_fila(b->espera));
	
	// o heroi entrara na fila de espera da base e aguardara pelo resultado do porteiro na funcao AVISA se entrara ou nao na base
	enfileirar(&(b->espera), h->id);
	insere_lef(lef, AVISA, tempo, -1, b->id); // o evento avisa nao possui parametro de heroi, por isso, eh passado para lef -1
}

void desiste(int tempo, heroi *h, base *b, evento **lef){

	printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, h->id, b->id);
	
	// se o heroi desistir de entrar em uma base, ele ira sortear uma outra base e viajara para ela
	int baseD = sorteia(0, N_BASES - 1);
	insere_lef(lef, VIAJA, tempo, h->id, baseD);
}

void avisa(int tempo, base *b, evento **lef){

	printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", tempo, b->id, tamanhoConjunto(b->presentes), b->lotacao);
	imprime_fila(b->espera);
	printf("]\n");

	// enquanto tiver espacos livres na base, e pessoas na fila esperando, o porteiro ira admitir as pessoas da fila para dentro da base
	// a ordem de admissao segue o padrao fila, ou seja, os primeiros da fila tem prioridade
	while ((tamanhoConjunto(b->presentes)) < b->lotacao && (tam_fila(b->espera)) > 0) {
		int heroiID = desenfileirar(&(b->espera));
		inserir(&(b->presentes), heroiID);
		insere_lef(lef, ENTRA, tempo, heroiID, b->id);
		printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, b->id, heroiID);
	}
}

void entra(int tempo, heroi *h, base *b, evento **lef){

	// ao entrar numa base, o heroi sorteia um tempo de permanencia nela. Quando esgotado, ira viajar para uma outra base
	// o tempo de permanencia é calculado pela paciencia do heroi * um numero aleatorio de 1-20, somado com 15
	int tpb = 15 + (h->pac * sorteia(1,20));
	printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, h->id, b->id, tamanhoConjunto(b->presentes), b->lotacao, tempo + tpb);

	insere_lef(lef, SAI, tempo+tpb, h->id, b->id);
}

void sai(int tempo, heroi *h, base *b, evento **lef){

	// ao sair, o heroi é removido do conjunto dos presentes naquela base
	// é sorteado uma base D aleatoria para a qual o heroi ira viajar
	retiraConjunto(&(b->presentes), h->id);
	int baseD = sorteia(0, N_BASES - 1);
	
	// insere na lef o evento de viagem do heroi, e avisa para o porteiro que liberou vagas na base
	insere_lef(lef, VIAJA, tempo, h->id, baseD);
	insere_lef(lef, AVISA, tempo, -1, b->id);

	printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo, h->id, b->id, tamanhoConjunto(b->presentes), b->lotacao);
}	

void viaja(int tempo, heroi *h, base *d, mundo mu, evento **lef){

	// a distancia do heroi ate a base e calculada de acordo com a distancia cartesiana, na funcao "distancia_pontos"
	int distancia = distancia_pontos(mu.bases[h->base].localX,mu.bases[h->base].localY,d->localX,d->localY);
	
	// a duracao dessa viagem segue de acordo com o atributo velocidade do heroi
	int duracao = distancia / h->velo;

	 printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, h->id, mu.bases[h->base].id, d->id, distancia, h->velo, tempo+duracao);

	insere_lef(lef, CHEGA, tempo+duracao, h->id, d->id); // heroi chegara na base destino no tempo atual + a duracao
}	

void missaoE(int tempo, missao *mi, mundo *mu, int *tentativas, int *cumpridas, evento **lef){

	printf("%6d: MISSAO %d HAB REQ : [ ", tempo, mi->id);
	imprime(mi->habilidades);
	printf("]\n");

	// a funcao missao recebe de parametros os contadores de tentativas e missoes cumpridas
	// dessa forma, e possivel calcular o saldo final na funcao fim
	(*tentativas)++;
	base vetorBases[N_BASES];

	// copia o vetor de bases do mundo para um auxiliar que será ordenado
	for (int i = 0; i < N_BASES; i++){
		vetorBases[i] = mu->bases[i];
	}

	// selection sort no vetor auxiliar de bases usando como parametro a distancia ate a missao
	base_SelectionSort(vetorBases, N_BASES, mi->localX, mi->localY);

	int bmp = -1; // inicia o indice da base mais proxima em -1 (nao encontrado)
	int indice = 0;
	bool requisitos = false;

	conjunto *habilidades = NULL;
	conjunto *heroisBase;

	while (indice < N_BASES && requisitos == false){ // enquanto houver bases e nenhuma tiver cumprido os requisitos

		heroisBase = vetorBases[indice].presentes;

	// faz a uniao das habilidades de todos os herois daquela base
		while (heroisBase != NULL){
			int heroi = heroisBase->numero;
			conjunto *aux_habilidades = habilidades; // aux do conjunto habilidades, para o free

			uniao(habilidades, mu->herois[heroi].habilidades, &(habilidades));

			// como a funcao uniao faz o conjunto "habilidades" apontar para um novo conjunto auxiliar
			// eh necessario dar free na memoria que antes pertencia ao conjunto "habilidades"
			limpa_conjunto(aux_habilidades);

			heroisBase = heroisBase->prox;
		}
		
		// se essa uniao conter as habilidades requisitadas, a missao podera ser realizada
		if (contem(habilidades, mi->habilidades)){
			bmp = indice;
			requisitos = true;
		}
		
		// imprime o conjunto de habilidades de cada base testada
		printf("%6d: MISSAO %d HAB BASE %d: [ ", tempo, mi->id, vetorBases[indice].id);
		imprime(habilidades);
		printf("]\n");
		
		// free no conjunto de habilidades apos cada iteracao
		while (habilidades != NULL){
			conjunto *aux = habilidades->prox;
			free(habilidades);
			habilidades = aux;
		}

		indice++; // contador indice para andar pelas bases do vetor de base
	}

	if (requisitos){
	
	// se a missao for realizada, todos os herois daquela base irao ganhar xp, e o contador de missoes
	// cumpridas ira subir um
		int id = vetorBases[bmp].id;
		conjunto *heroisXp = mu->bases[id].presentes;

		while (heroisXp != NULL){
			mu->herois[heroisXp->numero].exp++;
			heroisXp = heroisXp->prox;
		}
		printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: [ ", tempo, mi->id, id);
		imprime(mu->bases[id].presentes);
		printf("]\n");
		(*cumpridas)++;
	}

	else {
	// se a missao nao tiver sucesso, sera adiada para daqui 24 horas (1 dia)
		printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, mi->id);
		insere_lef(lef, MISSAO, tempo + (24*60), mi->id, -1);
	}
}

void fim(int tempo, mundo mu, int tentativas, int cumpridas){

	printf("%6d: FIM\n", tempo);

	// imprime saldo de todos os herois
	for (int i = 0; i < N_HEROIS; i++){
		printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS [ ", i, mu.herois[i].pac, mu.herois[i].velo, mu.herois[i].exp);
		imprime(mu.herois[i].habilidades);
		printf("]\n");
	}

	float tentativasPmissao = (float)tentativas / mu.Nmissoes;
	float porcentagem_cumpridas = ((float)cumpridas / mu.Nmissoes)*100;

	// imprime saldo das missoes
	printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n", cumpridas, mu.Nmissoes, porcentagem_cumpridas, tentativasPmissao);
	
	for (int i = 0; i < N_BASES; i++){
		limpa_fila(mu.bases[i].espera); // free em todas as filas de espera
	}

	for (int i = 0; i < N_HEROIS; i++){
        limpa_conjunto(mu.herois[i].habilidades); // free nos conjuntos de habilidades de todos os herois
	}

	for (int i = 0; i < N_BASES; i++){
        limpa_conjunto(mu.bases[i].presentes); // free no conjunto de presentes das bases
	}

	for (int i = 0; i < N_MISSOES; i++){
		limpa_conjunto(mu.missoes[i].habilidades); // free nos requisitos de habilidades das missoes
	}

}

// inicializa cada um dos 50 herois 
// cada heroi vai ter um id, experiencia, paciencia, velocidade e de 1-3 habilidades
void iniciaHerois(mundo *mu){

	for (int i = 0; i < N_HEROIS; i++){
		mu->herois[i].id = i;
		mu->herois[i].exp = 0;
		mu->herois[i].pac = sorteia(1,100);
		mu->herois[i].velo = sorteia(50,5000);

		int nHabilidades = sorteia(1,3);
		mu->herois[i].habilidades = NULL;

		for (int j = 0; j < nHabilidades; j++){
			int hab = sorteia(1, N_HABILIDADES);
			inserir(&(mu->herois[i].habilidades), hab);
		}
	}
}

// inicializa as bases sorteando seu local (x,y), sua capacidade, iniciando seu conjunto de presentes e fila de espera
void iniciaBases(mundo *mu){

	for (int i = 0; i < N_BASES; i++){
		mu->bases[i].id = i;
		mu->bases[i].localX = sorteia(0, N_TAMANHO_MUNDO - 1);
		mu->bases[i].localY = sorteia(0, N_TAMANHO_MUNDO - 1);
		mu->bases[i].lotacao = sorteia(3, 10);
		mu->bases[i].presentes = NULL;
		mu->bases[i].espera = NULL;
	}
}

// inicializa as missoes sorteando o local (x,y), numero de habilidade requisitadas e quais habilidades sao requisitadas
void iniciaMissoes(mundo *mu){

	for (int i = 0; i < N_MISSOES; i++){
		mu->missoes[i].id = i;
		mu->missoes[i].localX = sorteia(0, N_TAMANHO_MUNDO - 1);
		mu->missoes[i].localY = sorteia(0, N_TAMANHO_MUNDO - 1);

		int nHabilidades = sorteia(6,10);
		mu->missoes[i].habilidades = NULL;

		for (int j = 0; j < nHabilidades; j++){
			int hab = sorteia(1, N_HABILIDADES);
			inserir(&(mu->missoes[i].habilidades), hab);
		}
	}
}

// funcao eventos iniciais incializa o mundo com os parametros passados
// alem disso, sorteia os primeiros eventos (como missoes e nascimento dos herois)
void eventosIniciais(mundo *mu, evento **lef){

	// inicializacao do mundo
	mu->relogio = 0;
	mu->xMax = N_TAMANHO_MUNDO - 1;
	mu->yMax = N_TAMANHO_MUNDO - 1;

	mu->Nherois = N_HEROIS;
	mu->Nbases = N_BASES;
	mu->Nmissoes = N_MISSOES;
	mu->Nhabilidades = N_HABILIDADES;

	iniciaHerois(mu);
	iniciaBases(mu);
	iniciaMissoes(mu);

	for (int i = 0; i < N_HEROIS; i++){
		int base = sorteia(0, N_BASES - 1);
		int tempo = sorteia(0, 4320); // 4320 são os minutos contidos em 3 dias, tempo maximo p todos os herois nascerem
		insere_lef(lef, CHEGA, tempo, i, base);
	}

	for (int i = 0; i < N_MISSOES; i++){
		int tempo = sorteia(0, T_FIM_DO_MUNDO);
		insere_lef(lef, MISSAO, tempo, i, -1); // missoes podem ocorrer em qualquer minuto do progresso
	}

	insere_lef(lef, FIM_MUNDO, T_FIM_DO_MUNDO, -1, -1); // por fim insere na lef o ultimo evento (fim do mundo)
}	


void base_SelectionSort(base v[], int tam, int xmissao, int ymissao){

    int i, j, min;

    // algoritmo de ordenacao por selecao
    for (i = 0; i < tam - 1; i++) {
        min = i;
        for (j = i + 1; j < tam; j++) {
			// encontra min de acordo com a distancia
			int vj = distancia_pontos(v[j].localX, v[j].localY, xmissao, ymissao);
			int vmin = distancia_pontos(v[min].localX, v[min].localY, xmissao, ymissao);
            if (vj < vmin) {
                min = j;
            }
        }
        // troca os elementos conforme necessario
        trocaBase(&v[min], &v[i]);
    }
}

// funcao troca para o selection sort
void trocaBase(base *a, base *b){

	base aux = *a;
	*a = *b;
	*b = aux;
}

//calcula a distancia cartesiana entre pontos (raiz(x^2 + y^2))
int distancia_pontos(int x1, int y1, int x2, int y2){

	unsigned int x = x2-x1;
	unsigned int y = y2-y1;

	return round(sqrt((double)((x*x)+(y*y))));
}

// retorna num aleatorio entre min e max
int sorteia(int min, int max){

	int sorteio = rand() % (max + 1 - min) + min;
	return sorteio;
}

