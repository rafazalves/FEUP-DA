
#include <fstream>
#include <iostream>
#include <vector>
#include <limits.h>
#include "da_c2.h"
#include <chrono>
#include <tuple>
#include <algorithm>
#include "graph.h"
#include "minHeap.h"



using namespace std;
list<list<int>> paths;
list<int> bottlenecks;
Graph res_Graph;
int dimension;

bool is_all_nodes_visited(Graph graph){
    for(auto it = graph.nodes.begin(); it != graph.nodes.end(); ++it){
        if(!(*it).visited){
            return false;
        }
    }
    return true;
}

bool can_visit_start_node(Graph graph, int start, int final_node_temp){
    // Verify if starting from node final_node_temp is possible to visit the start node
    for (int i = final_node_temp; i != start; i = graph.nodes[i].parent){

        if (i == 0){
            return false;
        }

    }

    return true;
}

int get_limit_capacity(Graph graph, int start, int final_node_temp){
    int capacity = INT_MAX;
    // verify min capacity value from final_node_temp to start
    for (int i = final_node_temp; i != start; i = graph.nodes[i].parent){
        if (i == 0){
            return 0;
        }

        if (graph.nodes[i].capacity < capacity){
            capacity = graph.nodes[i].capacity;
        }
    }

    if (capacity == INT_MAX){
        capacity = 0;
    }

    return capacity;



}

void dijkstra(Graph graph, int start, int finish) {
    if (start == finish){
        cout << "A partida e a chegada devem ser diferentes." << endl;
        return;
    }
    //Use dijkstra algorithm to find the shortest path maximizing the Edge capacity
    //The path is stored in the vector path

    //Will handle the answer with final best path
    vector<int> final_path;

    //For each node
    int i = start;

    while (!is_all_nodes_visited(graph)) {

        // While not all nodes are visited then continue the process
        //TEST FILES SEQUENCE: src, destination, capacity, duration
        int last_higher_index_capacity = -1;
        int last_higher_capacity = -1;
        //Iterate between all edges in adj with iterator

        for (auto it = graph.nodes[i].adj.begin(); it != graph.nodes[i].adj.end(); ++it) {
            //destination node number
            int node_index = (*it).dest;
            // i is the main node now and node_index the destination

            // This path capacity to the next possible node
            int path_capacity_to_next_node = (*it).capacity;

            // The path capacity until now
            int path_capacity_until_now = graph.nodes[i].capacity;
            //Verify capacity based on limit of parents
            int limit_capacity = get_limit_capacity(graph, start, i);
            if (path_capacity_to_next_node >= limit_capacity && can_visit_start_node(graph, start, i)){

                // If capacity is higher than actual capacity then update
                if (limit_capacity == 0){
                    graph.nodes[node_index].capacity = max(limit_capacity, path_capacity_to_next_node);
                }else{
                    graph.nodes[node_index].capacity = limit_capacity;
                }

                graph.nodes[node_index].parent = i;

            }



            if (path_capacity_to_next_node > last_higher_capacity && !graph.nodes[node_index].visited){
                // Decide next node to visit based on higher capacity node
                last_higher_capacity = path_capacity_to_next_node;
                last_higher_index_capacity = node_index;
            }

        }

        graph.nodes[i].visited = true;

        // Update to visit node with higher capacity
        if (last_higher_index_capacity == -1){
            if (!can_visit_start_node(graph, start, finish)){
                if (i == start){
                    break;
                }
                i = start;
            }

        }else{
            i = last_higher_index_capacity;
        }

        if (graph.nodes[finish].parent != 0){
            break;
        }

    }

    if (!can_visit_start_node(graph, start, finish)){
        cout << "Nao existe forma de chegar ate o destino.";
    }else{
        //final_path
        cout << "Direcao: " << start << " para " << finish << endl;
        cout << "Caminho: " << start;
        for (int i = finish; i != start; i = graph.nodes[i].parent){
            final_path.push_back(i);
        }
        reverse(final_path.begin(),final_path.end());
        for(auto& i : final_path)
            std::cout << ", " << i ;
        cout << endl;
        cout << "Capacidade Maxima: " << graph.nodes[finish].capacity;

    }

}

void print_path_l(vector<int> v){
    for(auto it = v.begin(); it != v.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
}

void print_solution(struct Solution solution){
    cout << "Path: ";
    for(auto& i : solution.path){
        cout << i << " ";
    }
    cout << endl;
    cout << "Capacity: " << solution.capacity << endl;
}

bool compare_two_solutions(struct Solution solution1, struct Solution solution2){
    if (solution1.path != solution2.path){
        return false;
    }

    if (solution1.capacity != solution2.capacity){
        return false;
    }

    return true;
}

bool verify_if_solution_in_vector_of_solutions(struct Solution solution, vector<struct Solution> solutions){
    for (auto it = solutions.begin(); it != solutions.end(); ++it){
        if (compare_two_solutions(solution, *it)){
            return true;
        }
    }
    return false;
}

vector<int> get_path(Graph graph, int start, int finish){
    vector<int> solution;
    for (int i = finish; i != start; i = graph.nodes[i].parent){
        solution.push_back(i);
    }
    reverse(solution.begin(),solution.end());
    return solution;
}

vector<struct Solution> capacity_bfs1(Graph graph, int start,int finish) {

    vector<struct Solution> solutions;

    for (int v=1; v<=graph.nodes.size(); v++){
        graph.nodes[v].visited = false;
        graph.nodes[v].capacity = 0;
    }
    queue<int> q; // queue of unvisited nodes
    q.push(start);
    graph.nodes[start].visited = true;
    graph.nodes[start].capacity = INT_MAX;

    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        for (auto e : graph.nodes[u].adj) {
            int w = e.dest;
            int c = min(e.capacity,graph.nodes[u].capacity);
            q.push(w);
            graph.nodes[w].visited = true;
            graph.nodes[w].parent = u;
            graph.nodes[w].capacity = c;

        }




        if (graph.nodes[finish].parent != 0){
            // Verify if it is a solution

            if (can_visit_start_node(graph, start, finish)){
                //Save solution
                struct Solution solution;
                solution.path = get_path(graph, start, finish);
                solution.path.insert(solution.path.begin(), start);
                solution.capacity = graph.nodes[finish].capacity;

                if (!verify_if_solution_in_vector_of_solutions(solution, solutions)){
                    solutions.push_back(solution);

                }

            }
        }


    }
    return solutions;
}


struct Solution get_solution_with_highest_capacity(vector<struct Solution> solutions){
    struct Solution solution;
    solution.capacity = 0;
    for (auto it = solutions.begin(); it != solutions.end(); ++it){
        if ((*it).capacity > solution.capacity){
            solution = *it;
        }
    }
    return solution;
}

struct Solution get_solution_with_lower_path_size(vector<struct Solution> solutions){
    struct Solution solution;
    int last_solution_size = INT_MAX;
    for (auto it = solutions.begin(); it != solutions.end(); ++it){
        if ((*it).path.size() < last_solution_size){
            solution = *it;
            last_solution_size = (*it).path.size();
        }
    }
    return solution;
}



void find_all_possible_paths_from_start_node_to_end_node(Graph graph, int start, int finish) {
    cout << "Buscando solucoes para start: " << start << " e finish: " << finish << endl;

    vector<struct Solution> solutions = capacity_bfs1(graph, start, finish);
    if (!solutions.size()){
        cout << "Nao foi encontrada nenhuma solucao para os parametros colocados." << endl;
        return;
    }

    cout << "Fora encontradas um total de " << solutions.size() << " possibilidade(s)." << endl;
    cout << "Filtrando solucoes pareto-otimas..." << endl;

    struct Solution solution_with_highest_capacity = get_solution_with_highest_capacity(solutions);
    struct Solution solution_with_lower_path_size = get_solution_with_lower_path_size(solutions);

    if (compare_two_solutions(solution_with_highest_capacity, solution_with_lower_path_size)){
        // Even if solutions > 1 maybe all of them are equal on capacity and path size
        cout << "Foi encontrada apenas uma solucao pareto-otima." << endl;
        print_solution(solution_with_highest_capacity);
        return;
    }

    cout << "Solucao com maior capacidade: " << endl;
    print_solution(solution_with_highest_capacity);

    cout << endl << endl;

    cout << "Solucao com menor tamanho do caminho: " << endl;
    print_solution(solution_with_lower_path_size);

}




bool relaxWidestPath(int node1, int node2, int weight,Graph &graph){
    if(min(graph.nodes[node1].capacity,weight) > graph.nodes[node2].capacity){
        graph.nodes[node2].capacity = min(graph.nodes[node1].capacity,weight);
        graph.nodes[node2].parent = node1;
        return true;
    }else{
        return false;
    }
}
void widestpath(Graph graph,int start,int finish){
    MinHeap<int,int>heap(graph.nodes.size()-1,0);
    for(int i = 0; i < graph.nodes.size();i++){
        graph.nodes[i].capacity = 0;
        graph.nodes[i].parent = 0;
        heap.insert(i,graph.nodes[i].capacity);
    }

    graph.nodes[start].capacity = INT16_MAX;
    heap.decreaseKey(start,INT16_MIN);
    while(heap.getSize() > 0){
        auto vertex = heap.removeMin();
        for(auto e : graph.nodes[vertex.first].adj){
            if(relaxWidestPath(vertex.first,e.dest,e.capacity,graph)){
                heap.decreaseKey(e.dest,-(graph.nodes[e.dest].capacity));
            }
        }
    }
    list<int> path = graph.find_Path(start,finish);
    paths.push_back(path);
    bottlenecks.push_back(graph.nodes[finish].capacity);

}


int read_info_from_file(string filename, Graph &graph){
    

    ifstream file_to_process;
    file_to_process.open(filename);

    if (!file_to_process.is_open()) {
        cerr << "Could not open the file " << endl;
        return 0;
    }
    int nodes;
    int edges;
    int number;
    int src;
    int destination;
    int capacity;
    int duration;
    int iteration_controller = 0;
    
    file_to_process >> number;
    nodes = number;
    file_to_process >> number;
    edges = number;
    graph =Graph(nodes, true);
    // Push the numbers to the correct vector
    while(file_to_process >> number){

        

        if (iteration_controller == 0) {
            src = number;
        } else if (iteration_controller == 1) {
            destination = number;
        } else if (iteration_controller == 2) {
            capacity = number;
        } else if (iteration_controller == 3) {
            duration = number;
            graph.addEdge(src,destination,capacity,duration);
            iteration_controller = 0;
            continue;
        }

        
        

        iteration_controller++;
    }

    return 1;
}


tuple<int,int> calculateDuration(Graph graph){
    int result_max = 0,result_min = INT16_MAX,path_duration;
    list<int> temp_path;
    int temp_node1,temp_node2;

    for(auto it: paths){
        temp_path = it;
        path_duration= 0;
        while(temp_path.size() != 1){
            temp_node1 = temp_path.front();
            temp_path.pop_front();
            temp_node2 = temp_path.front();
            path_duration += graph.getEdgeDuration(temp_node1,temp_node2);

        }
        if(result_max < path_duration){
            result_max = path_duration;
        }
        if(result_min > path_duration){
            result_min = path_duration;
        }
    }
    std::tuple<int,int> result (result_min,result_max);
    return result;
}
int EdmondsKarp(int start,int finish){
    int max_flow = 0;
    list<int> curr_path;
    int bottleneck;
    bool exit = false;
    while(res_Graph.capacity_bfs(start,finish) && !exit ) {
        curr_path = res_Graph.find_Path(start, finish);
        bottleneck = res_Graph.nodes[finish].capacity;
        if(dimension <= max_flow + bottleneck){
            bottleneck = dimension - max_flow;
            exit = true;
        }

        if(curr_path == paths.back()){
            bottlenecks.back() = bottlenecks.back() + bottleneck;
        }else{
            paths.push_back(curr_path);
            bottlenecks.push_back(bottleneck);
        }

        max_flow += bottleneck;
        for(auto it = curr_path.rbegin(); it != curr_path.rend();++it){
            res_Graph.updateEdgeCapacity(res_Graph.nodes[(*it)].parent,(*it),bottleneck);
        }
    }
    return max_flow;
}

void print_path(){
    std::cout << "The group is divided as so: " << std::endl;
    auto it1 = paths.begin();
    auto it2 = bottlenecks.begin();
    for(;it1 != paths.end() && it2 != bottlenecks.end(); ++it1,++it2){
        std::cout << (*it2) << " people go through: ";
        for(auto it3 : (*it1)){
            std::cout << it3 << " ";
        }
        std::cout << std::endl;
    }
}

void menu(Graph graph){
    int choices = 0,people_behind = 0;
    int answer;
    int flow;
    bool keep_going = true;
    int start,finish;
    while(keep_going){
        res_Graph = graph;
        paths.clear();
        bottlenecks.clear();
        dimension = INT16_MAX;
        std::cout << "Chose a scenario (1/2)" << std::endl;
        std::cin >> choices;
        while(cin.fail() || (choices != 1 && choices != 2)){
            std::cout << "The scenario must be 1 or 2:" << std::endl;
            std::cin >> choices;
        }
        if(choices == 1){
            std::cout << "1 or 2" << std::endl;
            std::cin >> choices;
            while(cin.fail() || (choices != 1 && choices != 2)){
                std::cout << "Number is 1 or 2" << std::endl;
                std::cin >> choices;
            }

            if(choices == 1){
                std::cout << "Enter the start node: " << std::endl;
                std::cin >> start;
                while(cin.fail() || start < 0 || start > graph.nodes.size()){
                    std::cout << "Must be a number between 0 and " << graph.nodes.size() << std::endl;
                    std::cin >> start;
                }
                std::cout << "Enter the finish node: " << std::endl;
                std::cin >> finish;
                while(cin.fail() || finish < 0 || finish > graph.nodes.size()){
                    std::cout << "Must be a number between 0 and " << graph.nodes.size() << std::endl;
                    std::cin >> finish;
                }




                widestpath(graph,start,finish);
                print_path();


            }else if(choices == 2){
                std::cout << "Enter the start node: " << std::endl;
                std::cin >> start;
                while(cin.fail() || start < 0 || start > graph.nodes.size()){
                    std::cout << "Must be a number between 0 and " << graph.nodes.size() << std::endl;
                    std::cin >> start;
                }
                std::cout << "Enter the finish node: " << std::endl;
                std::cin >> finish;
                while(cin.fail() || finish < 0 || finish > graph.nodes.size()){
                    std::cout << "Must be a number between 0 and " << graph.nodes.size() << std::endl;
                    std::cin >> finish;
                }

                find_all_possible_paths_from_start_node_to_end_node(graph, start, finish);

            }
        }else{
            std::cout << "1 or 3" << std::endl;
            std::cin >> choices;
            while(cin.fail() || (choices != 1 && choices != 3)){
                std::cout << "Number must be 1 or 3! Please enter another value: " << std::endl;
                std::cin >> choices;
            }

            std::cout << "Start node: " << std::endl;
            std::cin >> start;
            std::cout << "Finish node: " << std::endl;
            std::cin>> finish;
            while(cin.fail() || (start >= graph.nodes.size()) && finish >= graph.nodes.size()){
                if(start >= graph.nodes.size()){
                    std::cout << "Start exceeds maximum value!" << std::endl << "Please enter another value for start: " << std::endl;
                    std::cin >> start;

                }
                if(finish >= graph.nodes.size()){
                    std::cout << "Finish exceeds maximum value!" << std::endl << "Please enter another value for finish: " << std::endl;
                    std::cin >> finish;
                }
            }
            if(choices == 1){
                res_Graph = graph;
                std::cout << "What is the dimension of the group? " << std::endl;
                cin >> dimension;

                while(cin.fail()){
                    std::cout << "Dimension must be a number! Please enter another value: " << std::endl;
                    cin >> dimension;
                }

                flow = EdmondsKarp(start,finish);

                print_path();
                if(dimension > flow){
                    people_behind += (dimension - flow);
                    std::cout << people_behind << " people got left behind!" << std::endl;
                }




            }
            if(choices == 3){
                res_Graph = graph;
                flow = EdmondsKarp(start,finish);
                std::cout << "The max dimension of the group is : " << flow << std::endl;
                print_path();


            }
            while (keep_going){
                std::cout << "Do you want to make more operations relative to this path?(0-no/1-yes) " << std::endl;
                std::cin >> keep_going;
                while(cin.fail() || (keep_going != 0 && keep_going != 1)){
                    std::cout << "It must be a 0 or 1! Please enter another value: " << std::endl;
                    std::cin >> keep_going;
                }
                std::cout << "--------------------------------------------------------" << std::endl;
                std::cout << "| 1 - Add people to the group                          |" << std::endl;
                std::cout << "| 2 - See when people will encounter each other        |" << std::endl;
                std::cout << "| 3 - How much time will the first group to arrive wait|" << std::endl;
                std::cout << "| 4 - Exit                                             |" << std::endl;
                std::cout << "--------------------------------------------------------" << std::endl;
                std::cout << "Please chose an option: " << std::endl;
                cin >> answer;
                while(cin.fail() || (answer != 1 && answer != 2 && answer != 3 && answer != 4)){
                    std::cout << "It must be 1 or 0! Please enter another value: " << std::endl;
                    std::cin >> answer;
                }
                if(answer == 1){
                    std::cout << "Please enter how many people you want to add to the group: " << std::endl;
                    cin >> dimension;
                    while(cin.fail()){
                        std::cout << "Dimension must be a number! Please enter another value: " << std::endl;
                        std::cin >> dimension;
                    }
                    flow = EdmondsKarp(start,finish);
                    print_path();
                    if(dimension > flow){
                        people_behind += (dimension - flow);
                        std::cout << people_behind << " people got left behind!" << std::endl;
                    }
                }else if(answer == 2){

                    std::cout << "People will encounter after " << get<1>(calculateDuration(graph)) << " minutes." << std::endl;

                }else if(answer == 3){
                    std::cout << "The first group to arrive will have to wait " << get<1>(calculateDuration(graph)) - get<0>(calculateDuration(graph)) << " minutes at location " << finish << "." << std::endl;
                }
            }




        }


        std::cout << "Do you want to find a new path?(0-no/1-yes): " << std::endl;
        std::cin >> keep_going;
        while(cin.fail() || (keep_going != 0 && keep_going != 1)){
            std::cout << "It must be a 0 or 1! Please enter another value: " << std::endl;
            std::cin >> keep_going;
        }

    }

}






int main(int argc, char** argv){
    Graph graph;
    string test_file;

    std::cout << "What file do you want to open? " << std::endl;
    std::cin >> test_file;
    if(!read_info_from_file(test_file,graph)){
        exit(0);
    };
    menu(graph);

    return 0;
}