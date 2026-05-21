#pragma once
#include <string>
using namespace std;

#define VEHICLE_AVAILABLE 0
#define VEHICLE_BUSY      1

struct Vehicle {
    string id;             // "V101"
    string type;           // "boat", "truck", "ambulance", "helicopter"
    int currentLocationId; // graph node id
    int status;            // VEHICLE_AVAILABLE / VEHICLE_BUSY
    int capacity;          // number of people it can carry
};


