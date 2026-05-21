#pragma once
#include <string>
using namespace std;

// Action types for log entries
#define LOG_LOCATION_ADDED      1
#define LOG_ROAD_ADDED          2
#define LOG_ROAD_STATUS_CHANGED 3
#define LOG_VICTIM_REPORTED     4
#define LOG_VEHICLE_DISPATCHED  5
#define LOG_ROUTE_COMPUTED      6
#define LOG_RESCUE_COMPLETED    7
#define LOG_DELIVERY_MADE       8

struct LogEntry {
    int actionType;          // one of the LOG_* macros
    string description;      // human-readable summary
    int sequenceNumber;      // 1, 2, 3, ... assigned at insertion
    LogEntry* next;
};

class LinkedList {
public:
    LogEntry* Head;
    LogEntry* Tail;
    int Size;

    LinkedList();
    void Append(int actionType, string description);
    void Display();          // traverses from Head to Tail, prints all entries
    int GetSize();
};
