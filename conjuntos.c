#include "conjuntos.h"

bool uniao(conjunto *inicio1, conjunto *inicio2, conjunto **resultado){

    conjunto *aux = inicio1;
    conjunto *aux_resultado = NULL; // variaveis auxiliares p/ andar na lista

    while (aux != NULL){
		if (!existe(aux_resultado, aux->numero))  // primeiro passa todo o primeiro conjunto p/ o resultado
			inserir(&aux_resultado, aux->numero);  // (sem repetir nenhum elemento)
        aux = aux->prox;
    }

    aux = inicio2;  // aponta a variavel auxiliar para a cabeca do segundo conjunto

    while (aux != NULL){
		if (!existe(aux_resultado, aux->numero)){
			inserir(&aux_resultado, aux->numero); //se for um elemento novo, ele sera atribuido ao resultado
		}
		aux = aux->prox;
	}

	*resultado = aux_resultado;

	return true; // retorna true se uniao sucedeu
}


bool interseccao(conjunto *inicio1, conjunto *inicio2, conjunto **resultado){
	if (inicio1 == NULL || inicio2 == NULL)
		return false; // se algum dos conjuntos for vazio, nao havera interseccao (return false)

	conjunto *aux_resultado = NULL;
	conjunto *aux = inicio1;

	//se o valor do conjunto 1 existir no conjunto 2, e ainda nao estiver no resultado, sera add a ele
	while (aux != NULL){
		if (existe(inicio2, aux->numero) && !existe(aux_resultado, aux->numero))
			inserir(&aux_resultado, aux->numero);
		aux = aux->prox;
	}

	*resultado = aux_resultado; 
	return true;
}


bool existe(conjunto *lista, int valor){
	conjunto *aux;
	aux = lista;

	while (aux != NULL){
		if (aux->numero == valor) // passa pelos elementos do conjunto, e se encontrar o valor, ja retorna true
			return true;
		aux = aux->prox;
	}
	return false; // se chegar ao fim do while, entao o elemento nao foi encontrado, entao nao existe no conjunto
}

bool contem(conjunto *inicio1, conjunto *inicio2){
	conjunto *aux = NULL;
	bool contem = false;

	// o conjunto B estara contido em A se for = a intersseccao entre os 2
	if (!interseccao(inicio1, inicio2, &aux))
		contem = false;

	if (igual(inicio2, aux)) // retornando true se o conjunto A contiver B
		contem = true;

	limpa_conjunto(aux);

	return contem;
}

void imprime(conjunto *lista){
	conjunto *aux;
	aux = lista;

	while (aux != NULL){
		printf("%d ", aux->numero); // imprime o conteudo de cada pos do conjunto
		aux = aux->prox;
	}
}

bool igual(conjunto *inicio1, conjunto *inicio2){
	conjunto *auxI = NULL;
	conjunto *auxU = NULL;// conjuntos serao iguais se a uniao for igual a interseccao
	bool igualB = true;

	if (!interseccao(inicio1, inicio2, &auxI))
		return false; // se a interseccao ja der errado, podemos abortar a funcao

	if (!uniao(inicio1, inicio2, &auxU))
		return false; // o mesmo vale para a uniao
	
	conjunto *auxI_ = auxI; // conjuntos auxiliares dos auxiliares para limpeza posterior
	conjunto *auxU_ = auxU;

	while (auxI != NULL && auxU != NULL){  // checa a igualdade dos elementos
		if (auxI->numero != auxU->numero)
			igualB = false;
		auxI = auxI->prox;
		auxU = auxU->prox;
	}

	if (auxU != NULL || auxI != NULL)   // checando tambem os tamanhos
		igualB = false;

	limpa_conjunto(auxU_);
	limpa_conjunto(auxI_); // free nos auxiliares dos auxiliares

	return igualB;
}

// aloca memoria e insere um novo valor ao conjunto
void inserir(conjunto **lista, int valor){
	conjunto *aux = (conjunto *) malloc(sizeof(conjunto)); // alloc para a lista
	aux->numero = valor;
	aux->prox = *lista;
	*lista = aux;
} 

int tamanhoConjunto(conjunto *lista){
	conjunto *aux = lista;
	int tamanho = 0;

// anda pelo conjunto com um conjunto auxiliar e conta qnts posicoes andou
	while (aux != NULL){
		tamanho++;
		aux = aux->prox;
	}
	return tamanho;
}

void retiraConjunto(conjunto **lista, int valor) {
    conjunto *aux = *lista;

    if (aux != NULL) {
        if (aux->numero == valor) { // se o valor estiver na primeira pos
            *lista = aux->prox;
            free(aux);}

		else { // se nao avanca ate o penultimo elemento ou achar o valor
            while (aux->prox != NULL && aux->prox->numero != valor) {
                aux = aux->prox;
			}

            conjunto *removido = aux->prox;
            aux->prox = aux->prox->prox;
			free(removido); // free na posicao removida
        }
    }
}

bool duplicado(conjunto *lista, conjunto **duplicados, conjunto **duplicados_v){ // função devolvera dois conjuntos: um com os duplicados, e outro com as respectivas repeticoes.
    conjunto *aux = lista;
    conjunto *aux2 = NULL;
    conjunto *aux3 = NULL;

    while (aux != NULL){
        if (aux->prox != NULL && existe(aux->prox, aux->numero) && !existe(aux2, aux->numero)){
    // se um elemento aparece mais de uma vez, sera adicionado ao conjunto dos repetidos
            conjunto *anda = aux->prox;
            inserir(&aux2, aux->numero);
            conjunto *aux33 = (conjunto *)malloc(sizeof(conjunto));
            aux33->numero = 1;

            while (anda != NULL){ // while para contar as repeticoes
                if (anda->numero == aux->numero)
                    aux33->numero++;
                anda = anda->prox;
            }

            aux33->prox = aux3;
            aux3 = aux33;
        }
        aux = aux->prox;
    }


	*duplicados_v = aux3; // devolve os dois "vetores" resultantes
    *duplicados = aux2;

    if (*duplicados != NULL)
        return true;  // funcao devolve true se houver pelo menos um elemento duplicado
    return false;
}

// limpa cada elemento do conjunto
void limpa_conjunto(conjunto *lista){
	while (lista != NULL){
		conjunto *aux = lista->prox;
		free(lista);
		lista = aux;
	}
}
