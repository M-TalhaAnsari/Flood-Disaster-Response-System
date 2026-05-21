#pragma once
#include <string>
#include "Graph.h"
#include "Dijkstra.h"
#include "PriorityQueue.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "Vehicle.h"
using namespace std;

class PredictiveEngine {
public:
    PredictiveEngine(Graph* g, Dijkstra* d, HashTable* vt, LinkedList* log);

    void RunFullAnalysis();      // computes scores, BFS spread, builds queue, shows ranking  (menu 51)
    void PrePositionVehicles();  // moves nearest vehicle to camp closest to top risk zone    (menu 52)

private:
    Graph*      G;
    Dijkstra*   Solver;
    HashTable*  VehicleTable;
    LinkedList* OpsLog;

    // Risk score per location id (index = locationId, same as Distance[] in Dijkstra)
    int RiskScores[MAX_LOCATIONS];

    // Min-heap reused with flipped priority — same trick as victims:
    //   heapPriority = 1000 - riskScore
    //   highest risk => smallest number => extracted first
    PriorityQueue RiskQueue;

    void ComputeRiskScores();
    void SpreadRiskBFS();
    void BuildRiskQueue();
    void DisplayRiskRanking();

    int    CountRoadsByStatus(Location* loc, int status);
    int    ComputeRawScore(Location* loc);
    string IntToStr(int value);
    int    StrToInt(string s);
};