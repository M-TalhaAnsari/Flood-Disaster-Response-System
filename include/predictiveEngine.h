#include<iostream>
#include "graph.h"
#include "system.h"

using namespace std;

class PredictiveEngine {
private:
    Graph& graph;           // reference to the same graph FDRRC uses
    Location* locations;    // pointer to FDRRC's locations array
    int locationCount;

public:
    PredictiveEngine(Graph& g, Location* locs, int count);

    int countConnectedRoads(string locationName);
    int calculateRiskScore(int locationIndex);
    void updateAllRiskScores();
    void propagatRisk();
    void buildMaxHeap(Location heap[], int size);
    void heapify(Location heap[], int size, int index);
    Location getHighestRiskLocation(Location heap[], int size);
    void runFullAnalysis();
};