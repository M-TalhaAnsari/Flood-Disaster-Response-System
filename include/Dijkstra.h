#pragma once
#include "Graph.h"
#include <string>
using namespace std;

#define MAX_LOCATIONS 200

struct PathResult {
    bool found;
    int totalDistance;
    int pathLength;
    int path[MAX_LOCATIONS];    // sequence of location IDs from source to dest
};

class Dijkstra {
public:
    Dijkstra(Graph* g);
    PathResult FindShortestPath(int sourceId, int destinationId);
    PathResult FindNearestOfType(int sourceId, string type);  // for "nearest camp", "nearest hospital"
    void DisplayPath(PathResult result, Graph* g);             // pretty-print the path with names

private:
    Graph* G;
    int Distance[MAX_LOCATIONS];
    int Previous[MAX_LOCATIONS];
    bool Visited[MAX_LOCATIONS];

    void ResetArrays();
    int GetEffectiveWeight(Edge* edge);   // returns weight, or weight*2 if damaged, or -1 if flooded
    PathResult BuildResult(int sourceId, int destinationId);
};
