#include <iostream>
#include <climits>
#include "../include/PredictiveEngine.h"
using namespace std;

string PredictiveEngine::IntToStr(int value) {
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

int PredictiveEngine::StrToInt(string s) {
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

PredictiveEngine::PredictiveEngine(Graph* g, Dijkstra* d, HashTable* vt, LinkedList* log) {
    G            = g;
    Solver       = d;
    VehicleTable = vt;
    OpsLog       = log;

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        RiskScores[i] = 0;
    }
}


int PredictiveEngine::CountRoadsByStatus(Location* loc, int status) {
    int count = 0;
    Edge* e = loc->Head;
    while (e != NULL) {
        if (e->roadStatus == status) {
            count++;
        }
        e = e->next;
    }
    return count;
}

//  Raw risk score formula per location:
//
//  Villages start with +30  (civilians live there)
//  population / 10          (bigger village = more at risk)
//  floodedRoads  * 20       (each flooded road = severely cut off)
//  damagedRoads  * 10       (each damaged road = partially cut off)
//  distanceToCamp / 2       (farther from safety = higher risk, capped at 60)
//  no camp reachable        (+100 isolation penalty)

int PredictiveEngine::ComputeRawScore(Location* loc) {
    int score = 0;

    if (loc->type == "village") {
        score += 30;
    }

    score += loc->population / 10;

    score += CountRoadsByStatus(loc, ROAD_FLOODED) * 20;
    score += CountRoadsByStatus(loc, ROAD_DAMAGED) * 10;

    PathResult campPath = Solver->FindNearestOfType(loc->id, "camp");
    if (campPath.found == false) {
        score += 100;    // completely isolated — no camp reachable
    } else {
        int distComponent = campPath.totalDistance / 2;
        if (distComponent > 60) {
            distComponent = 60;
        }
        score += distComponent;
    }

    return score;
}

void PredictiveEngine::ComputeRiskScores() {
    cout << "  [1] Computing raw risk scores..." << endl;

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        RiskScores[i] = 0;
    }

    Location* loc = G->Head;
    while (loc != NULL) {
        RiskScores[loc->id] = ComputeRawScore(loc);
        loc = loc->next;
    }

    cout << "  Raw scores computed." << endl;
}

// ─────────────────────────────────────────
//  Step 2 — BFS risk spreading
//  Villages are the flood sources.
//  Each neighbor gets += currentRisk / 2 (50% decay).
//  Flooded roads block BFS propagation entirely.
//  Uses a plain int array as queue — no STL.
// ─────────────────────────────────────────

void PredictiveEngine::SpreadRiskBFS() {
    cout << " Spreading risk to neighbours via BFS..." << endl;

    int  queue[MAX_LOCATIONS];
    bool visited[MAX_LOCATIONS];
    int  spreadBonus[MAX_LOCATIONS];

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        visited[i]     = false;
        spreadBonus[i] = 0;
    }

    int front = 0;
    int back  = 0;

    // Seed BFS with all villages — they are the primary flood sources
    Location* loc = G->Head;
    while (loc != NULL) {
        if (loc->type == "village") {
            queue[back] = loc->id;
            back++;
            visited[loc->id] = true;
        }
        loc = loc->next;
    }

    // BFS
    while (front < back) {
        int currentId     = queue[front];
        front++;

        Location* current = G->FindLocation(currentId);
        if (current == NULL) {
            continue;
        }

        int currentRisk = RiskScores[currentId] + spreadBonus[currentId];

        Edge* e = current->Head;
        while (e != NULL) {
            // Flooded roads block BFS — rescue teams cannot cross them
            if (e->roadStatus != ROAD_FLOODED) {
                int neighborId = e->destinationId;

                // 50% decay
                int spread = currentRisk / 2;
                spreadBonus[neighborId] += spread;

                if (visited[neighborId] == false) {
                    visited[neighborId] = true;
                    queue[back] = neighborId;
                    back++;
                }
            }
            e = e->next;
        }
    }

    // Apply bonuses
    Location* l = G->Head;
    while (l != NULL) {
        RiskScores[l->id] += spreadBonus[l->id];
        l = l->next;
    }

    cout << "  BFS spread complete." << endl;
}


void PredictiveEngine::BuildRiskQueue() {
    cout << " Building risk priority queue (min-heap, flipped)..." << endl;

    // Reset the queue
    RiskQueue.Size = 0;

    Location* loc = G->Head;
    while (loc != NULL) {
        int flipped = 1000 - RiskScores[loc->id];
        RiskQueue.Insert(IntToStr(loc->id), flipped);
        loc = loc->next;
    }

    cout << "  Risk queue built (" << RiskQueue.GetSize() << " locations)." << endl;
}

void PredictiveEngine::DisplayRiskRanking() {
    if (RiskQueue.IsEmpty() == true) {
        cout << "  No risk data available." << endl;
        return;
    }

    cout << endl;
    cout << "  ===== PREDICTIVE RISK RANKING (Most Dangerous First) =====" << endl;
    cout << "  Rank  ID    Name                  Type        RiskScore" << endl;
    cout << "  --------------------------------------------------------" << endl;

    // Copy heap — same pattern as HandleViewRescueQueue() in main.cpp
    PriorityQueue temp = RiskQueue;
    int rank = 1;

    while (temp.IsEmpty() == false) {
        HeapNode node  = temp.ExtractMin();
        int locId      = StrToInt(node.id);
        int realRisk   = 1000 - node.priority;   // reverse the flip

        Location* loc  = G->FindLocation(locId);
        string name    = (loc != NULL) ? loc->name : "Unknown";
        string type    = (loc != NULL) ? loc->type : "?";

        cout << "  [" << rank << "]   "
             << locId << "     "
             << name;

        // simple padding so columns line up
        int pad = 22 - (int)name.size();
        for (int i = 0; i < pad; i++) {
            cout << " ";
        }
        cout << type;
        int tpad = 12 - (int)type.size();
        for (int i = 0; i < tpad; i++) {
            cout << " ";
        }
        cout << realRisk << endl;
        rank++;
    }

    cout << "  =========================================================" << endl;
    cout << endl;
}

void PredictiveEngine::RunFullAnalysis() {
    cout << endl;
    cout << "  Running Predictive Risk Analysis..." << endl;
    cout << "  ------------------------------------" << endl;

    ComputeRiskScores();
    SpreadRiskBFS();
    BuildRiskQueue();
    DisplayRiskRanking();

    OpsLog->Append(LOG_ROUTE_COMPUTED, "Predictive risk analysis completed");
}


void PredictiveEngine::PrePositionVehicles() {
    cout << endl;
    cout << "  ===== PRE-POSITIONING VEHICLES TO HIGH-RISK ZONES =====" << endl;

    if (RiskQueue.IsEmpty() == true) {
        cout << "  No risk data. Please run Risk Analysis (menu 51) first." << endl;
        return;
    }

    // Work on a copy so the original heap stays intact
    PriorityQueue tempQueue = RiskQueue;

    // Process top 3 risk zones (or fewer if not enough locations)
    int processed = 0;
    int topN = 3;

    while (tempQueue.IsEmpty() == false && processed < topN) {
        HeapNode node  = tempQueue.ExtractMin();
        int locId      = StrToInt(node.id);
        int realRisk   = 1000 - node.priority;

        Location* riskLoc = G->FindLocation(locId);
        if (riskLoc == NULL) {
            continue;
        }

        // Only pre-position for villages — they hold civilians
        if (riskLoc->type != "village") {
            continue;
        }

        cout << "  High-risk zone: [" << locId << "] "
             << riskLoc->name
             << "  (riskScore=" << realRisk << ")" << endl;

        // Find nearest available vehicle using Dijkstra
        string bestVehicleId = "";
        int    bestDist      = -1;
        PathResult bestPath;
        bestPath.found = false;

        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            HashEntry* entry = VehicleTable->Buckets[i];
            while (entry != NULL) {
                Vehicle* candidate = (Vehicle*)entry->data;
                if (candidate != NULL && candidate->status == VEHICLE_AVAILABLE) {
                    PathResult p = Solver->FindShortestPath(
                        candidate->currentLocationId, locId);
                    if (p.found == true) {
                        if (bestDist == -1 || p.totalDistance < bestDist) {
                            bestDist      = p.totalDistance;
                            bestVehicleId = candidate->id;
                            bestPath      = p;
                        }
                    }
                }
                entry = entry->next;
            }
        }

        if (bestVehicleId == "") {
            cout << "    No available vehicle can reach this zone." << endl;
            processed++;
            continue;
        }

        Vehicle* v = (Vehicle*)VehicleTable->Search(bestVehicleId);
        if (v == NULL) {
            cout << "    Internal error: vehicle not found." << endl;
            processed++;
            continue;
        }

        // Move vehicle to the camp nearest the risk zone
        // so it sits right next to the danger zone, ready to respond
        PathResult campPath = Solver->FindNearestOfType(locId, "camp");

        if (campPath.found == true && campPath.pathLength >= 1) {
            int destId = campPath.path[campPath.pathLength - 1];
            v->currentLocationId = destId;
            Location* destLoc = G->FindLocation(destId);
            string destName = (destLoc != NULL) ? destLoc->name : IntToStr(destId);

            cout << "    Vehicle " << v->id
                 << " (" << v->type << ")"
                 << " pre-positioned to [" << destId << "] " << destName
                 << "  (nearest camp to risk zone, travel cost="
                 << bestDist << ")" << endl;

            OpsLog->Append(LOG_VEHICLE_DISPATCHED,
                "Pre-positioned " + v->id
                + " to camp near risk zone " + IntToStr(locId)
                + " (riskScore=" + IntToStr(realRisk) + ")");
        } else {
            // No camp reachable — move directly to risk zone
            v->currentLocationId = locId;
            cout << "    Vehicle " << v->id
                 << " (" << v->type << ")"
                 << " moved directly to risk zone [" << locId << "] "
                 << riskLoc->name
                 << "  (no camp reachable, travel cost=" << bestDist << ")" << endl;

            OpsLog->Append(LOG_VEHICLE_DISPATCHED,
                "Pre-positioned " + v->id
                + " directly to risk zone " + IntToStr(locId)
                + " (riskScore=" + IntToStr(realRisk) + ")");
        }

        processed++;
    }

    cout << "  =======================================================" << endl;
    cout << endl;
}