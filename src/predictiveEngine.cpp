#include<iostream>
#include "../include/graph.h"
#include "../include/system.h"

using namespace std;

    PredictiveEngine::PredictiveEngine(Graph& g, Location* locs, int count):graph(g) {};
    // Constructor — takes references so it works on same data as FDRRC
    // Does NOT copy data, just points to it

    int PredictiveEngine::countConnectedRoads(string locationName){return -1;};
    // Counts how many edges a node has in the graph
    // Calls graph.findNode() then counts Edge* list
    // Used inside calculateRiskScore()

    int PredictiveEngine::calculateRiskScore(int locationIndex){return -1;};
    // Formula: population * floodSeverity + connectedRoads (inverse bonus)
    // Calls countConnectedRoads()
    // Called by: updateAllRiskScores(), propagateRisk()

    void PredictiveEngine::updateAllRiskScores(){};
    // Loops through all locations, calls calculateRiskScore() for each
    // Updates location.riskScore
    // Called before building the heap

    void PredictiveEngine::propagatRisk(){};
    // BFS from every high severity location
    // Spreads partial risk to neighbouring locations
    // Calls graph.findNode() to get neighbours
    // Calls calculateRiskScore() to update neighbours
    // Called before buildMaxHeap()

    void PredictiveEngine::buildMaxHeap(Location heap[], int size){};
    // Takes locations array, arranges it as max heap by riskScore
    // Uses heapify — no STL, plain array
    // Called after propagateRisk()

    void PredictiveEngine::heapify(Location heap[], int size, int index){};
    // Helper for buildMaxHeap()
    // Compares parent with children, swaps if needed
    // Called by buildMaxHeap() internally

    Location PredictiveEngine::getHighestRiskLocation(Location heap[], int size){return heap[0];};
    // Returns heap[0] — the root is always highest risk
    // Called by: FDRRC::findHighestRiskLocation(), FDRRC::prePositionVehicle()

    void PredictiveEngine::runFullAnalysis(){};
    // Master function — calls everything in order:
    // 1. updateAllRiskScores()
    // 2. propagateRisk()
    // 3. buildMaxHeap()
    // 4. prints ranked list from heap
    // Called by FDRRC menu option "Find Highest Risk Location"
