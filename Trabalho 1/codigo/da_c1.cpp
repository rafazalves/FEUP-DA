#include <fstream>
#include <iostream>
#include <vector>
#include "da_c1.h"
#include <chrono>
#include "algorithm"
using namespace std;

vector<int> carrinhas_volume_max;
vector<int> carrinhas_peso_max;
vector<int> carrinhas_custo;

vector<int> encomendas_volume;
vector<int> encomendas_peso;
vector<int> encomendas_recompensa;
vector<int> encomendas_duracao;

int read_info_from_file(string filename, int filetype){
    // filetype = 1 -> carrinhas
    // filetype = 2 -> encomendas

    ifstream file_to_process;
    file_to_process.open(filename);

    if (!file_to_process.is_open()) {
        cerr << "Could not open the file " << endl;
        return 0;
    }

    int number;
    string p;
    int iteration_controller = 1;
    int max_n = filetype == 2 ? 5 : 4;

    // Remove header from txt
    while (iteration_controller < max_n) {
        file_to_process >> p;
        iteration_controller++;
    }

    iteration_controller = 0;

    // Push the numbers to the correct vector
    while(file_to_process >> number){

        if (filetype == 2){

            if (iteration_controller == 0) {
                encomendas_volume.push_back(number);
            } else if (iteration_controller == 1) {
                encomendas_peso.push_back(number);
            } else if (iteration_controller == 2) {
                encomendas_recompensa.push_back(number);
            } else if (iteration_controller == 3) {
                encomendas_duracao.push_back(number);
                iteration_controller = 0;
                continue;
            }

        }else if (filetype == 1){

            if (iteration_controller == 0) {
                carrinhas_volume_max.push_back(number);
            } else if (iteration_controller == 1) {
                carrinhas_peso_max.push_back(number);
            } else if (iteration_controller == 2) {
                carrinhas_custo.push_back(number);
                iteration_controller = 0;
                continue;
            }

        }

        iteration_controller++;
    }

    return 1;
}





bool compare_encomenda_by_peso_and_volume(struct Encomenda e1, struct Encomenda e2){

    return e1.peso+e1.volume < e2.peso+e2.volume;
}

bool compare_carrinha_by_higher_pesomax_and_volumemax(struct Carrinha c1, struct Carrinha c2){

    return c1.pesoMax+c1.volMax > c2.pesoMax+c2.volMax;
}


bool compare_indice_encomenda(struct Encomenda e1, struct Encomenda e2){
    return ((e1.recompensa)/(e1.peso+e1.volume) > (e2.recompensa)/(e2.peso+e2.volume));
}
bool compare_indice_carrinha(struct Carrinha c1, struct Carrinha c2){
    return ((c1.custo)/(c1.pesoMax+c1.volMax) < (c2.custo)/(c2.pesoMax+c2.volMax));
}




void cenario1(){
    auto start = std::chrono::high_resolution_clock::now();
    vector<struct Carrinha> carrinhas_disponiveis;
    vector<struct Encomenda> encomendas_a_entregar;
    int total_carrinhas = carrinhas_volume_max.size();
    int total_encomendas = encomendas_volume.size();

    // Create carrinhas
    for (int i = 0; i < total_carrinhas; i++) {
        struct Carrinha carrinha;
        carrinha.volMax = carrinhas_volume_max[i];
        carrinha.pesoMax = carrinhas_peso_max[i];
        carrinha.custo = carrinhas_custo[i];
        carrinha.is_assigned = false;
        carrinhas_disponiveis.push_back(carrinha);
    }

    // Create encomendas
    for (int i = 0; i < total_encomendas; i++) {
        struct Encomenda encomenda;
        encomenda.volume = encomendas_volume[i];
        encomenda.peso = encomendas_peso[i];
        encomenda.recompensa = encomendas_recompensa[i];
        encomenda.duracao = encomendas_duracao[i];
        encomendas_a_entregar.push_back(encomenda);
    }

    sort(encomendas_a_entregar.begin(), encomendas_a_entregar.end(), compare_encomenda_by_peso_and_volume);
    sort(carrinhas_disponiveis.begin(), carrinhas_disponiveis.end(), compare_carrinha_by_higher_pesomax_and_volumemax);

    int encomendas_a_entregar_no_dia = 0;
    // fit all encomendas to carrinhas
    for (int i = 0; i < total_encomendas; i++) {
        // find the first carrinha that can fit the encomenda
        for (int j = 0; j < total_carrinhas; j++) {
            // if carrinha is not full
            if (carrinhas_disponiveis[j].volMax >= encomendas_a_entregar[i].volume && carrinhas_disponiveis[j].pesoMax >= encomendas_a_entregar[i].peso) {
                // if carrinha is not already assigned
                carrinhas_disponiveis[j].encomendas.push_back(i);
                carrinhas_disponiveis[j].volMax -= encomendas_a_entregar[i].volume;
                carrinhas_disponiveis[j].pesoMax -= encomendas_a_entregar[i].peso;
                carrinhas_disponiveis[j].is_assigned = true;
                encomendas_a_entregar_no_dia++;
                break;
            }
        }
    }
        
    //Verify how many carrinhas are assigned
    int total_carrinhas_assigned = 0;
    for (int i = 0; i < total_carrinhas; i++) {
        if (carrinhas_disponiveis[i].is_assigned) {
            total_carrinhas_assigned++;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cout << total_carrinhas_assigned << " carrinhas foram atribuidas para realizar o trabalho de um total de " << total_carrinhas << " carrinhas." << endl;
    cout << "O tempo de execução do cenário 1 é " << duration.count() << "." << endl;
    float eficiencia = ((float)encomendas_a_entregar_no_dia) / total_encomendas;

cout << "Relativamente à funcionalidade extra Medir a eficiência da operação da empresa, em termos do quociente entre o número de pedidos efetivamente entregues e o número de pedidos recebidos num dia, podemos afirmar que a empresa teve um quociente de " << eficiencia << " com o cenário 1.\n\n\n";
}


void cenario2(){
    auto start = std::chrono::high_resolution_clock::now();
    vector<struct Carrinha> carrinhas_disponiveis;
    vector<struct Encomenda> encomendas_a_entregar;
    int total_carrinhas = carrinhas_volume_max.size();
    int total_encomendas = encomendas_volume.size();
    // Create carrinhas
    for (int i = 0; i < total_carrinhas; i++) {
        struct Carrinha carrinha;
        carrinha.volMax = carrinhas_volume_max[i];
        carrinha.pesoMax = carrinhas_peso_max[i];
        carrinha.custo = carrinhas_custo[i];
        carrinha.is_assigned = false;
        carrinhas_disponiveis.push_back(carrinha);
        carrinha.lucro = 0 - carrinha.custo;

    }

    // Create encomendas
    for (int i = 0; i < total_encomendas; i++) {
        struct Encomenda encomenda;
        encomenda.volume = encomendas_volume[i];
        encomenda.peso = encomendas_peso[i];
        encomenda.recompensa = encomendas_recompensa[i];
        encomenda.duracao = encomendas_duracao[i];
        encomendas_a_entregar.push_back(encomenda);
    }

    sort(encomendas_a_entregar.begin(), encomendas_a_entregar.end(), compare_indice_encomenda);
    sort(carrinhas_disponiveis.begin(), carrinhas_disponiveis.end(), compare_indice_carrinha);

    float encomendas_a_entregar_no_dia = 0;
    int recompensa_sum = 0;
    // fit all encomendas to carrinhas
    for (int i = 0; i < total_encomendas; i++) {
        // find the first carrinha that can fit the encomenda
        for (int j = 0; j < total_carrinhas; j++) {
            // if carrinha is not full
            if (carrinhas_disponiveis[j].volMax >= encomendas_a_entregar[i].volume && carrinhas_disponiveis[j].pesoMax >= encomendas_a_entregar[i].peso) {
                // if carrinha is not already assigned
                carrinhas_disponiveis[j].encomendas.push_back(i);
                carrinhas_disponiveis[j].volMax -= encomendas_a_entregar[i].volume;
                carrinhas_disponiveis[j].pesoMax -= encomendas_a_entregar[i].peso;
                carrinhas_disponiveis[j].is_assigned = true;
                encomendas_a_entregar_no_dia++;
                carrinhas_disponiveis[j].lucro += encomendas_a_entregar[i].recompensa;
                break;
            }
        }
    }
    int c = 0;

    int profit = 0;
    for (int c = 0; c <= total_carrinhas; c++) {
        if (carrinhas_disponiveis[c].lucro >= 0) {
            profit += carrinhas_disponiveis[c].lucro;
        }
        else {
            encomendas_a_entregar_no_dia -= carrinhas_disponiveis[c].encomendas.size();
            carrinhas_disponiveis[c].encomendas.clear();
        }

    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "A empresa tem " << profit << " euros de lucro com esta distribuição." << endl;
    cout << "O tempo de execução do cenário 2 é " << duration.count() << "." << endl;

    float eficiencia = ((float)encomendas_a_entregar_no_dia) / total_encomendas;

    cout << "Relativamente à funcionalidade extra Medir a eficiência da operação da empresa, em termos do quociente entre o número de pedidos efetivamente entregues e o número de pedidos recebidos num dia, podemos afirmar que a empresa teve um quociente de " << eficiencia << " com o cenário 2.\n\n\n";
}



void cenario3() {
    auto start = std::chrono::high_resolution_clock::now();

    int tmpmedWITHorder;
    int somatmpWITHOUTorder = 0;
    int somatmpWITHorder = 0;
    int numencomendas_entregarWITHOUTorder = 0;
    int numencomendas_entregarWITHorder = 0;
    int horarioEMP = (60 * 60) * 8;
    int soma = 0;

    for (int i = 0; i < encomendas_duracao.size() && soma < horarioEMP; i++) {
        if (soma + encomendas_duracao[i] <= horarioEMP) {
            soma += encomendas_duracao[i];
            somatmpWITHOUTorder += soma;
            numencomendas_entregarWITHOUTorder += 1;
        }
        else {
            soma = horarioEMP;
        }
    }
    soma = 0;

    sort(encomendas_duracao.begin(), encomendas_duracao.end());

    for (int i = 0; i < encomendas_duracao.size() && soma < horarioEMP; i++) {
        if (soma + encomendas_duracao[i] <= horarioEMP) {
            soma += encomendas_duracao[i];
            somatmpWITHorder += soma;
            numencomendas_entregarWITHorder += 1;
        }
        else {
            soma = horarioEMP;
        }
    }

    tmpmedWITHorder = somatmpWITHorder / numencomendas_entregarWITHorder;

    float eficiencia = (float)numencomendas_entregarWITHorder / encomendas_duracao.size();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cout << "Serão entregues " << numencomendas_entregarWITHOUTorder << " encomendas no dia, antes da ordenação das encomendas.\n";

    cout << "Serão entregues " << numencomendas_entregarWITHorder << " encomendas no dia, depois da ordenação das encomendas.\n";

    cout << "O tempo médio previsto das entregas expresso, depois da ordenação das encomendas é " << tmpmedWITHorder << " segundos.\n";

    cout << "O tempo de execução do cenário 3 é " << duration.count() << "." << endl;

    cout << "Relativamente à funcionalidade extra Medir a eficiência da operação da empresa, em termos do quociente entre o número de pedidos efetivamente entregues e o número de pedidos recebidos num dia, podemos afirmar que a empresa teve um quociente de " << eficiencia << " com o cenário 3.\n";
   
}


int main(int argc, char** argv){
    
    if(argc != 4){
        cout << "Número de argumentos inválido.\n";
        cout << "Utilize: ./a.out <ficheiro_de_entrada_encomendas> <ficheiro_de_entrada_carrinhas> <cenario>\n";
        return 0;
    }

    string encomendas_file = argv[1];
    string carrinhas_file = argv[2];
    read_info_from_file(encomendas_file, 2);
    read_info_from_file(carrinhas_file, 1);



    int cenario = atoi(argv[3]);
    if (cenario == 1){
        cenario1();
    }else if(cenario == 2){
        cenario2();
    }else if(cenario == 3){
        cenario3();
    }else{
        cout << "Cenário inválido.\n";
        return 0;
    }

    return 0;
    }