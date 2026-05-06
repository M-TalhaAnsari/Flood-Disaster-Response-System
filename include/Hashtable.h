#include<iostream>
#include<limits>
#include "../include/structures.h"
using namespace std;

const int HASH_SIZE = 23;

struct HashNode {
    std::string key;   // The unique identifier
    int value;         // The data associated with the key
    HashNode* next;    // Pointer to the next node in the same bucket (for collisions)

    // Constructor to initialize a node
    HashNode(std::string k, int v) {
        key = k;
        value = v;
        next = nullptr;
    }
};

class HashTable {

    HashNode* table[HASH_SIZE]; // array of chains

    int hashFn(int);
    // Returns id % HASH_SIZE
    // Called by insert and lookup

    void insertVehicle(Vehicle);
    // Calls hashFn(), adds vehicle to correct bucket
    // Called by: FDRRC::addVehicle()

    Vehicle* lookupVehicle(int);
    // Calls hashFn(), searches chain, returns pointer or NULL
    // Called by: DispatchNearestVehicle, SearchVehicle, prePositionVehicle

    void insertRequest(Request);
    // Same as insertVehicle but for rescue requests
    // Called by: FDRRC::Create_Rescue_Request()

    Request* lookupRequest(int);
    // Same as lookupVehicle but for requests
    // Called by: DispatchNearestVehicle, searchLocation
};