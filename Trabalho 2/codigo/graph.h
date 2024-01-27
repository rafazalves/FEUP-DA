
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <list>
#include <queue>
#include <iostream>

using namespace std;

class Graph {

    struct Edge {
        int dest;   // Destination node
        int capacity; // An integer capacity
        int duration;
        int flow = 0;
    };

    struct Node {
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        bool visited;// As the node been visited on a search?
        int capacity;
        int parent;

    };

    int n;              // Graph size (vertices are numbered from 1 to n)
    bool hasDir;        // false: undirect; true: directed
     // The list of nodes being represented

public:
    vector<Node> nodes;
    Graph();
    // Constructor: nr nodes and direction (default: undirected)
    Graph(int nodes, bool dir = false);

    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, int capacity,int duration);

    // Depth-First Search: example implementation
    void dfs(int v);

    // Breadth-First Search: example implementation
    bool capacity_bfs(int start,int finish);
    list<int> find_Path(int origin, int dest);
    void updateEdgeCapacity(int node1,int node2,int bottleneck);
    int getEdgeDuration(int node1,int node2);
};

#endif
