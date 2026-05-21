#include <iostream>
#include <climits>
#include <string>
#include "../include/Dijkstra.h"
#include "../include/PriorityQueue.h"
using namespace std;

Dijkstra::Dijkstra(Graph* g) {
    G = g;
    ResetArrays();
}

void Dijkstra::ResetArrays() {
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        Distance[i] = INT_MAX;
        Previous[i] = -1;
        Visited[i] = false;
    }
}

int Dijkstra::GetEffectiveWeight(Edge* edge) {
    if (edge == NULL) {
        return -1;
    }
    if (edge->roadStatus == ROAD_FLOODED) {
        return -1;             //  skip this edge
    }
    if (edge->roadStatus == ROAD_DAMAGED) {
        return edge->weight * 2;
    }
    // ROAD_OPEN
    return edge->weight;
}

// Convert int to string 
// Used to push location IDs into the PriorityQueue's HeapNode.id field.
static string IntToString(int value) {
    if (value == 0) {
        return "0";
    }

    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }

    string result = "";
    while (value > 0) {
        char digit = (char)('0' + (value % 10));
        result = digit + result;
        value = value / 10;
    }
    if (negative == true) {
        result = "-" + result;
    }
    return result;
}

// Parse a non-negative integer from a string (used for PriorityQueue ids in Dijkstra)
static int StringToInt(string s) {
    int result = 0;
    int i = 0;
    bool negative = false;
    if (s.length() > 0 && s[0] == '-') {
        negative = true;
        i = 1;
    }
    while (i < (int)s.length()) {
        result = (result * 10) + (s[i] - '0');
        i++;
    }
    if (negative == true) {
        result = -result;
    }
    return result;
}

PathResult Dijkstra::BuildResult(int sourceId, int destinationId) {
    PathResult result;
    result.found = false;
    result.totalDistance = 0;
    result.pathLength = 0;

    if (Distance[destinationId] == INT_MAX) {
        return result;     // unreachable
    }

    // Reconstruct path by walking Previous[] from destination back to source.
    int tempPath[MAX_LOCATIONS];
    int count = 0;
    int curr = destinationId;
    while (curr != -1) {
        tempPath[count] = curr;
        count++;
        if (curr == sourceId) {
            break;
        }
        curr = Previous[curr];
    }

    // Reverse into result.path
    for (int i = 0; i < count; i++) {
        result.path[i] = tempPath[count - 1 - i];
    }
    result.pathLength = count;
    result.totalDistance = Distance[destinationId];
    result.found = true;
    return result;
}

PathResult Dijkstra::FindShortestPath(int sourceId, int destinationId) {
    ResetArrays();

    PathResult failResult;
    failResult.found = false;
    failResult.totalDistance = 0;
    failResult.pathLength = 0;

    Location* src = G->FindLocation(sourceId);
    Location* dst = G->FindLocation(destinationId);
    if (src == NULL) {
        cout << "Source location " << sourceId << " not found" << endl;
        cin.get();
        cin.get();
        return failResult;
    }
    if (dst == NULL) {
        cout << "Destination location " << destinationId << " not found" << endl;
        cin.get();
        cin.get();
        return failResult;
    }
    if (sourceId == destinationId) {
        // Trivial path - just the source
        PathResult trivial;
        trivial.found = true;
        trivial.totalDistance = 0;
        trivial.pathLength = 1;
        trivial.path[0] = sourceId;
        return trivial;
    }

    Distance[sourceId] = 0;

    // PriorityQueue is reused here as Dijkstra's frontier
    PriorityQueue frontier;
    frontier.Insert(IntToString(sourceId), 0);

    while (frontier.IsEmpty() == false) {
        HeapNode current = frontier.ExtractMin();
        int currentId = StringToInt(current.id);

        if (Visited[currentId] == true) {
            continue;
        }
        Visited[currentId] = true;

        // Early-out if we've finalized the destination
        if (currentId == destinationId) {
            break;
        }

        Location* currLoc = G->FindLocation(currentId);
        if (currLoc == NULL) {
            continue;
        }

        Edge* edge = currLoc->Head;
        while (edge != NULL) {
            int effWeight = GetEffectiveWeight(edge);
            if (effWeight == -1) {
                // Flooded - skip entirely
                edge = edge->next;
                continue;
            }

            int neighborId = edge->destinationId;
            if (Visited[neighborId] == false && Distance[currentId] != INT_MAX) {
                int newDist = Distance[currentId] + effWeight;
                if (newDist < Distance[neighborId]) {
                    Distance[neighborId] = newDist;
                    Previous[neighborId] = currentId;
                    frontier.Insert(IntToString(neighborId), newDist);
                }
            }
            edge = edge->next;
        }
    }

    return BuildResult(sourceId, destinationId);
}

PathResult Dijkstra::FindNearestOfType(int sourceId, string type) {
    ResetArrays();

    PathResult failResult;
    failResult.found = false;
    failResult.totalDistance = 0;
    failResult.pathLength = 0;

    Location* src = G->FindLocation(sourceId);
    if (src == NULL) {
        cout << "Source location " << sourceId << " not found" << endl;
        cin.get();
        cin.get();
        return failResult;
    }

    Distance[sourceId] = 0;
    PriorityQueue frontier;
    frontier.Insert(IntToString(sourceId), 0);

    int foundId = -1;

    while (frontier.IsEmpty() == false) {
        HeapNode current = frontier.ExtractMin();
        int currentId = StringToInt(current.id);

        if (Visited[currentId] == true) {
            continue;
        }
        Visited[currentId] = true;

        Location* currLoc = G->FindLocation(currentId);
        if (currLoc == NULL) {
            continue;
        }

        // Check whether this is the target type (and not the source itself, unless it is the only option)
        if (currLoc->type == type && currentId != sourceId) {
            foundId = currentId;
            break;
        }

        Edge* edge = currLoc->Head;
        while (edge != NULL) {
            int effWeight = GetEffectiveWeight(edge);
            if (effWeight == -1) {
                edge = edge->next;
                continue;
            }

            int neighborId = edge->destinationId;
            if (Visited[neighborId] == false && Distance[currentId] != INT_MAX) {
                int newDist = Distance[currentId] + effWeight;
                if (newDist < Distance[neighborId]) {
                    Distance[neighborId] = newDist;
                    Previous[neighborId] = currentId;
                    frontier.Insert(IntToString(neighborId), newDist);
                }
            }
            edge = edge->next;
        }
    }

    if (foundId == -1) {
        cout << "No reachable location of type '" << type << "' from source " << sourceId << endl;
        cin.get();
        cin.get();
        return failResult;
    }

    return BuildResult(sourceId, foundId);
}

void Dijkstra::DisplayPath(PathResult result, Graph* g) {
    if (result.found == false) {
        cout << "No path found" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Path (total cost = " << result.totalDistance << "):" << endl;
    for (int i = 0; i < result.pathLength; i++) {
        Location* loc = g->FindLocation(result.path[i]);
        if (loc == NULL) {
            cout << "  [" << result.path[i] << "] (unknown)";
        } else {
            cout << "  [" << loc->id << "] " << loc->name << " (" << loc->type << ")";
        }   
        if (i < result.pathLength - 1) {
            cout << "  --> ";
        }
        cout << endl;
    }
    cin.get();
   
}
