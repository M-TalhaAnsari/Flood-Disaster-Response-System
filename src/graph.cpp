#include <iostream>
#include "../include/Graph.h"
using namespace std;

Graph::Graph() {
    Head = NULL;
    Tail = NULL;
    LocationCount = 0;
}

Location* Graph::FindLocation(int id) {
    Location* temp = Head;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void Graph::AddLocation(int id, string name, string type, int population) {
    if (FindLocation(id) != NULL) {
        cout << "Location with id " << id << " already exists" << endl;
        cin.get();
        return;
    }

    Location* newLocation = new Location;
    newLocation->id = id;newLocation->name = name; newLocation->type = type; newLocation->population = population; newLocation->Head = NULL; newLocation->Tail = NULL;newLocation->next = NULL;

    if (Head == NULL) {
        Head = newLocation;
        Tail = newLocation;
    } else {
        Tail->next = newLocation;
        Tail = newLocation;
    }
    LocationCount++;
}

bool Graph::HasRoad(int fromId, int toId) {
    Location* from = FindLocation(fromId);
    if (from == NULL) {
        return false;
    }

    Edge* temp = from->Head;
    while (temp != NULL) {
        if (temp->destinationId == toId) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void Graph::AddRoad(int fromId, int toId, int weight) {
    Location* from = FindLocation(fromId);
    Location* to = FindLocation(toId);

    if (from == NULL) {
        cout << "Source location " << fromId << " not found" << endl;
        cin.get();
        return;
    }
    if (to == NULL) {
        cout << "Destination location " << toId << " not found" << endl;
        cin.get();
        return;
    }
    if (fromId == toId) {
        cout << "Cannot add road from a location to itself" << endl;
        cin.get();
        return;
    }
    if (HasRoad(fromId, toId) == true) {
        cout << "Road from " << fromId << " to " << toId << " already exists" << endl;
        cin.get();
        return;
    }

    // Forward edge: from -> to
    Edge* forward = new Edge;
    forward->destinationId = toId;
    forward->weight = weight;
    forward->roadStatus = ROAD_OPEN;
    forward->next = NULL;

    if (from->Head == NULL) {
        from->Head = forward;
        from->Tail = forward;
    } else {
        from->Tail->next = forward;
        from->Tail = forward;
    }

    // Reverse edge: to -> from (undirected graph)
    Edge* reverse = new Edge;
    reverse->destinationId = fromId;
    reverse->weight = weight;
    reverse->roadStatus = ROAD_OPEN;
    reverse->next = NULL;

    if (to->Head == NULL) {
        to->Head = reverse;
        to->Tail = reverse;
    } else {
        to->Tail->next = reverse;
        to->Tail = reverse;
    }
}

void Graph::UpdateRoadStatus(int fromId, int toId, int newStatus) {
    Location* from = FindLocation(fromId);
    Location* to = FindLocation(toId);

    if (from == NULL || to == NULL) {
        cout << "One or both locations not found" << endl;
        cin.get();
        cin.get();
        return;
    }
    if (newStatus != ROAD_OPEN && newStatus != ROAD_DAMAGED && newStatus != ROAD_FLOODED) {
        cout << "Invalid road status value" << endl;
        cin.get();
        cin.get();
        return;
    }

    bool updated = false;

    // Update forward edge
    Edge* temp = from->Head;
    while (temp != NULL) {
        if (temp->destinationId == toId) {
            temp->roadStatus = newStatus;
            updated = true;
            break;
        }
        temp = temp->next;
    }

    // Update reverse edge
    temp = to->Head;
    while (temp != NULL) {
        if (temp->destinationId == fromId) {
            temp->roadStatus = newStatus;
            updated = true;
            break;
        }
        temp = temp->next;
    }

    if (updated == false) {
        cout << "No road found between " << fromId << " and " << toId << endl;
        cin.get();
        cin.get();
    }
}

void Graph::Display() {
    if (Head == NULL) {
        cout << "Graph is empty" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "------ Road Network (Adjacency List) ------" << endl;
    Location* loc = Head;
    while (loc != NULL) {
        cout << "[" << loc->id << "] " << loc->name
             << " (" << loc->type << ", pop=" << loc->population << ")";

        if (loc->Head == NULL) {
            cout << " -> no roads";
        } else {
            Edge* edge = loc->Head;
            while (edge != NULL) {
                cout << " -> " << edge->destinationId
                     << " (w=" << edge->weight << ", status=";
                if (edge->roadStatus == ROAD_OPEN) {
                    cout << "OPEN";
                } else if (edge->roadStatus == ROAD_DAMAGED) {
                    cout << "DAMAGED";
                } else if (edge->roadStatus == ROAD_FLOODED) {
                    cout << "FLOODED";
                } else {
                    cout << "UNKNOWN";
                }
                cout << ")";
                edge = edge->next;
            }
        }
        cout << endl;
        loc = loc->next;
    }
    cout << "-------------------------------------------" << endl;
    cin.get();
    cin.get();
}

int Graph::GetLocationCount() {
    return LocationCount;
}
