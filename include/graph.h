#pragma once
#include <string>
using namespace std;

// Road status flags
#define ROAD_OPEN     0
#define ROAD_DAMAGED  1
#define ROAD_FLOODED  2

struct Edge {
    int destinationId;
    int weight;            // travel time
    int roadStatus;        // ROAD_OPEN / ROAD_DAMAGED / ROAD_FLOODED
    Edge* next;
};

struct Location {
    int id;
    string name;
    string type;           // "village", "camp", "hospital", "depot"
    int population;
    Edge* Head;            // head of edge list
    Edge* Tail;
    Location* next;
};

class Graph {
public:
    Location* Head;
    Location* Tail;
    int LocationCount;

    Graph();
    void AddLocation(int id, string name, string type, int population);
    void AddRoad(int fromId, int toId, int weight);   // undirected: adds both directions
    void UpdateRoadStatus(int fromId, int toId, int newStatus);
    Location* FindLocation(int id);
    bool HasRoad(int fromId, int toId);
    void Display();        // prints adjacency list
    int GetLocationCount();
};
