#ifndef SYSTEM_H
#define SYSTEM_H

#include "graph.h"
#include "linkedlist.h"
#include "Hashtable.h"
#include "structures.h"
#include "predictiveEngine.h"

class FDRRC {
private:
    Graph graph;
    LinkedList logs;
    HashTable vehicleTable;  
    HashTable requestTable;  
    Location locations[20];  
    PredictiveEngine engine;
    int locationCount;        
    Vehicle vehicles[20];     
    int vehicleCount;         

public:
    FDRRC();
    void menu();
    
    void addVehicle();
    // Takes input (type, startLocation, capacity)
    // Stores in vehicles[], calls vehicleTable.insertVehicle()
    // Does NOT call Dijkstra
    
    void reportflood();
    // Takes locationID and severity level
    // Updates location's floodSeverity
    // Calls UpdateFloodSeverity() internally
    // Calls logs.insert()

    void addLocation();
    // Takes input from user (name, type, population)
    // Calls graph.addNode(), stores in locations[]
    // Does NOT call Dijkstra

    void addRoad();
    // Takes input (src, dest, travelTime)
    // Calls graph.addEdge()
    // Does NOT call Dijkstra

    void UpdateRoadStatus();
    // Asks user: damaged or flooded?
    // If flooded: calls graph.removeEdge()
    // If damaged: calls graph.updateEdgeWeight()

    void UpdateFloodSeverity();
    // Takes locationID, increases or decreases severity
    // Recalculates riskScore = population * severity
    // If severity hits 0: calls graph.addEdge() to restore road
    // Calls logs.insert() to record it

    void Create_Rescue_Request();
    // Takes input (location, numPeople, severity)
    // Calculates urgencyScore = numPeople * severity
    // Calls requestTable.insertRequest()
    // Calls logs.insert()

    void findEvacuationRoute();
    // Takes a village name from user
    // Calls findNearestOfType() to get nearest camp
    // Calls dijkstra() to get the path and cost
    // Prints the route, calls logs.insert()

    void DispatchNearestVehicle();
    // Takes a requestID from user
    // Calls requestTable.lookupRequest() to get victim location
    // Loops through vehicles[], calls dijkstra() from each available vehicle
    // Picks the one with smallest distance, marks it dispatched
    // Calls vehicleTable, calls logs.insert()

    void findHighestRiskLocation();
    // Loops through locations[], finds highest riskScore
    // Prints that location — does NOT call Dijkstra
    // Called by prePositionVehicle() as well

    void prePositionVehicle();
    // Calls findHighestRiskLocation() to get the danger zone
    // Calls findNearestOfType() to get nearest rescue station
    // Calls dijkstra() from each available vehicle to that station
    // Moves nearest available vehicle there
    // Calls logs.insert()

     
    void searchLocation();
    // Takes location name, searches locations[] array
    // Prints all details: type, population, severity, riskScore
    // Does NOT call Dijkstra or HashTable

    void SearchVehicle();
    // Takes vehicle ID, calls vehicleTable.lookupVehicle()
    // Prints vehicle details and current status
    // Does NOT call Dijkstra

    void displayOperationLog();
    // Simply calls logs.display()
    // Does NOT call anything else

};

#endif