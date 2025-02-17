/** @file ep.h
 * @brief Constantes, estruturas, protótipos de funções essenciais (mandatórias) 
 * para o EP.
 * 
 * Os corpos das funções para o exemplo estão em ep.c. O exemplo implementa
 * uma solução gulosa que tenta encontrar um circuito hamiltoniano em um grafo.
 * O problema é uma variação do Problema do Caixeiro Viajante.
 * 
 * No EP, espera-se que o aluno implemente uma solução por tentativa e erro
 * (backtracking) para esta variação do Problema do Caixeiro Viajante.
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
 * @date 2021-09-14
 * 
 */
#ifndef EP_H
#define EP_H

#include "grafo.h"

/** @brief Esta função é importante para que o corretor automático inclua 
 * corretamente a autoria do EP. Deve retornar Nome e NUSP separados por 
 * ponto-e-virgula, sem pular linhas no meio nem no final.
 * @return Nome e NUSP separados por ponto-e-virgula, sem pular linhas.
 */
char *autor (void);

/** @brief Aloca e inicializa variáveis e executa o algoritmo.
 * 
 * @param G Grafo onde procurar o circuito hamiltoniano e onde, também, 
 * armazenar o resultado, como arestas selecionadas, na lista S. Ver grafo.h
 * para detalhes.
 * @param verticeInicial Vértice inicial.
 * @return retorna 1 caso tenha achado o circuito hamiltoniano e zero caso não 
 * tenha achado.
 */
int iniciaEexecuta(struct grafo *G, int verticeInicial);

/** @brief Libera memória e toma outras ações para limpeza do ambiente de
 * execução.
 */
void termina();

#endif
