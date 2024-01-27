/**
* @file
* @author Gabriel Rocco
* @version 1.0
* @date 2018-06-10
*
* Primeiro trabalho de Desenho de Algoritmos
*/


#ifndef DA_C1_H
#define DA_C1_H


/**
 * @brief Read the input file and store the data in the data structure.
 *
 * Open files that stores data of carrinhas and encomendas and stores it to global variables. So it can be used on all solutions.
 * @param carrinhas_file_name Name of the file that stores carrinhas data.
 * @param encomendas_file_name Name of the file that stores encomendas data.
 */
int read_info_from_file(std::string filename, int filetype);

/**
 * @brief Algorithm for the first scenario.
 * 
 * Algorithm for the first scenario.
 */
void cenario1();


/**
 * @brief Compare encomenda
 *
 * Compare encomenda based on the weight and volume.
 * @param e1 First encomenda.
 * @param e2 Second encomenda.
 */
bool compare_encomenda_by_peso_and_volume(struct Encomenda e1, struct Encomenda e2);
/**
 * @brief Compare carrinha
 *
 * Compare carrinha based on the max weight and max volume.
 * @param c1 First carrinha.
 * @param c2 Second carrinha.
 */
bool compare_carrinha_by_higher_pesomax_and_volumemax(struct Carrinha c1, struct Carrinha c2);

/**
 * @brief Algorithm for the second scenario.
 * 
 * Algorithm for the second scenario.
 */
void cenario2();

/**
 * @brief Compare encomenda
 *
 * Compare encomenda based on the weight and volume.
 */
bool compare_indice_encomenda(struct Encomenda e1, struct Encomenda e2);
/**
 * @brief Compare carrinha
 *
 * @param c1 First carrinha.
 * @param c2 Second carrinha.
 */
bool compare_indice_carrinha(struct Carrinha c1, struct Carrinha c2);


/**
 * @brief Algorithm for the third scenario.
 * 
 * Algorithm for the third scenario.
 */
void cenario3();

struct Encomenda {
    int volume;
    int peso;
    int recompensa;
    int duracao;
};


struct Carrinha {
    int volMax;
    int pesoMax;
    int custo;
    int lucro;
    bool is_assigned;
    std::vector<int> encomendas;
};

#endif // !DA_C1_H