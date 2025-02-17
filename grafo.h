/** @file grafo.h
 * @brief Constantes, estruturas, protótipos de funções para representação, 
 * armazenamento e manipulação de grafos. 
 * 
 * Os corpos das funções estão em grafo.c.
 * 
 * 
 * @author Fábio Nakano
 */
#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>

/** @struct grafo
 * @brief Estrutura para representação de um grafo.
 * 
 * Os vértices são rotulados por números inteiros no intervalo [0:N-1], 
 * sem "pulos".
 * 
 * A lista de arestas é uma matriz [M, 3] onde cada linha corresponde 
 * a uma aresta. Dada a linha, o primeiro elemento é o rótulo do vértice de 
 * origem, o segundo elemento é o rótulo do vértice de destino e o terceiro
 * elemento é um valor associado à aresta, por exemplo, seu "peso".
 * Espera-se que todas as linhas da lista contenham arestas válidas.
 * 
 * No EP, a aresta pode ser percorrida tanto da origem para o destino quanto
 * do destino para a origem. Em outras palavras, o grafo não é dirigido.
 * 
 * A lista de flags de seleção de arestas é uma lista que indica se a aresta
 * contida na lista de arestas foi ou não selecionada. Se a aresta não foi 
 * selecionada, o valor armazenado na posição deve ser 0 (zero), se a aresta foi
 * selecionada, o valor deve ser diferente de 0. Esta lista foi acrescentada
 * por conveniência para retornar a resposta da execução de algoritmos.
 */ 
struct grafo {
	int N;  /**< Número de vértices */
	int M;  /**< Número de arestas */
	int *A; /**< ponteiro para lista de arestas */
	int *S; /**< ponteiro para lista de flags de seleção de aresta */
};

/** @brief Aloca estruturas na memória para armazenar um grafo.
 * 
 * Aloca memória para uma estrutura, inicializa a quantidade de vértices e 
 * de arestas e aloca a lista de arestas. Esta não é inicializada.
 * @param nV número de vértices do grafo;
 * @param nA número de arestas do grafo. 
 * @return ponteiro para a estrutura criada. */
struct grafo *criaGrafo (int nV, int nA);

/** @brief Libera memória ocupada pelo grafo. 
 * 
 * Libera a memória alocada para a lista de arestas e para a estrutura. 
 * @param g ponteiro para a estrutura que armazena o grafo.
 */
void destroiGrafo (struct grafo *g);

/** @brief Imprime grafo, em formato próprio, na tela.
 * 
 * @param g ponteiro para grafo a imprimir
 */
void imprimeGrafo (const struct grafo *g);

/** @brief Imprime grafo em formato Mermaid, na tela.
 * 
 * Mermaid (https://mermaid-js.github.io/) é um gerador de diagramas.
 * Tem um renderizador on-line em 
 * https://github.com/mermaid-js/mermaid-live-editor 
 * 
 * A idéia de uso é: copiar, ou do console ou de arquivo (gerado através de 
 * redirecionamento de saída), e colar no mermaid live editor.
 * 
 * Caso uma lista de arestas a destacar seja fornecida, ela é usada para 
 * destacar arestas. A lista deve ter o mesmo comprimento da lista de arestas do
 * grafo. Se o elemento correspondente à aresta contiver 0 então a aresta é 
 * desenhada com linha pontilhada, se contiver !=0 então a aresta é desenhada
 * com linha cheia.
 * Caso o ponteiro para a lista valha NULL, entende-se que a lista não foi 
 * fornecida e todas as arestas são desenhadas com linha cheia.
 * 
 * Esta função não mostra vértices de grau zero.
 * Não sei se mermaid permite vértices de grau zero.
 * 
 * 
 * @param g ponteiro para grafo a imprimir
 * @param destacarArestas ponteiro para lista de arestas a destacar, ou NULL.
 */
void imprimeGrafoMermaid (const struct grafo *g, const int *destacarAresta);

/** @brief Preenche a estrutura com um grafo aleatório.
 *  
 * Em um grafo criado (estruturas alocadas e de tamanho definido), preenche 
 * com a quantidade definida de arestas. Estas recebem origem, destino e peso 
 * aleatórios. Origem e destino no intervalo [0:N-1] e peso no 
 * intervalo [0:1000]
 */
void preencheAleatorio (struct grafo *g);

/** @brief Troca o rótulo (antigo) de um vértice por outro (novo).
 * 
 * Função de apoio para preencheHamiltonianoAleatorio. 
 * A troca de um rótulo por outro já existente altera o grafo. A alteração não
 * pode ser revertida pela troca inversa.
 * 
 * Segundo esta especificação do grafo, os vértices são rotulados com números
 * de [0:N-1]. É muito provável que esta função precise ser aplicada várias
 * vezes, com parâmetros diferentes, passando por estados que não obedecem a
 * especificação, para que, ao final da aplicação do conjunto, chegue-se ao 
 * ajuste desejado E que segue a especificação.
 * @param g Ponteiro para o grafo que contém o vértice;
 * @param antigo rótulo do vértice.
 * @param novo Novo rótulo do vértice.
 */
void trocaRotulo (struct grafo *g, int antigo, int novo);

/**  @brief Preenche a estrutura com um circuito Hamiltoniano e completa com 
 * arestas geradas aleatoriamente.
 * 
 * Em um grafo criado (estruturas alocadas e de tamanho definido), preenche com
 * um circuito hamiltoniano e completa o número de arestas com origem destino e
 * peso aleatórios.
 * 
 * Caso haja menos arestas que o necessário para armazenar o circuito, não 
 * preenche e retorna uma mensagem de erro.
 * 
 * As arestas do caminho são geradas sequencialmente: 0-1, 1-2, 2-3, ... O 
 * circuito é completado por uma aresta do último para o primeiro vértice.
 * 
 * Na solução do Problema do Caixeiro Viajante por tentativa e erro 
 * (backtracking), isto favorece que algoritmos errados gerem soluções corretas.
 * Por isso foram implementadas rotinas para permutar as arestas aleatoriamente
 * e para trocar os rótulos dos vértices aleatoriamente.
 * 
 * @param g Ponteiro para o grafo a preencher;
 * @param mudaOrdem se 0 não muda ordem, se !=0 muda ordem das arestas na lista 
 *        de arestas (A).
 * @param mudaRotulo se 0 não muda rótulos, se !=0 muda rótulos dos vértices, 
 *        preservando a forma do grafo (o grafo resultante é isomórfo ao grafo
 *        inicial. 
 *        Ao final, preserva a propriedade de que os rótulos dos vértices estão 
 *        no intervalo [0:N-1]
 */
void preencheHamiltonianoAleatorio (struct grafo *g, int mudaOrdem, int mudaRotulo);

#endif
