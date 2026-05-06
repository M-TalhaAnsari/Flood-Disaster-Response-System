#include "../include/graph.h"

// Dijkstra works ON the Graph — takes Graph as parameter

string* dijkstra(Graph& graph, string start, string end, int& totalCost){return NULL;};
// Finds shortest path from start to end using the graph's adjacency list
// Returns the path as array of location names, sets totalCost by reference
// Internally uses a simple unvisited array (no priority queue needed)
// Called by: findEvacuationRoute, DispatchNearestVehicle, prePositionVehicle, reliefDistribution

string findNearestOfType(Graph& graph, string startNode, int targetType){return NULL;};
// Runs dijkstra from startNode to every node, returns nearest one matching targetType
// targetType: 1=Camp, 2=Hospital, 3=Depot, 4=Station
// Called by: findEvacuationRoute, prePositionVehicle