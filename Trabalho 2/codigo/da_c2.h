/**
* @file
* @author Gabriel Rocco
* @version 1.0
* @date 2018-06-10
*
* Segundo trabalho de Desenho de Algoritmos
*/


#ifndef DA_C2_H
#define DA_C2_H
#include <string>
#include "graph.h"


// Read graph from file
/**
 * @brief
 *
 * @param filename
 * @param graph
 * @return int
 */
int read_info_from_file(std::string filename, Graph &graph);

struct Solution{
    vector<int> path;
    int capacity;
};


/**
 * @brief Função que verifica se o vértice inicial pode ser visitado a partir do vértice final em questão
 * @param graph Grafo
 * @param source Vértice de origem
 * @param target Vértice de destino
 * @return Retorna a solução
 */
bool can_visit_start_node(Graph graph, int start, int final_node_temp);


/**
 * @brief Função que imprime o caminho
 * @param solution Vector of ints that represents path based on solution
 */
void print_solution(struct Solution solution);



/**
 * @brief Função que compara duas soluções
 * @param solution1 Solução 1
 * @param solution2 Solução 2
 * @return Retorna true se as soluções forem iguais
 */
bool compare_two_solutions(struct Solution solution1, struct Solution solution2);


/**
 * @brief Função que verifica se uma solução está na lista de soluções
 * @param solutions Lista de soluções
 * @param solution Solução
 * @return Retorna true se a solução está na lista de soluções
 */
bool verify_if_solution_in_vector_of_solutions(struct Solution solution, vector<struct Solution> solutions);



/*+
    * @brief Função que retorna o caminho completo
    * @param graph Grafo
    * @param source Vértice de origem
    * @param target Vértice de destino
    * @return Retorna o caminho completo
*/
vector<int> get_path(Graph graph, int start, int finish);


/**
 * @brief Função que retorna a solução usando o algoritmo de busca em largura
 * @param graph Grafo
 * @param source Vértice de origem
 * @param target Vértice de destino
 * @return Retorna a solução
 */
vector<struct Solution> capacity_bfs1(Graph graph, int start,int finish);



/**
 * @brief Função que retorna a solução com a capacidade máxima
 * @param solutions Lista de soluções
 * @return Retorna a solução com a capacidade máxima
 */
struct Solution get_solution_with_highest_capacity(vector<struct Solution> solutions);


/**
 * @brief Função que retorna a solução com o menor caminho
 * @param solutions Lista de soluções
 * @return Retorna a solução com o menor caminho
 */
struct Solution get_solution_with_lower_path_size(vector<struct Solution> solutions);


/**
 * @brief Função que retorna a solução usando o algoritmo de busca em profundidade
 * @param graph Grafo
 * @param source Vértice de origem
 * @param target Vértice de destino
 * @return Retorna a solução
 */
void find_all_possible_paths_from_start_node_to_end_node(Graph graph, int start, int finish);

tuple<int,int> calculateDuration(Graph graph);


/**
 * @brief Função que retorna a solução usando o algoritmo de Edmonds-Karp
 * @param graph Grafo
 * @param source Vértice de origem
 * @param target Vértice de destino
 * @return Retorna a solução
 */
int EdmondsKarp(int start,int finish);



/**
 * @brief Função que imprime o caminho
 */
void print_path();


/**
 * @brief Imprime o Menu na tela
 */
void menu(Graph graph);

#endif // !DA_C2_H