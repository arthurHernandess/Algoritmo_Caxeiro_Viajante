/** @file ep.c
 * @brief Para uso como exemplo, contém uma aplicação da estratégia gulosa para
 * solução do TSP. Para uso como modelo de EP, deve conter sua solução para o EP.
 * 
 * Lembre-se de substituir o nome do autor, abaixo, pelo seu nome e NUSP.
 * (Redundância com o retorno da função autor())
 * 
 * @author Arthur de Lima Hernandes NUSP 15552518
 */
#include "ep.h"
#if 0 // modelo
Conjunto Guloso(Conjunto C)
/∗ C: conjunto de candidatos ∗/
{
	S = ∅ ; /∗ S contem conjunto solucao ∗/
	while( (C ! = ∅) && !( solucao(S) ) ) {
		x = seleciona (C) ;
		C = C − x;
		if (viavel (S + x )) S = S + x ;
	}
	if solucao(S) return(S) else return( ’Nao existe solucao ’ ) ;
}
#endif

int *grauDoVertice;
int *melhorSolucao; // flag que informa se uma aresta está sendo usada no caminho que está sendo construído ou não. Ao final, se o algoritmo chegar a uma solução, este array conterá essa solução.
int *solucaoIntermediaria;
int arVisitadas = 0;

/** @brief Esta função é importante para que o corretor automático inclua 
 * corretamente a autoria do EP. Deve retornar Nome e NUSP separados por 
 * ponto-e-virgula, sem pular linhas no meio nem no final.
 */
char *autor (void) {
	return "Arthur de Lima Hernandes ; 15552518";
}

int ehSolucao (struct grafo *G) { // devido à maneira de construção da solução, verificar que todos os vértices foram alcançados NÃO equivale à existência de um caminho hamiltoniano.
// tem que testar o grau do vértice
// se o grafo for composto por vários circuitos disjuntos que cobrem o grafo 
// todo, o que não é uma solução, os graus serão todos 2 e esta função 
// retornará que ehSolucao. Esta situação é evitada pela maneira como o circuito
// é construido pela função tenta, que vai aumentando um (único) caminho.
# if DEBUG
	for (int i=0;i<G->N;i++) printf ("%d ", grauDoVertice[i]);
    puts("");
# endif
	for (int i=0;i<G->N;i++) {
		if (grauDoVertice[i]!=2) return 0; // não coberto ou completou circuito com vértice errado. 
	}
	return 1;
}

int ehAceitavel (struct grafo *G, int arestaATestar, int verticeAtual) {
	if (solucaoIntermediaria[arestaATestar]) return 0;   // aresta a testar está em uso. Não é aceitável
	if (G->A[3*arestaATestar] == verticeAtual && G->A[3*arestaATestar] != G->A[3*arestaATestar+1]) { // arestaATestar conecta no vértice atual
		if (grauDoVertice[G->A[3*arestaATestar+1]] < 2) { // vértice no outro extremo da aresta não é coberto.
			return 1; // é aceitável
		}
	}
	if (G->A[3*arestaATestar+1] == verticeAtual && G->A[3*arestaATestar] != G->A[3*arestaATestar+1]) { // arestaATestar conecta no vértice atual.
		if (grauDoVertice[G->A[3*arestaATestar]] < 2) { // vértice no outro extremo da aresta não é coberto.
			return 1; // é aceitável
		}
	}
	return 0;
}

/* Parte do pressuposto que arestaAcrescentar é aceitável. */
int aumentaCaminho (struct grafo *G, int arestaAcrescentar, int verticeAtual) {  // registra movimento
	solucaoIntermediaria[arestaAcrescentar] = 1;   // TRUE: agora está em uso.
	grauDoVertice[G->A[3*arestaAcrescentar+1]]++;
	grauDoVertice[G->A[3*arestaAcrescentar]]++;
	arVisitadas++;
	if (G->A[3*arestaAcrescentar] == verticeAtual) { // ajusta o lado
		return G->A[3*arestaAcrescentar+1];
	}
	return G->A[3*arestaAcrescentar];
}

int diminuiCaminho (struct grafo *G, int arestaRemover, int verticeAtual) { 		// passo de "voltar" (pra, na função principal, a gnt complementar tentando outro caminho), faz o oposto de acrescentar um caminho novo
	solucaoIntermediaria[arestaRemover] = 0;
	grauDoVertice[G->A[3*arestaRemover+1]]--;
	grauDoVertice[G->A[3*arestaRemover]]--;
	arVisitadas--;
	if (G->A[3*arestaRemover] == verticeAtual) {
		return G->A[3*arestaRemover+1];
	}
	return G->A[3*arestaRemover];
}

int ehMelhor(struct grafo *g){				// verifica se o peso da solução intermediária é menor que o peso da melhor
	int pesoInt = 0;
	int pesoMlr = 0;
	for (int i=0; i < g->M; i++) {
		if(solucaoIntermediaria[i]){
			pesoInt += g->A[(3*i)+2];
		}
	}
	for (int j=0; j < g->M; j++) {
		if(melhorSolucao[j]){
			pesoMlr += g->A[(3*j)+2];
		}
	}

	if(pesoInt < pesoMlr)
		return 1;
	else
		return 0;
}

int atualizarMelhor(struct grafo *g){					// se ehMelhor() volta true, a gnt troca, atualiza melhorSolucao pra ter os valores dessa intermediaria 
	melhorSolucao[g->M+1] = -1;
	for (int i=0; i < g->M; i++) {
		melhorSolucao[i] = solucaoIntermediaria[i];
	}
}

int backtracking(struct grafo *g, int verticeAtual){		// basicamente oq ele faz é: a cada chamada recursiva atualiza uma vez o vértice, e roda todas as arestas pra cada um dos vértices (cada uma das recursivas). Enqto isso procura a melhor solução
	if(arVisitadas == g->M){									// condição de parada se passar da qnt de arestas
		if(ehSolucao(g)){
			if(melhorSolucao[g->M+1] == -1){					// se for solução vai chegar aqui, se melhorSolução ja existe, compara a nova com a melhor, ve se o peso da nova é menor ainda, substitui se for o caso
				if(ehMelhor(g))
					atualizarMelhor(g);
			}
			else												// se não tiver MelhorSolução ainda então a melhor é a unica que tem 
				atualizarMelhor(g);
			return 1;
		}	
		else
			return 0;										
	}
	
	for(int aresta = 0; aresta < g->M; aresta++){
		if(ehAceitavel(g, aresta, verticeAtual)){						// verifica se aresta é aceitável, se for, inclui na solução intermediária
			verticeAtual = aumentaCaminho(g, aresta, verticeAtual);		// da um passo pra frente, verifica se ele compõe uma solução
			if(ehSolucao(g)){
				if(melhorSolucao[g->M+1] == -1){					// se for solução vai chegar aqui, se melhorSolução ja existe, compara a nova com a melhor, ve se o peso da nova é menor ainda, substitui se for o caso
					if(ehMelhor(g))
						atualizarMelhor(g);
				}
				else												// se não tiver MelhorSolução ainda então a melhor é a unica que tem 
					atualizarMelhor(g);
			}
			backtracking(g, verticeAtual);							// chama recursivamente no novo vértice q ta do outro lado da aresta q eu to
			verticeAtual = diminuiCaminho(g, aresta, verticeAtual);				// volta atrás com o vértice e dxa o loop rodar dnv
		}
	}
}

int guloso(struct grafo *G, int verticeAtual ) {
  /* inicializa seleção de movimentos; */
  int iAresta=0;
  while ((iAresta<G->M)&&(!ehSolucao (G))) { 
	/* x = seleciona (C) ; */
	
    if ( ehAceitavel (G, iAresta, verticeAtual) ) {  //if (viavel (S + x )) 
		/* registra movimento; */ 
		verticeAtual=aumentaCaminho (G, iAresta, verticeAtual);  //  S = S + x ;
    }
    iAresta++; // C = C − x;
  } 
  if (ehSolucao(G)) {
	puts ("Achou solução");
	return 1;
  }
  puts ("Não achou solução");
  return 0;
}

int iniciaEexecuta(struct grafo *G, int verticeInicial) {
    grauDoVertice=calloc(G->N, sizeof(int)); 
	melhorSolucao=calloc(G->M+1, sizeof(int));
	solucaoIntermediaria=calloc(G->M+1, sizeof(int));
	melhorSolucao[G->M+1] = -2;							//	enquanto nao existir melhor solucao, o espaço extra é -2, qndo existir, atualiza pra -1
	verticeInicial = G->A[0];			//	nem sempre o vertice inicial é o zero, pois o grafo gera aleatório
	arVisitadas = 0;					// reinicia o valor das arestas visitadas

	int r = backtracking(G, verticeInicial);
	for (int i = 0; i < G->M; i++) {  					
		G->S[i]=melhorSolucao[i];
	}

	return r;
}

void termina() {
	free (grauDoVertice);
	free (solucaoIntermediaria);
}