#include <iostream>
#include <string>
#include "../include/Graph.h"
#include "../include/Dijkstra.h"
#include "../include/PriorityQueue.h"
#include "../include/HashTable.h"
#include "../include/LinkedList.h"
#include "../include/Vehicle.h"
#include "../include/Victim.h"
#include "../include/PredictiveEngine.h"
using namespace std;

// Global data structures
Graph cityMap;
HashTable vehicleTable;
HashTable victimTable;
PriorityQueue rescueQueue;
LinkedList opsLog;
Dijkstra solver(&cityMap);
PredictiveEngine riskEngine(&cityMap, &solver, &vehicleTable, &opsLog);

//  victim IDs countinue from this
int nextVictimNumber = 5001;

// helper function: for log purposr
static string IntToStr(int value) {
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

void clearScreen()
{
    system("cls");
}

static string RoadStatusName(int status) {
    if (status == ROAD_OPEN) {
        return "OPEN";
    } else if (status == ROAD_DAMAGED) {
        return "DAMAGED";
    } else if (status == ROAD_FLOODED) {
        return "FLOODED";
    }
    return "UNKNOWN";
}

// demo data
void SeedDemoData() {
    cout << "Seeding demo data..." << endl;

    // 7 locations: mix of villages, camps, hospitals, depots
    cityMap.AddLocation(1, "Swat",     "village",  500);
    cityMap.AddLocation(2, "Abottabad",     "village",  320);
    cityMap.AddLocation(3, "Charsadda",   "village",  410);
    cityMap.AddLocation(4, "Camp Alpha",        "camp",     0);
    cityMap.AddLocation(5, "Camp Bravo",         "camp",     0);
    cityMap.AddLocation(6, "CMH",  "hospital", 0);
    cityMap.AddLocation(7, "Depot Golf",        "depot",    0);

    opsLog.Append(LOG_LOCATION_ADDED, "Seeded 7 locations (3 villages, 2 camps, 1 hospital, 1 depot)");

    // ~10 roads connecting the network
    cityMap.AddRoad(1, 2, 10);
    cityMap.AddRoad(1, 4, 15);
    cityMap.AddRoad(2, 3, 12);
    cityMap.AddRoad(2, 4, 8);
    cityMap.AddRoad(3, 5, 20);
    cityMap.AddRoad(4, 5, 7);
    cityMap.AddRoad(4, 6, 18);
    cityMap.AddRoad(5, 6, 9);
    cityMap.AddRoad(6, 7, 11);
    cityMap.AddRoad(3, 7, 25);

    opsLog.Append(LOG_ROAD_ADDED, "Seeded 10 roads across the network");

    // 4 vehicles parked at various locations
    Vehicle* v1 = new Vehicle;
    v1->id = "V101"; v1->type = "boat";      v1->currentLocationId = 4; v1->status = VEHICLE_AVAILABLE; v1->capacity = 8;
    vehicleTable.Insert(v1->id, (void*)v1);

    Vehicle* v2 = new Vehicle;
    v2->id = "V102"; v2->type = "truck";     v2->currentLocationId = 7; v2->status = VEHICLE_AVAILABLE; v2->capacity = 20;
    vehicleTable.Insert(v2->id, (void*)v2);

    Vehicle* v3 = new Vehicle;
    v3->id = "V103"; v3->type = "ambulance"; v3->currentLocationId = 6; v3->status = VEHICLE_AVAILABLE; v3->capacity = 4;
    vehicleTable.Insert(v3->id, (void*)v3);

    Vehicle* v4 = new Vehicle;
    v4->id = "V104"; v4->type = "boat";      v4->currentLocationId = 5; v4->status = VEHICLE_AVAILABLE; v4->capacity = 6;
    vehicleTable.Insert(v4->id, (void*)v4);

    Vehicle* v5 = new Vehicle;
    v5->id = "V105";    v5->type = "helicopter";   v5->currentLocationId = 7;    v5->status = VEHICLE_AVAILABLE;  v5->capacity = 6;
    vehicleTable.Insert(v5->id, (void*)v5);

    opsLog.Append(LOG_LOCATION_ADDED, "Seeded 4 rescue vehicles (V101..V104)");

    // Mark one road as damaged and one as flooded to make Dijkstra interesting
    cityMap.UpdateRoadStatus(3, 7, ROAD_DAMAGED);
    opsLog.Append(LOG_ROAD_STATUS_CHANGED, "Road 3 <-> 7 marked DAMAGED");

    cityMap.UpdateRoadStatus(1, 4, ROAD_FLOODED);
    opsLog.Append(LOG_ROAD_STATUS_CHANGED, "Road 1 <-> 4 marked FLOODED");

    cout << "Demo data seeded." << endl << endl;
}

// Menu
void PrintHeading(){
    cout << "============================================" << endl;
    cout << "  FDRC - Flood Disaster Response System"     << endl;
    cout << "============================================" << endl;
}
void PrintMenu() {
    cout << endl;
    clearScreen();
    PrintHeading();
    cout << "  1. Location and Road Management"            << endl;
    cout << "     11. Add Location"                        << endl;
    cout << "     12. Add Road"                            << endl;
    cout << "     13. Update Road Status"                  << endl;
    cout << "     14. Display Network"                     << endl;
    cout << endl;
    cout << "  2. Citizen Emergency Reporting"             << endl;
    cout << "     21. Report Emergency"                    << endl;
    cout << "     22. View Pending Rescue Queue"           << endl;
    cout << endl;
    cout << "  3. Route Finding and Dispatch"              << endl;
    cout << "     31. Find Evacuation Route (village -> nearest camp)" << endl;
    cout << "     32. Dispatch Vehicle to Most Urgent Victim"          << endl;
    cout << "     33. Plan Relief Delivery Route (depot -> village)"   << endl;
    cout << endl;
    cout << "  4. Search and Reports"                      << endl;
    cout << "     41. Search Location by ID"               << endl;
    cout << "     42. Search Vehicle by ID"                << endl;
    cout << "     43. Search Victim by ID"                 << endl;
    cout << "     44. View Operations Log"                 << endl;
    cout << endl;
    cout << "  5. Predictive Risk Analysis"                               << endl;
    cout << "     51. Run Risk Analysis (BFS + Ranking)"                  << endl;
    cout << "     52. Pre-Position Vehicles to High-Risk Zones"           << endl;
    cout << endl;
    cout << "  0. Exit"                                    << endl;
    cout << "============================================" << endl;
    cout << "Enter choice: ";
}

// handles for each menu option
void HandleAddLocation() {
    clearScreen();
    PrintHeading();
    cout << "--- Location & Road Management Tab ---\n";
    int id; int population;
    string name; string type;

    cout << "Enter location id (integer): ";
    cin >> id;

    if (cityMap.FindLocation(id) != NULL) {
        cout << "Location with ID " << id << " already exists. Cannot add duplicate." << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Enter name (single token): ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter type (village/camp/hospital/depot): ";
    getline(cin, type);
    cout << "Enter population (0 for non-village): ";
    cin >> population;

    cityMap.AddLocation(id, name, type, population);
    opsLog.Append(LOG_LOCATION_ADDED,
                  "Added location " + IntToStr(id) + " (" + name + ", " + type + ")");
    cout << "Location " << id << " (" << name << ") added successfully." << endl;

    if (type == "depot" || type == "camp") {
        char choice;
        cout << "Add a vehicle at this location or elsewhere? (y/n): ";
        cin >> choice;

        do {
            if (choice != 'y' && choice != 'Y') break;
            int vehicleLocId; int vcapacity;string vid, vtype;

            cout << "Enter location id for vehicle: ";
            cin >> vehicleLocId;
            Location* vLoc = cityMap.FindLocation(vehicleLocId);
            if (vLoc == NULL) {
                cout << "Location not found. Skipping." << endl;
            } else if (vLoc->type != "depot" && vLoc->type != "camp") {
                cout << "Vehicle can only be at depot or camp. Skipping." << endl;
            } else {
                cout << "Enter vehicle id (e.g. V106): ";
                cin >> vid;

                if (vehicleTable.Search(vid) != NULL) {
                    cout << "Vehicle " << vid << " already exists. Skipping." << endl;
                } else {
                    cout << "Enter type (boat/truck/ambulance/helicopter): ";
                    cin >> vtype;
                    cout << "Enter capacity: ";
                    cin >> vcapacity;

                    Vehicle* v = new Vehicle;v->id  = vid;  v->type = vtype;  v->currentLocationId = vehicleLocId;  v->status = VEHICLE_AVAILABLE; v->capacity = vcapacity;
                    vehicleTable.Insert(v->id, (void*)v);

                    cout << "Vehicle " << vid << " (" << vtype
                         << ") added at " << vLoc->name << "." << endl;

                    opsLog.Append(LOG_VEHICLE_DISPATCHED,
                        "Vehicle " + vid + " added at location "
                        + IntToStr(vehicleLocId) + " (" + vLoc->name + ")");
                }
            }

            cout << "Add another vehicle? (y/n): ";
            cin >> choice;

        } while (choice == 'y' || choice == 'Y');}

    cin.get();
}

void HandleAddRoad() {
    clearScreen();
    PrintHeading();
    cout << "--- Location & Road Management Tab ---\n";
    int fromId, toId, weight;
    cout << "Enter source location id: ";
    cin >> fromId;
    cout << "Enter destination location id: ";
    cin >> toId;
    cout << "Enter road weight (travel time): ";
    cin >> weight;

    cityMap.AddRoad(fromId, toId, weight);
    opsLog.Append(LOG_ROAD_ADDED,
                  "Added road " + IntToStr(fromId) + " <-> " + IntToStr(toId)
                  + " (w=" + IntToStr(weight) + ")");
}

void HandleUpdateRoadStatus() {
    clearScreen();
    PrintHeading();
    cout << "--- Location & Road Management Tab ---\n";
    int fromId, toId, status;
    cout << "Enter source location id: ";
    cin >> fromId;
    cout << "Enter destination location id: ";
    cin >> toId;
    cout << "Enter new status (0=OPEN, 1=DAMAGED, 2=FLOODED): ";
    cin >> status;

    cityMap.UpdateRoadStatus(fromId, toId, status);
    opsLog.Append(LOG_ROAD_STATUS_CHANGED,
                  "Road " + IntToStr(fromId) + " <-> " + IntToStr(toId)
                  + " status set to " + RoadStatusName(status));
}

void HandleReportEmergency() {
    clearScreen();
    PrintHeading();
    cout << "--- Citizen Emergency Reporting Tab ---\n";
    int locationId, numPeople, severity;

    cout << "Enter location id where victim is: ";
    cin >> locationId;

    Location* loc = cityMap.FindLocation(locationId);
    if (loc == NULL) {
        cout << "Location " << locationId << " does not exist. Aborting." << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Enter number of people in need: ";
    cin >> numPeople;
    cout << "Enter severity (1-5, 5 = most severe): ";
    cin >> severity;

    if (numPeople <= 0) {
        cout << "Number of people must be positive" << endl;
        cin.get();
        cin.get();
        return;
    }
    if (severity < 1 || severity > 5) {
        cout << "Severity must be between 1 and 5" << endl;
        cin.get();
        cin.get();
        return;
    }

    // Build victim
    Victim* vic = new Victim;
    vic->id = "VIC" + IntToStr(nextVictimNumber);
    nextVictimNumber++;
    vic->locationId = locationId;
    vic->numPeople = numPeople;
    vic->severity = severity;

    int rawUrgency = (severity * 10) + numPeople;
    vic->urgency = 1000 - rawUrgency;   // min-heap: smaller = more urgent
    vic->rescued = false;

    victimTable.Insert(vic->id, (void*)vic);
    rescueQueue.Insert(vic->id, vic->urgency);

    cout << "Emergency reported. Assigned id " << vic->id
         << " (heap priority=" << vic->urgency << ")" << endl;

    opsLog.Append(LOG_VICTIM_REPORTED,
                  "Victim " + vic->id + " reported at location " + IntToStr(locationId)
                  + " (severity=" + IntToStr(severity)
                  + ", people=" + IntToStr(numPeople) + ")");
    cin.get();
    cin.get();
}

void HandleViewRescueQueue() {
    clearScreen();
    PrintHeading();
    cout << "--- Citizen Emergency Reporting Tab ---\n";
    if (rescueQueue.IsEmpty() == true) {
        cout << "Rescue queue is empty" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Pending rescue queue (heap order, root = most urgent):" << endl;
    rescueQueue.Display();

    // Also print each victim's underlying data for clarity
    cout << endl << "Victim details:" << endl;
    for (int i = 0; i < rescueQueue.GetSize(); i++) {
        string vid = rescueQueue.Data[i].id;
        Victim* v = (Victim*)victimTable.Search(vid);
        if (v != NULL) {
            cout << "  " << v->id
                 << "  locID=" << v->locationId
                 << "  Location Name=" << cityMap.FindLocation(v->locationId)->name
                 << "  people=" << v->numPeople
                 << "  severity=" << v->severity
                 << "  heapPriority=" << v->urgency << endl;
        }
    }
   
    cin.get();
}

void HandleEvacuationRoute() {
    clearScreen();
    PrintHeading();
    cout << "--- Route Finding & Dispatch Tab ---\n";
    int villageId;
    cout << "Enter village location id (evacuating from): ";
    cin >> villageId;

    Location* src = cityMap.FindLocation(villageId);
    if (src == NULL) {
        cout << "Location not found" << endl;
        cin.get();
        cin.get();
        return;
    }
    // find severity of victim(s) at this location to help determine routing priority
    int severity = 0;
    for (int i = 0; i < rescueQueue.GetSize(); i++) {
        string vid = rescueQueue.Data[i].id;
        Victim* v = (Victim*)victimTable.Search(vid);
        if (v != NULL && v->locationId == villageId) {
            if (v->severity > severity) {
                severity = v->severity;
            }
        }
    }
    
    PathResult result;
    if (severity >= 4) 
    {
        result = solver.FindNearestOfType(villageId, "hospital");
        if (!result.found) {
            cout << "No hospital reachable -- falling back to nearest camp" << endl;
            result = solver.FindNearestOfType(villageId, "camp");
        } 
        else {
            cout << "Severity is 4 -- Critical case -- routing to nearest hospital" << endl;
        }
   } 
    else {
        result = solver.FindNearestOfType(villageId, "camp");
        cout << "Routing to nearest relief camp" << endl;
    }
    if (result.found == false) {
        cout << "No reachable camp from this village" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Evacuation route from " << src->name << " to nearest camp or hospital:" << endl;
    solver.DisplayPath(result, &cityMap);

    opsLog.Append(LOG_ROUTE_COMPUTED,
                  "Evacuation route computed from " + IntToStr(villageId)
                  + " (cost=" + IntToStr(result.totalDistance) + ")");
      
        cin.get();
}

void HandleDispatchVehicle() {
    clearScreen();
    PrintHeading();
    cout << "--- Route Finding & Dispatch Tab ---\n";

    if (rescueQueue.IsEmpty() == true) {
        cout << "Rescue queue is empty - no victims to dispatch to" << endl;
        cin.get();
        cin.get();
        return;
    }

    HeapNode top = rescueQueue.ExtractMin();
    Victim* victim = (Victim*)victimTable.Search(top.id);
    if (victim == NULL) {
        cout << "Heap entry references missing victim record - skipping" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Most urgent victim: " << victim->id
         << " at location " << victim->locationId
         << " (severity=" << victim->severity
         << ", people=" << victim->numPeople << ")" << endl;

    if (victim->severity == 5) {
        cout << "  CRITICAL severity 5 -- searching for helicopter first!" << endl;
    }

    string bestVehicleId = "";
    int bestDist = -1;
    PathResult bestPath;
    bestPath.found = false;
    bestPath.totalDistance = 0;
    bestPath.pathLength = 0;
    bool helicopterFound = false;

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (helicopterFound == true) break;
        HashEntry* entry = vehicleTable.Buckets[i];
        while (entry != NULL) {
            Vehicle* candidate = (Vehicle*)entry->data;
            if (candidate != NULL && candidate->status == VEHICLE_AVAILABLE) {

                if (victim->severity == 5 && candidate->type == "helicopter") {
                    PathResult p = solver.FindShortestPath(
                        candidate->currentLocationId, victim->locationId);
                    if (p.found == true) {
                        bestDist        = p.totalDistance;
                        bestVehicleId   = candidate->id;
                        bestPath        = p;
                        helicopterFound = true;
                        break;
                    }
                }

                if (victim->severity != 5 || bestVehicleId == "") {
                    PathResult p = solver.FindShortestPath(
                        candidate->currentLocationId, victim->locationId);
                    if (p.found == true) {
                        if (bestDist == -1 || p.totalDistance < bestDist) {
                            bestDist      = p.totalDistance;
                            bestVehicleId = candidate->id;
                            bestPath      = p;
                        }
                    }
                }
            }
            entry = entry->next;
        }
    }

    if (bestVehicleId == "") {
        cout << "No available vehicle can reach the victim. Re-queueing victim." << endl;
        rescueQueue.Insert(victim->id, victim->urgency);
        return;
    }

    Vehicle* assigned = (Vehicle*)vehicleTable.Search(bestVehicleId);
    if (assigned == NULL) {
        cout << "Internal error: vehicle disappeared" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Dispatching vehicle " << assigned->id
         << " (" << assigned->type << ", at loc=" << assigned->currentLocationId
         << ") -- total travel cost = " << bestDist << endl;
    solver.DisplayPath(bestPath, &cityMap);

    assigned->status = VEHICLE_BUSY;
    assigned->currentLocationId = victim->locationId;
    victim->rescued = true;

    opsLog.Append(LOG_VEHICLE_DISPATCHED,
                  "Vehicle " + assigned->id + " dispatched to victim "
                  + victim->id + " (cost=" + IntToStr(bestDist) + ")");
    opsLog.Append(LOG_RESCUE_COMPLETED,
                  "Rescue of " + victim->id + " marked complete");
   
    cin.get();
}

void HandleReliefDelivery() {
    clearScreen();
    PrintHeading();
    cout << "--- Route Finding & Dispatch Tab ---\n";
    int depotId, villageId;
    cout << "Enter depot location id: ";
    cin >> depotId;
    cout << "Enter destination village id: ";
    cin >> villageId;

    Location* depot = cityMap.FindLocation(depotId);
    Location* village = cityMap.FindLocation(villageId);
    if (depot == NULL || village == NULL) {
        cout << "One or both locations not found" << endl;
        cin.get();
        cin.get();
        return;
    }

     if (village->type != "village") {
        cout << "Warning: destination location is not a village. " << endl;
        cin.get();
        return;
    }
    PathResult result;
    if (depot->type != "depot") {
        cout << "Warning: source location is not a depot. Proceeding anyway." << endl;
        result = solver.FindNearestOfType(villageId, "depot");  // auto-reroute from nearest depot
        
    }
    else{
       result = solver.FindShortestPath(depotId, villageId);
    }

   

    if (result.found == false) {
        cout << "No path from " << depot->name << " to " << village->name << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Relief delivery route from " << result.path[result.pathLength - 1] 
         << " to " << village->name << ":" << endl;
    solver.DisplayPath(result, &cityMap);

    opsLog.Append(LOG_DELIVERY_MADE,
                  "Relief delivery route planned " + IntToStr(result.path[result.pathLength - 1])
                  + " -> " + IntToStr(villageId)
                  + " (cost=" + IntToStr(result.totalDistance) + ")");
    cin.get();
  
    
}


void HandleSearchLocation() {
    clearScreen();
    PrintHeading();
    cout << "--- Search & Report Tab ---\n";
    int id;
    cout << "Enter location id: ";
    cin >> id;

    Location* loc = cityMap.FindLocation(id);
    if (loc == NULL) {
        cout << "Location " << id << " not found" << endl;
        return;
    }

    cout << "Location " << loc->id << ": " << loc->name
         << " (type=" << loc->type
         << ", population=" << loc->population << ")" << endl;

    if (loc->Head == NULL) {
        cout << "  No outgoing roads" << endl;
    } else {
        cout << "  Roads:" << endl;
        Edge* e = loc->Head;
        while (e != NULL) {
            cout << "    -> " << e->destinationId
                 << " (w=" << e->weight
                 << ", status=" << RoadStatusName(e->roadStatus) << ")" << endl;
            e = e->next;
        }
    }
    cin.get(); 
    cin.get(); 
}

void HandleSearchVehicle() {
    clearScreen();
    PrintHeading();
    cout << "--- Search & Report Tab ---\n";
    string id;
    cout << "Enter vehicle id (e.g. V101): ";
    cin >> id;

    Vehicle* v = (Vehicle*)vehicleTable.Search(id);
    if (v == NULL) {
        cout << "Vehicle " << id << " not found" << endl;
        return;
    }

    cout << "Vehicle " << v->id
         << "  type=" << v->type
         << "  currentLocation=" << v->currentLocationId
         << "  capacity=" << v->capacity
         << "  status=";
    if (v->status == VEHICLE_AVAILABLE) {
        cout << "AVAILABLE";
    } else {
        cout << "BUSY";
    }
    cout << endl;
    cin.get();
    cin.get();
}

void HandleRunRiskAnalysis() {
    clearScreen();
    PrintHeading();
    cout << "--- Predictive Risk Analysis Tab ---" << endl;
    riskEngine.RunFullAnalysis();
    cin.get();
    cin.get();
}

 
void HandlePrePositionVehicles() {
    clearScreen();
    PrintHeading();
    cout << "--- Predictive Risk Analysis Tab ---" << endl;
    // Analysis must be fresh before pre-positioning
    riskEngine.RunFullAnalysis();
    riskEngine.PrePositionVehicles();
    cin.get();
    cin.get();
}

void HandleSearchVictim() {
    clearScreen();
    PrintHeading();
    cout << "--- Search & Report Tab ---\n";
    string id;
    cout << "Enter victim id (e.g. VIC5001): ";
    cin.ignore();
    getline(cin,id);

    Victim* v = (Victim*)victimTable.Search(id);
    if (v == NULL) {
        cout << "Victim " << id << " not found" << endl;
        cin.get();
        
        return;
    }

    cout << "Victim " << v->id
         << "  location=" << v->locationId
         << "  people=" << v->numPeople
         << "  severity=" << v->severity
         << "  heapPriority=" << v->urgency
         << "  rescued=";
    if (v->rescued == true) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    cout << endl;
    cin.get();
   
}

void HandleViewLog() {
    clearScreen();
    PrintHeading();
    cout << "--- Search & Report Tab ---\n";
    opsLog.Display();
}

// Main Loop

int main() {
    SeedDemoData();

    int choice;
    do {
        PrintMenu();
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting FDRC. Stay safe." << endl;
        } else if (choice == 11) {
            HandleAddLocation();
        } else if (choice == 12) {
            HandleAddRoad();
        } else if (choice == 13) {
            HandleUpdateRoadStatus();
        } else if (choice == 14) {
            clearScreen();
            PrintHeading();
            cout << "--- Location & Road Management Tab ---\n";
            cityMap.Display();
        } else if (choice == 21) {
            HandleReportEmergency();
        } else if (choice == 22) {
            HandleViewRescueQueue();
        } else if (choice == 31) {
            HandleEvacuationRoute();
        } else if (choice == 32) {
            HandleDispatchVehicle();
        } else if (choice == 33) {
            HandleReliefDelivery();
        } else if (choice == 41) {
            HandleSearchLocation();
        } else if (choice == 42) {
            HandleSearchVehicle();
        } else if (choice == 43) {
            HandleSearchVictim();
        } else if (choice == 44) {
            HandleViewLog();
        }  else if (choice == 51) {
            HandleRunRiskAnalysis();
        } else if (choice == 52) {
            HandlePrePositionVehicles();
        } else {
            cout << "Invalid choice. Try again." << endl;
            cin.get();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}
