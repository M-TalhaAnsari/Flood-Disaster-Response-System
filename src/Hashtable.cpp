#include<iostream>
#include<limits>
#include "../include/Hashtable.h"
using namespace std;


int HashTable::hashFn(int id){return -1;};
    // Returns id % HASH_SIZE
    // Called by insert and lookup

void HashTable::insertVehicle(Vehicle v){};
    // Calls hashFn(), adds vehicle to correct bucket
    // Called by: FDRRC::addVehicle()

Vehicle* HashTable::lookupVehicle(int id){return NULL;};
    // Calls hashFn(), searches chain, returns pointer or NULL
    // Called by: DispatchNearestVehicle, SearchVehicle, prePositionVehicle

void HashTable::insertRequest(Request r){};
    // Same as insertVehicle but for rescue requests
    // Called by: FDRRC::Create_Rescue_Request()

Request* HashTable::lookupRequest(int id){return NULL;};
    // Same as lookupVehicle but for requests
    // Called by: DispatchNearestVehicle, searchLocation
