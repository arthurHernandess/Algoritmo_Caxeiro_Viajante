/*
 * @brief Corpos das funções declaradas em grafo.h.
 * 
 * Eu queria deixar "o que faz" no header e "como faz" no .c, mas o gerador
 * de documentos (doxygen), quando documenta o header, concatena a documentação
 * documentação do header com a documentação do .c, o que duplica muitas 
 * explicações, aumenta o tamanho da documentação, o que desmotiva sua leitura. 
 * 
 * Por isso, o que puder ser documentado no header ou no .c será documentado
 * somente no header.
 * 
 * @author Fábio Nakano
 * 
 * início 2021-08-31 21:00
 * só falta o backtracking 2021-08-31 21:40
 * backtracking feito e testado 2021-09-09 16:43
 * só falta documentar
 * começando com doxygen em 2021-09-09 19:00
 * Acrescentei array para armazenar resposta no próprio grafo em 2021-09-13
 */

#include "grafo.h"

struct grafo *criaGrafo (int nV, int nA) {
	struct grafo *g = (struct grafo *) malloc (sizeof(struct grafo));
	g->N=nV;
	g->M=nA;
	g->A=malloc (3*sizeof(int)*nA);
	g->S=malloc (sizeof(int)*nA);

	/* teste pessoal q cria um grafo específico */
	// int arestas[20][3] = {
    //     {0, 1, 41}, {1, 2, 467}, {2, 3, 334}, {3, 4, 500}, {4, 5, 169},
    //     {5, 6, 724}, {6, 7, 478}, {7, 8, 358}, {8, 9, 962}, {9, 0, 464},
    //     {2, 3, 40}, {7, 1, 491}, {5, 2, 827}, {6, 1, 604}, {2, 3, 292},
    //     {2, 1, 716}, {8, 5, 447}, {6, 1, 538}, {9, 0, 300}, {9, 5, 894}
    // };

    // for (int i = 0; i < g->M; i++) {
    //     g->A[3 * i] = arestas[i][0];      
    //     g->A[3 * i + 1] = arestas[i][1];  
    //     g->A[3 * i + 2] = arestas[i][2];  
    // }

    return g;
}

void destroiGrafo (struct grafo *g) {
	free (g->A);
	free (g->S);
	free (g);
}

void imprimeGrafo (const struct grafo *g) {
	printf ("número de vértices=%d\n", g->N);
	printf ("número de arestas=%d\n", g->M);
	for (int i=0;i<g->M;i++) {
		printf ("%d - %d: %d\n", g->A[0+3*i], g->A[1+3*i], g->A[2+3*i]);
	}
}

void imprimeGrafoMermaid (const struct grafo *g, const int *destacarAresta) {
	char *arestaNormal="-.-";
	char *arestaDestacada="---";
	printf ("número de vértices=%d\n", g->N);
	printf ("número de arestas=%d\n", g->M);
	puts ("graph TD");
	if (destacarAresta==NULL) {
		arestaNormal=arestaDestacada;
	}
	for (int i=0;i<g->M;i++) {
		if (destacarAresta && (destacarAresta[i])) {
			printf ("%d %s|%d| %d\n", g->A[0+3*i], arestaDestacada, g->A[2+3*i], g->A[1+3*i]);
		} else {
			printf ("%d %s|%d| %d\n", g->A[0+3*i], arestaNormal, g->A[2+3*i], g->A[1+3*i]);
		}
	}
}

void preencheAleatorio (struct grafo *g) {
	int o, d, p;
	for (int i=0;i<g->M;i++) {
		o=rand()%g->N;
		d=rand()%g->N;
		p=rand()%1000;
		g->A[0+3*i]=o;
		g->A[1+3*i]=d;
		g->A[2+3*i]=p;
	}
}

void trocaRotulo (struct grafo *g, int antigo, int novo) {
	for (int i=0;i<g->M;i++) {
		if (g->A[0+3*i]==antigo) g->A[0+3*i]=novo;
		if (g->A[1+3*i]==antigo) g->A[1+3*i]=novo;
	}
}

void preencheHamiltonianoAleatorio (struct grafo *g, int mudaOrdem, int mudaRotulo) {
	int o, d, p;
	if (g->M<g->N) {
		puts ("Abortando, arestas insuficientes para conter grafo com circuito hamiltoniano");
		exit (0);
	}
	for (int i=0;i<g->N;i++) {
		o=i;
		d=(i+1)%g->N;
		p=rand()%1000;
		g->A[0+3*i]=o;
		g->A[1+3*i]=d;
		g->A[2+3*i]=p;
	}
	for (int i=g->N;i<g->M;i++) {
		// insere na ordem
		o=rand()%g->N;
		d=rand()%g->N;
		p=rand()%1000;
		g->A[0+3*i]=o;
		g->A[1+3*i]=d;
		g->A[2+3*i]=p;
	}
	if (mudaOrdem) {
		/* mistura arestas */
		o=g->A[0];
		d=g->A[1];
		p=g->A[2];
		int ante=0;
		int j;
		for (int i=0;i<g->M*10;i++) {
			j=rand()%g->M;
			g->A[0+3*ante]=g->A[0+3*j];
			g->A[1+3*ante]=g->A[1+3*j];
			g->A[2+3*ante]=g->A[2+3*j];
			ante=j;
		}
		g->A[0+3*j]=o;
		g->A[1+3*j]=d;
		g->A[2+3*j]=p;
	}
	if (mudaRotulo) {
 		trocaRotulo (g, 0, -1);

		int novoRotulo=0;
		int antigoRotulo=0;
		int i=0;
		while (i<g->N) {
			antigoRotulo=rand()%g->N;
			trocaRotulo (g, antigoRotulo, novoRotulo);
			novoRotulo=antigoRotulo;
			i++;
		}
 		trocaRotulo (g, -1, novoRotulo);
	}
}

#if 0
void main (void) {
	struct grafo *G;
	G=criaGrafo(10,20);
	//preencheAleatorio (G);
	preencheHamiltonianoAleatorio (G);
	imprimeGrafoMermaid(G);
}
#endif


// gcc grafo.c ham.c
