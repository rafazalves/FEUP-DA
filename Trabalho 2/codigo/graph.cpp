
#define INF (INT_MAX/2)
#include "graph.h"

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph() {

}
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, int capacity,int duration) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, capacity,duration});
    if (!hasDir) nodes[dest].adj.push_back({src, capacity,duration});
}
void Graph::updateEdgeCapacity(int node1,int node2, int bottleneck){
    for(auto & it : nodes[node1].adj){
        if(it.dest == node2){
            it.capacity = it.capacity - bottleneck;
        }
    }
}
int Graph::getEdgeDuration(int node1, int node2) {
    for(auto  it : nodes[node1].adj){
        if(it.dest == node2){
            return it.duration;
        }
    }
    return -1;
}
// Depth-First Search: example implementation
void Graph::dfs(int v) {
    cout << v << " "; // show node order
    nodes[v].visited = true;
    for (auto e : nodes[v].adj) {
        int w = e.dest;
        if (!nodes[w].visited)
            dfs(w);
    }
}

// Depth-First Search with capacity
bool Graph::capacity_bfs(int start,int finish) {
    for (int v=1; v<=n; v++){
        nodes[v].visited = false;
        nodes[v].capacity = 0;
    }
    queue<int> q; // queue of unvisited nodes
    q.push(start);
    nodes[start].visited = true;
    nodes[start].capacity = INF;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        for (auto e : nodes[u].adj) {
            int w = e.dest;
            int c = min(e.capacity,nodes[u].capacity);
            if (!nodes[w].visited && c > 0) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].parent = u;
                nodes[w].capacity = c;
            }
        }
    }
    return nodes[finish].visited;
}
list<int> Graph::find_Path(int start, int finish) {
    list<int> path;
    path.push_back(finish);
    int v = finish;
    while (v != start) {
        v = nodes[v].parent;
        path.push_front(v);
    }
    return path;
}

