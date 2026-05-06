#include "../include/graph.h"

Edge::Edge(string d, int w){
            dest = d;
            weight = w;
            next = NULL;
}
graph_Node::graph_Node(string n){
    name = n;
    next = NULL;
    head = NULL;
}
Graph::Graph() {
    nodeHead = NULL;
}

graph_Node* Graph::findNode(string name) {
    graph_Node* temp = nodeHead;
    while (temp != NULL) {
        if (temp->name == name)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void Graph::addNode(string name) {
    graph_Node* newNode = new graph_Node(name);
    newNode->head = NULL;
    newNode->next = nodeHead;
    nodeHead = newNode;
}

void Graph::addEdge(string src, string dest, int weight) {
    graph_Node* s = findNode(src);
    graph_Node* d = findNode(dest);

    if (s == NULL || d == NULL) {
        cout << "Node not found\n";
        return;
    }

    Edge* newEdge = new Edge(dest,weight);
    newEdge->next = s->head;
    s->head = newEdge;

    // Undirected graph (optional)
    Edge* backEdge = new Edge(src,weight);
    backEdge->next = d->head;
    d->head = backEdge;
}

void Graph::display() {
    graph_Node* temp = nodeHead;

    while (temp != NULL) {
        cout << temp->name << " -> ";
        Edge* e = temp->head;

        while (e != NULL) {
            cout << "(" << e->dest << "," << e->weight << ") ";
            e = e->next;
        }

        cout << endl;
        temp = temp->next;
    }
}
