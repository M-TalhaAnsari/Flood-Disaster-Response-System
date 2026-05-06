#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>


using namespace std;
class Edge {
public:
        string dest;
        int weight;
        Edge* next;
        Edge(string, int);
    };

class graph_Node {
public:
        string name;
        Edge* head;
        graph_Node* next;
        graph_Node(string);
    };

class Graph {
public:
    graph_Node* nodeHead;
    Graph();
    void addNode(string name);
    // Creates a new graph_Node and adds to list
    // Does NOT call anything else

    void addEdge(string src, string dest, int weight);
    // Calls findNode() to get src node, then adds Edge to its list
    // Undirected: adds edge both ways
    
    void removeEdge(string src, string dest);
    // Calls findNode(), then removes edge from adjacency list
    // Used when a road gets FLOODED (completely gone)

    void updateEdgeWeight(string src, string dest, int newWeight);
    // Calls findNode(), finds the edge, changes its weight
    // Used when a road gets DAMAGED (slower but not gone)

    void display();
    // Traverses all nodes and their edges, prints them
    // Does NOT call anything else

    graph_Node* findNode(string name);
    // Traverses nodeHead list, returns matching node or NULL
    // Called by almost every other Graph function
};

#endif